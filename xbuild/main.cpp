#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <base/proc.h>
#include <regex>
#include <stdlib.h>
#include <unistd.h>
#include <filesystem>
#include <sys/stat.h>

#include "json.hpp"

using std::map;
using std::string;
using std::vector;
using std::tuple;

typedef map<string, vector<string>> Loop;

void error(const string &s)
{
	std::cerr << s << "\n";
	exit(-1);
}

struct Dep {
	vector<string> outputs;
	vector<string> inputs;
};

class Target {
	public:
	vector<string> targets;
	string makeDeps;
	Dep dep;
	vector<vector<string>> exec;
};

void readMakeDepFile(Target& t) {
	std::ifstream file(t.makeDeps);
	if (!file.is_open()) {
		return;
	}

	std::string line((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();
	
	// Find the position of the first colon
	size_t colonPos = line.find(':');
	if (colonPos == std::string::npos) {
		error("Error: Invalid make line format\n");
	}

	// Extract outputs
	t.dep.outputs.push_back(line.substr(0, colonPos));

	// Extract inputs
	std::stringstream ss(line.substr(colonPos + 1));
	std::string input;
	while (ss >> input) {
		if (input == "\\") { // Handle line continuation
			continue;
		}
		if (input[0] != '/') {
			input = std::filesystem::current_path().string() + "/" + input;
		}
		t.dep.inputs.push_back(input);
	}

	return;
}

void appendStrings(map<string, string> &map, Loop &loop, json::JSON &list, string input) {
	for (auto m : map) {
		input = std::regex_replace(input, std::regex("\\{" + m.first + "\\}"), m.second);
	}
	vector<string> strings = { input };
	for (auto l : loop) {
		vector<string> new_string = {};
		for (auto s : strings) {
			string key = "\\{" + l.first + "\\}";
			if (s.find("{" + l.first + "}") != string::npos)
			{
				for (auto replace : l.second) {
					new_string.push_back(std::regex_replace(s, std::regex(key), replace));
				}
			} else {
				new_string.push_back(s);
			}
		}
		strings = new_string;
	}
	for (auto s : strings) {
		list.append(s);
	}
}

json::JSON replace(map<string, string> &map, Loop &loop, const json::JSON &input);

void expandObject(map<string, string> &map, Loop &loop, json::JSON &obj, string key, const json::JSON& value) {
	bool found = false;
	for (auto l : loop)
	{
		if (key.find("{" + l.first + "}") != string::npos)
		{
			loop.erase(l.first);
			for (auto lv : l.second)
			{
				map[l.first] = lv;
				string key2 = std::regex_replace(key, std::regex("\\{" + l.first + "\\}"), lv);
				expandObject(map, loop, obj, key2, value);
				map.erase(l.first);
			}
			loop[l.first] = l.second;
			found = true;
			break;
		}
	}
	if (found == false) {
		for (auto m : map)
		{
			key = std::regex_replace(key, std::regex("\\{" + m.first + "\\}"), m.second);
		}
		obj[key] = replace(map, loop, value);
	}
}

json::JSON replace(map<string, string> &map, Loop &loop, const json::JSON &input) {
	if (input.JSONType() == json::JSON::Class::String) {
		string out = input.ToString();
		for (auto m : map) {
			out = std::regex_replace(out, std::regex("\\{" + m.first + "\\}"), m.second);
		}
		for (auto l : loop) {
			if (l.second.size() == 1) {
				out = std::regex_replace(out, std::regex("\\{" + l.first + "\\}"), l.second[0]);
			}
		}
		return json::JSON(out);
	}
	if (input.JSONType() == json::JSON::Class::Array) {
		json::JSON out = json::Array();
		for (auto j : input.ArrayRange()) {
			if (j.JSONType() == json::JSON::Class::String)
			{
				appendStrings(map, loop, out, j.ToString());
			} else {
				out.append(replace(map, loop, j));
			}
		}
		return out;
	}
	if (input.JSONType() == json::JSON::Class::Object) {
		json::JSON out = json::Object();
		for (auto j : input.ObjectRange()) {
			expandObject(map, loop, out, j.first, j.second);
		}
		return out;
	}
	return input;
}

void execTarget(Target &t) {
	if (!t.makeDeps.empty()) {
		readMakeDepFile(t);
	}
	if (t.dep.outputs.size() >= 1) {
		struct timespec oldestOut = { 1000000000000000000, 0 };
		bool rebuild = false;
		for (string o : t.dep.outputs) {
			struct stat s;
			int32_t err = stat(o.c_str(), &s);
			if (err) { rebuild = true; continue; }
			if (s.st_mtim.tv_sec < oldestOut.tv_sec || s.st_mtim.tv_sec == oldestOut.tv_sec && s.st_mtim.tv_nsec < oldestOut.tv_nsec) {
				oldestOut.tv_sec = s.st_mtim.tv_sec;
				oldestOut.tv_nsec = s.st_mtim.tv_nsec;
			}
		}
		for (string i : t.dep.inputs) {
			struct stat s;
			int32_t err = stat(i.c_str(), &s);
			if (err) { rebuild = true; continue; }
			if (s.st_mtim.tv_sec > oldestOut.tv_sec || s.st_mtim.tv_sec == oldestOut.tv_sec && s.st_mtim.tv_nsec > oldestOut.tv_nsec) {
				rebuild = true;
			}
		}
		if (!rebuild) {
			std::cout << "already built\n";
			return;
		}
	}
	for (auto exec: t.exec) {
		const char ** args = (const char**)calloc(exec.size() + 1, sizeof(char*));
		int i = 0;
		std::cout << "run: ";
		for (string & arg : exec) {
			std::cout << arg << " ";
			args[i] = arg.c_str();
			i++;
		}
		std::cout << std::endl;
		uintptr_t pid;
		const char * env[] = { 0 };
		int32_t err = base_proc_exec(args[0], args, env, 0, &pid);
		uint8_t exit = 0;
		if (err != 0) {
			error("exec failed");
		}
		err = base_proc_wait(pid, &exit);
		if (exit != 0 || err != 0) {
			error("exec failed");
		}
		free(args);
	}
}

void buildTarget(map<string, Target> &targets, string target, map<string, bool> &alreadyBuilt, map<string, bool> &building) {
	if (targets.count(target) == 0)
	{ error("target does not exist " + target); }

	if (alreadyBuilt.count(target) != 0) {
		return;
	}

	if (building.count("target") != 0)
	{ error("circular dependency on " + target); }
	building[target] = true;

	Target &t = targets[target];

	building[target] = true;

	for (auto dep : t.targets) {
		buildTarget(targets, dep, alreadyBuilt, building);
	}

	std::cout << "exec: " << target << "\n";
	execTarget(t);
	alreadyBuilt[target] = true;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		error("    buildx <target>\n    buildx --sh mkdir <file>");
	}
	if (strcmp(argv[1], "--sh") == 0) {
		if (argc < 3) {
			error("    buildx --sh cmd");
		}
		if (strcmp(argv[2], "mkdir") == 0) {
			if (argc != 4) {
				error("    buildx --sh mkdir file");
			}
			try {
				// Create the directory along with all its parent directories
				std::filesystem::create_directories(argv[3]);
			} catch (const std::filesystem::filesystem_error& e) {
				std::cerr << "Failed to create directory: " << e.what() << std::endl;
			}
		} else {
			error("unknown command");
		}
		return 0;
	}
	std::ifstream file("xbuild.json");
	//std::ifstream file(argc == 1 ? "xbuild.json" : argv[1]);

	if (!file.is_open()) {
		std::cerr << "Unable to open file." << std::endl;
		return -1;
	}

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();

	json::JSON obj = json::JSON::Load( content );

	Loop loop;
	json::JSON env = json::JSON({});
	json::JSON target = json::JSON({});

	for (auto e : obj.ObjectRange()) {
		if (e.first == "env") {
			env = e.second;
		}
		if (e.first == "loop") {
			if (e.second.JSONType() != json::JSON::Class::Object)
			{ error("loop has to be Object"); }
			for (auto l : e.second.ObjectRange()) {
				if (l.second.JSONType() != json::JSON::Class::Array)
				{ error("loop value has to be Array"); }
				{
					vector<string> loopValue;
					for (auto lv : l.second.ArrayRange()) {
						if (lv.JSONType() != json::JSON::Class::String)
						{ error("loop value entry has to be String"); }
						loopValue.push_back(lv.ToString());
					}
					loop[l.first] = loopValue;
				}
			}
		}
		if (e.first == "target") {
			target = e.second;
		}
	}

	loop["DISTX_SRC"] = vector<string>({ getcwd(0, 4096) });
	loop["DISTX_DEP"] = vector<string>({ string(getcwd(0, 4096)) + "/_distx" });
	loop["DISTX_INSTALL"] = vector<string>({ string(getcwd(0, 4096)) + "/_distx" });
	loop["DISTX_BUILD"] = vector<string>({ string(getcwd(0, 4096)) + "/build" });
	loop["DISTX_PREFIX"] = vector<string>({ "distx.org_2024-" });
	loop["DISTX_ARCH"] = vector<string>({
#ifdef __x86_64__
			"x86_64"
#else
			"aarch64"
#endif
			});

	map<string, string> varmap;
	json::JSON transformed = replace(varmap, loop, target);
	std::cout << transformed << "\n";

	map<string, Target> targets;

	if (transformed.JSONType() != json::JSON::Class::Object)
	{ error("trarget needs to be object"); }
	for (auto target : transformed.ObjectRange()) {
		if (target.second.JSONType() != json::JSON::Class::Object)
		{ error("target value needs to be object"); }

		Target t;
		for (auto entry : target.second.ObjectRange())
		{
			if (entry.first == "targets") {
				if (entry.second.JSONType() != json::JSON::Class::Array)
				{ error("targets needs to be an array"); }
				for (auto entry : entry.second.ArrayRange()) {
					if (entry.JSONType() != json::JSON::Class::String)
					{ error("targets entry needs to be a string"); }
					t.targets.push_back(entry.ToString());
				}
			}
			if (entry.first == "make-deps") {
				if (entry.second.JSONType() != json::JSON::Class::String)
				{ error("make-deps needs to be a string"); }
				t.makeDeps = entry.second.ToString();
			}
			if (entry.first == "inputs") {
				if (entry.second.JSONType() != json::JSON::Class::Array)
				{ error("inputs needs to be an array"); }
				for (auto entry : entry.second.ArrayRange()) {
					if (entry.JSONType() != json::JSON::Class::String)
					{ error("inputs entry needs to be a string"); }
					t.dep.inputs.push_back(entry.ToString());
				}
			}
			if (entry.first == "outputs") {
				if (entry.second.JSONType() != json::JSON::Class::Array)
				{ error("outputs needs to be an array"); }
				for (auto entry : entry.second.ArrayRange()) {
					if (entry.JSONType() != json::JSON::Class::String)
					{ error("outputs entry needs to be a string"); }
					t.dep.outputs.push_back(entry.ToString());
				}
			}
			if (entry.first == "exec") {
				if (entry.second.JSONType() != json::JSON::Class::Array)
				{ error("exec needs to be an array"); }
				for (auto entry : entry.second.ArrayRange()) {
					if (entry.JSONType() != json::JSON::Class::Array)
					{ error("exec needs to be an array"); }

					t.exec.push_back({});
					vector<string> &exec = t.exec.at(t.exec.size() - 1);
					for (auto entry2 : entry.ArrayRange()) {
						if (entry2.JSONType() != json::JSON::Class::String)
						{ error("exec entry needs to be a string"); }
						exec.push_back(entry2.ToString());
					}
				}
			}
		}
		targets[target.first] = t;
	}

	map<string,bool> alreadyBuild;
	map<string,bool> building;
	buildTarget(targets, argv[1], alreadyBuild, building);
}
