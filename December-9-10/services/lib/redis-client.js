'use strict';

var config = require('../config');
var redis = require('redis');

module.exports = function(callback) {
    var client = redis.createClient(config.redis);

    client.on('error', function(err) {
        throw err;
    });

    return client.select(config.redis.select || 0, function(err) {
        if (err) return callback(err);
        return callback(null, client);
    });
};
