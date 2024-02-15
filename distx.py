#!/usr/bin/env python3

import sys
import json
import os
import urllib.request
import tarfile
import subprocess
import platform

def extract_tarfile(src, dest):
    with tarfile.open(src, 'r') as tar:
        tar.extractall(path=dest)

def parse_json_file(file_path):
    with open(file_path, 'r') as file:
        data = json.load(file)
        return data

def expand_any(json, loops):
    if (type(json) == str):
        #print("expand str", json)
        singles = {}
        for (var, values) in loops.items():
            if len(values) == 1:
                singles[var] = values[0]
        return json.format(**singles)
    if (type(json) == dict):
        #print("expand dict", json)
        return expand_table(json, loops)
    if (type(json) == list):
        #print("expand list", json)
        return expand_list(json, loops)

def expand_table(table, loops):
    ret = {}
    for key, value in table.items():
        dicts = [{}]
        for (var, values) in loops.items():
            repl_str = "{" + var + "}"
            if (key.find(repl_str) == -1):
                continue
            dicts = [{**d, var: [v]} for d in dicts for v in values]
        for dic in dicts:
            ret[key.format(**dic)] = expand_any(value, { **loops, **dic })
    return ret

def expand_list(expand_list, loops):
    ret = []
    for entry in expand_list:
        dicts = [{}]
        for (var, values) in loops.items():
            repl_str = "{" + var + "}"
            if (entry.find(repl_str) == -1):
                continue
            dicts = [{**d, var: v} for d in dicts for v in values]
        for dic in dicts:
            ret.append(entry.format(**dic))
    return ret

def download_files(expanded, loop):
    
    # Create _distx directory if it doesn't exist
    distx_dir = loop["DISTX_DEP"][0]
    if not os.path.exists(distx_dir):
        os.makedirs(distx_dir)

    try:
        for key in expanded:
            # Extract the part before the plus sign
            repo_name = key.split('_')[0]
            
            # Download the file containing URLs based on extracted repo name
            url = f"https://{repo_name}/.well-known/distx.json"
            print("get wellknown", url)
            with urllib.request.urlopen(url) as response:
                print("got wellknown", url)
                urls_json = json.loads(response.read().decode())
                urls = urls_json.get('urls', [])
                
                file_url = f"https://{urls[0]}/{key}.tar.xz"
                distx_filename = os.path.join(distx_dir, f"{key}.tar.xz")

                print(f"Downloading file: {file_url} to {distx_filename}")

                urllib.request.urlretrieve(file_url, distx_filename)
                extract_tarfile(distx_filename, distx_dir)
    except urllib.error.URLError:
        print(f"Failed to download URLs from the server for {repo_name}")

def run_cmd(cmd, loops):
    print("running", cmd)
    system = "windows.exe"
    if platform.system() == "Linux":
        system = "linux"
    loader = loops["DISTX_DEP"][0] + "/" + loops["DISTX_PREFIX"][0] + "xload-" + loops["DISTX_ARCH"][0] + "/bin/load_elf_" + system
    print("loader", loader)
    subprocess.run([ loader ] + cmd["cmd"])


def main():
    args = sys.argv[1:]
    
    if len(args) == 0:
        print("Usage: <program> install | install-dev | install <name> | install-dev <name>")
        return
    
    # Load JSON configuration
    json_config = parse_json_file("distx-build.json")
    loop = json_config.get('loop', {})
    loop["DISTX_HOST"] = ["x86_64"]
    loop["DISTX_DEP"] = [os.getcwd() + "/_distx"]
    loop["DISTX_SRC"] = [os.getcwd()]
    loop["DISTX_DEST"] = [os.getcwd() + "/_distx"]
    loop["DISTX_BUILD"] = [os.getcwd() + "/build"]
    loop["DISTX_PREFIX"] = ["distx.org_2024-"]
    loop["DISTX_ARCH"] = [platform.machine()]
    for key, value in json_config.get('env', {}).items():
        loop[key] = [value]

    expanded_dep = expand_list(json_config.get('dep', {}), loop)
    expanded_cmd = expand_any(json_config.get('run', {}), loop)
    print("loop", loop)
    print("dep", expanded_dep)
    print("run", expanded_cmd)

    if args[0] == "asdf":
        return
    elif args[0] == "i" or args[0] == "install":
        download_files(expanded_dep, loop)
    elif args[0] == "r" or args[0] == "run":
        run_cmd(expanded_cmd[args[1]], loop)
    else:
        print("Invalid arguments. Usage: <program> install | install-dev | install <name> | install-dev <name>")

if __name__ == "__main__":
    main()

