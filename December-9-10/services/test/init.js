'use strict';

var appLoader = require('..');
var request = require('supertest');

before(function(done) {
    return appLoader(function(err, app) {
        if (err) return done(err);

        global.agent = request.agent(app);

        var redis = app.get('redis');
        global.resetDatabases = function(callback) {
            return redis.flushdb(callback);
        };

        return done();
    });
});
