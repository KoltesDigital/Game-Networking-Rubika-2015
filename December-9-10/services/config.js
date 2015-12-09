'use strict';

// terrible hack to support tests
if (require.main.filename.lastIndexOf('mocha') !== -1)
    process.env.NODE_ENV = (process.env.NODE_ENV || 'development') + '-test';

module.exports = require('config-path')();
