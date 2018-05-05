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
// let quit     = global.quit     = Liker.quit;


// var settings = {};
// var db, dba;
// global return quit
// var _res, _req;

// Liker.init();
// Liker.start();
const http = require('http')
const port = 3000
const requestHandler = (request, response) => {
  console.log(request.url)
  response.end('Hello Node.js Server!')
}
const server = http.createServer(requestHandler)
server.listen(port, (err) => {
  if (err) {
    return console.log('something bad happened', err)
  }
  console.log(`server is listening on ${port}`)
})

process.on('exit', function (code) {
  warn('Process exit code '+code);
  // Add shutdown logic here.
});