/* globals eio */
'use strict';

var socket;

function connect() {
	socket = new eio.Socket('ws://localhost:3000/');
	
	socket.on('open', function() {
		console.log('Connected');
	});

	socket.on('message', function(data) {
		console.log('Message', data);
	});

	socket.on('error', function(err) {
		console.log('Error', err);
	});

	socket.on('close', function() {
		console.log('Disconnected');
		return connect();
	});
}

connect();
