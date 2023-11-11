#include <iostream>
#include <fstream>
#include <string>
#include <base/proc.h>

#include "json.hpp"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "./testcpp_runtests <jsonfile>" << std::endl;
		return -1;
	}
	std::ifstream file(argv[1]);

	if (!file.is_open()) {
		std::cerr << "Unable to open file." << std::endl;
		return -1;
	}

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();

	json::JSON obj = json::JSON::Load( content );

	if (!obj.hasKey("tests")) {
		std::cerr << "No tests in file." << std::endl;
		return -1;
	}
	for (auto test : obj["tests"].ArrayRange()) {
		uintptr_t pid = 0;
		std::string path = test[0].ToString();
		const char * const args[] = { path.c_str(), "foo", 0 };
		std::cout << "run test: " << test << std::endl;
		int32_t error = base_proc_exec(args[0], args, { 0 }, &pid);
		uint8_t exit = 0;
		error = base_proc_wait(pid, &exit);
		if (exit != 0) {
			std::cerr << "test failed " << test[0] << std::endl;
			return -1;
		}
	}
	std::cout << "All tests passed" << std::endl;
}
