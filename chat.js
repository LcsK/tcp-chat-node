const net = require('net');
const readline = require('readline');

const SERVER_IP = '127.0.0.1';
const SERVER_PORT = 9998;

const client = new net.Socket();
const rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout
});

let addressInput = true;
let closed = false;
let ip = '';
let port = '';

const server = net.createServer(socket => {
	console.log(`A new host has been connected: ${socket.remoteAddress}:${socket.remotePort}`);
	socket.on('data', data => {
        console.log(` msg: ${data}`);
		if(data === 'quit') {
			close();
		}
	});
	socket.on('close', data => {
		if(!addressInput) {
			close();
		}
	});
})
.listen(SERVER_PORT, SERVER_IP);

console.log("Please, type the Host's IP and Port, in this format: 127.0.0.1:9999 (IP:PORT)");

rl.on('line', input => {
	if(addressInput) {
		let address = input.split(':');
		ip = address[0];
		port = address[1];
		addressInput = false;
		connectClient();
	}
	else {
		if(input != 'quit') {
			client.write(input);
		}
		else {
			close();
		}
	}
});

const connectClient = () => {
	client.connect(port, ip, () => {
		console.log(`Connected to ${ip}:${port}`);
	})
}

const close = () => {
	client.end();
	server.close();
	process.exit();
}

client.on('close', () => {
	console.log('Connection closed');
});