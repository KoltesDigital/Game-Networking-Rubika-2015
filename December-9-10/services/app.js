'use strict';

var async = require('async');
var bodyParser = require('body-parser');
var config = require('./config');
var directoryContents = require('directory-contents');
var express = require('express');
var path = require('path');
var redisClient = require('./lib/redis-client');

module.exports = function(callback) {
    var app = express();

    app.use(bodyParser.json());

    // display requests and responses
    if (app.get('env') === 'development')
        app.use(function(req, res, next) {
            console.log('Request %s %s: %j', req.method, req.url, req.body);
            var write = res.write;
            var chunks = [];
            res.write = function(chunk, encoding, callback) {
                chunks.push(Buffer.isBuffer(chunk) ? chunk : new Buffer(chunk, encoding));
                return write.call(res, chunk, encoding, callback);
            };
            res.on('finish', function() {
                var data = Buffer.concat(chunks).toString();
                try {
                    data = JSON.parse(data);
                } catch (err) {}
                console.log('Response: %j', data);
            });
            return next();
        });

    return async.series([
        function(callback) {
            return redisClient(function(err, client) {
                if (err) return callback(err);

                app.set('redis', client);
                return callback();
            });
        },

        // automatically register services
        function(callback) {
            var serviceFilenameRegexp = /^(\w+)\.(v\d+)\.js$/;
            return directoryContents('services', {
                extensions: {
                    js: function(filename, callback) {
                        var basename = path.basename(filename);
                        var match = basename.match(serviceFilenameRegexp);
                        if (!match) return callback(new Error(basename + ' is not correctly formatted'));

                        var loader = require(filename);

                        var router = express.Router({
                            caseSensitive: true,
                            strict: true,
                        });
                        loader(app, router);

                        app.use('/' + match[1] + '/' + match[2], router);
                        return callback();
                    }
                }
            }, callback);
        },
    ], function(err) {
        if (err) return callback(err);
        return callback(null, app);
    });
};

// run the server if started in stand-alone
if (require.main === module)
    module.exports(function(err, app) {
        if (err) throw err;

        return app.listen(config.listen, function() {
            console.log('Listening on %s', config.listen);
        });
    });
