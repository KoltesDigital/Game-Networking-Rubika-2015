'use strict';

var engine = require('engine.io');
var http = require('http').createServer();
var server = engine.attach(http);

server.on('connection', function (socket) {
	console.log(socket.id, 'Connected');

	socket.on('message', function(data) {
		console.log(socket.id, 'Message', data);
	});

	socket.on('close', function() {
		console.log(socket.id, 'Disconnected');
	});
});

http.listen(3000, function() {
	console.log('Server listening on port 3000');
});
