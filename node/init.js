/**
 * CONSTANTS
 */
const ROOT = global.ROOT = __dirname;


/**
 * Node modules
 */
let fs        = global.fs          = require('fs');
let path      = global.path        = require('path');
// let _         = global._           = require('lodash');
// let cheerio   = global.cheerio     = require('cheerio');
// var logger = require('morgan');
// var md5 = require('MD5');
// let http = require('http');
// let moment    = global.moment      = require('moment');
// let request   = global.request     = require('request');
let readLineSync = global.readLineSync = require('readline-sync');

let utils    = require( `${ROOT}/lib/utils` );
let config = global.config = {};

// Libs
let AubioCare    = global.AubioCare    = require( `${ROOT}/lib/AubioCare` );
// let quit     = global.quit     = Liker.quit;


// var settings = {};
// var db, dba;
// global return quit
// var _res, _req;

// Liker.init();
// Liker.start();



config = global.config = loadConfig()
confirmConfig(config)
info('Running the script...')
AubioCare.start()


process.on('exit', function (code) {
  warn('Process exit code '+code);
  // Add shutdown logic here.
});