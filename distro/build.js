#!/run/current-system/sw/bin/node
const fs = require("fs");
const { argv, chdir } = require("process");
const { spawn } = require("child_process");

async function main() {
	const f = await fs.promises.readFile(argv[2]);
	const json = JSON.parse(f.toString())
	const start = argv.length == 4 ? parseInt(argv[3]) : 0;
	for (let i = start ; i < json.length ; i++) {
		console.log(`${i}. ${JSON.stringify(json[i])}\n`);
		const cmd = json[i];
		await new Promise((res) => {
			const child = spawn(cmd[1], cmd.slice(2), { cwd: cmd[0], stdio: "inherit" })
			child.on("close", (code) => {
				if (code === 0) {
					res()
				} else {
					console.error("exitcode " + code);
				}
			});
		});
	}
}

main()
