'use strict';

describe('Leaderboard v1', function() {
    before(function(done) {
        return resetDatabases(done);
    });

    it('is initially empty', function(done) {
        return agent
            .get('/leaderboard/v1/')
            .expect(200)
            .expect([])
            .end(done);
    });

    it('adds a score', function(done) {
        return agent
            .post('/leaderboard/v1/')
            .send({
                userName: 'alice',
                score: 5.2,
            })
            .expect(201)
            .end(done);
    });

    it('has the score', function(done) {
        return agent
            .get('/leaderboard/v1/')
            .expect(200)
            .expect([
                {
                    userName: 'alice',
                    score: 5.2,
                },
            ])
            .end(done);
    });

    it('adds another score', function(done) {
        return agent
            .post('/leaderboard/v1/')
            .send({
                userName: 'bob',
                score: 7.5,
            })
            .expect(201)
            .end(done);
    });

    it('has both scores', function(done) {
        return agent
            .get('/leaderboard/v1/')
            .expect(200)
            .expect([
                {
                    userName: 'bob',
                    score: 7.5,
                },
                {
                    userName: 'alice',
                    score: 5.2,
                },
            ])
            .end(done);
    });

    it('updates a score', function(done) {
        return agent
            .post('/leaderboard/v1/')
            .send({
                userName: 'alice',
                score: 9.4,
            })
            .expect(201)
            .end(done);
    });

    it('keeps the scores sorted', function(done) {
        return agent
            .get('/leaderboard/v1/')
            .expect(200)
            .expect([
                {
                    userName: 'alice',
                    score: 9.4,
                },
                {
                    userName: 'bob',
                    score: 7.5,
                },
            ])
            .end(done);
    });

    it('does not update a score lower than the highscore', function(done) {
        return agent
            .post('/leaderboard/v1/')
            .send({
                userName: 'bob',
                score: 2.9,
            })
            .expect(200)
            .end(done);
    });

    it('has not updated the lower score', function(done) {
        return agent
            .get('/leaderboard/v1/')
            .expect(200)
            .expect([
                {
                    userName: 'alice',
                    score: 9.4,
                },
                {
                    userName: 'bob',
                    score: 7.5,
                },
            ])
            .end(done);
    });
});
