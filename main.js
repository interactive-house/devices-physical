const { SerialPort } = require("serialport");
const readline = require("readline");
var prompt = require("prompt-sync")();
const port = new SerialPort({
	path: "COM5",
	baudRate: 9600,
	autoOpen: false,
});
try {
	port.open();
} catch (error) {
	console.log(error);
}

//const rl = readline.createInterface({
//	input: process.stdin,
//	output: process.stdout,
//});

//rl.question("input: ", function (answer) {
//	port.write(Buffer.from(answer));
//	console.log(typeof answer);
//	rl.close();
//});

var n = prompt("How many more times? ");
console.log(typeof n);
port.write(Buffer.from(n));
