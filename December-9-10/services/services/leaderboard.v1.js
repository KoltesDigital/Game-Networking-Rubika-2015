'use strict';

module.exports = function(app, router) {
    var redis = app.get('redis');

    // retrieve the leaderboard
    router.get('/', function(req, res, next) {
        // todo
    });

    // add an entry in the leaderboard
    router.post('/', function(req, res, next) {
        // todo
    });
};
