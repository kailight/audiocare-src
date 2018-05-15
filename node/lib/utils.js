const fs    = require('fs');
const path  = require('path');
const clc   = require('cli-color');
const readLineSync = require('readline-sync');

let utils = {};

module.exports = utils

let logHTML = function( filename, data ) {
  fs.writeFileSync( 'debug/'+filename+'.html', data );
};

let logJson = function( filename, data ) {
  fs.writeFileSync( 'debug/'+filename+'.js', 'let '+filename+' = '+JSON.stringify( data, null, '  ' ) + ';');
};

let quit = function (status, message_or_data, message) {
// info('quit()', status, typeof message_or_data, typeof message_or_data);

  if (typeof _req == 'undefined') {
    quit2(status, message_or_data);
  }

  // console.info(_req);
  // console.info(_res);
  let data;

  if (typeof message_or_data == 'object') {
    message = message;
    data = message_or_data;
  }
  else if (typeof message_or_data == 'string' && !message) {
    message = message_or_data;
    data = message_or_data;
  }
  else if (typeof message_or_data == 'number') {
    data = ''+message_or_data;
  }
  else if (message) {
    message = message;
    data = message_or_data;
  }
  else {
    message = 'Something happened';
    data = null;
  }

  // isAjax = _req.xhr;
  isAjax = true;

  // db.disconnect();

  // _res.header('Cache-Control', 'no-cache, private, no-store, must-revalidate, max-stale=0, post-check=0, pre-check=0');
  // _res.header('Cache-Control', 'no-cache');

  if (data) {
    // info(data);
  }

  if ( isAjax ) {
    if (message) _res.statusMessage = message;
    return _res.status(status).send( data );
  } else {
    return _res.status(status).send( pug.renderFile('error', data) ).end();
  }

};








/**
 * @param table string
 * @param collection array
 * @param callback function
 */
let update = function(table,collection,callback) {
// console.info('update()', table, collection.length);

  const len = collection.length;

  if ( !collection.length ) {
    info(`${len} ${table} updated`);
    console.warn('Out of items');
    return callback();
  }
  let item = collection.shift();
  if (!item.id) {
    return quit(402,'Item id is required');
  }

  db.where( { id : item.id } ).update( table, item, function (err) {

    if (err) {
      console.info(err);
      console.info(db._last_query());
      return quit( 402, "Could not save collection");
    }
    // info( table+' updated' );
    // console.info( db._last_query() );
    update( table, collection, callback );

  });

};


let rand = function(min, max) {
  min = parseInt(min);
  max = parseInt(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
};


let info = function() {
  if (isDev()) {
    if (arguments.length > 1) {
      for (let arg of arguments) {
        console.info(arg);
      }
    } else {
      console.info(arguments[0]);
    }
  }
};
let warn = console.warn;



let message = function() {
  for (let arg of arguments) {
    console.info(arg);
  }
};


let gettime = (str) => {
  let s = 0
  if (str.indexOf('s') > -1) {
    s = parseInt( str.replace('s','') )
  }
  if (str.indexOf('m') > -1) {
    s = parseInt( str.replace('m','') ) * 60
  }
  if (str.indexOf('h') > -1) {
    s = parseInt( str.replace('h','') ) * 60 * 60
  }
return s
}



/**
 * Loads config according to environment
 * @param filename
 */
let loadConfig = () => {
  let cfgraw = fs.readFileSync( './config' ).toString();
  cfgraw = cfgraw.split("\n")
  let cfg = {}
  let config = {}

  for (let str of cfgraw) {
    if ( str.charAt(0) != '#' ) {
      str = str.split('=');
      str.forEach( (s) => { s.trim() } )
      cfg[str[0]] = str[1]
    }
  }

  let allowed = 'mfcc/mean/centroid/slope/spread/skewness/kurtosis/decrease/rollof'.split('/')

  if (!cfg['device_id']) {
    quit(`No device_id in config, please run config`)
  }
  config.device_id = cfg.device_id
  info(`device_id: ${cfg.device_id}`)

  if (!cfg['server']) {
    quit(`No server in config, please run config`)
  }
  if (!cfg['server'].match(/[A-z0-9.]+:[0-9]+/)) {
    quit(`Bad server format, please run config`)
  }
  config.server = cfg.server
  info(`server: ${cfg.server}`)

  if ( config.timer = gettime( cfg['timer'] )  ) {
    config.hasTimer = true
  } else {
    config.hasTimer = false
  }
  info(`timer: ${cfg.timer}`)

  if (!cfg['audio_duration'] && config.hasTimer) {
    quit(`Missing audio_duration in config, please run config OR set timer to 0`)
  }
  config.audio_duration = gettime( cfg.audio_duration )
  info(`audio_duration: ${cfg.audio_duration}`)

  if (!cfg['sample_duration']) {
    quit(`Missing sample_duration in config, please run config`)
  }
  if ( config.sample_duration = gettime( cfg.sample_duration ) ) {
    config.sample_size = config.sample_duration * 16384
  }
  info(`sample_duration: ${cfg.sample_duration}`)

  if (!cfg['send_timer'] && cfg['send_timer'] != 0) {
    quit(`Missing send_timer in config, please run config`)
  }
  else {
    config.send_timer = gettime( cfg.send_timer )
  }
  info(`send_timer: ${cfg.send_timer}`)

  if (!cfg['data']) {
    quit(`Missing data in config, please run config`)
  } else {
    let data = cfg['data'].split(',')
    for (let d of data) {
      d = d.trim()
      if (allowed.indexOf(d) == -1) {
        q(`data param d is not allowed, allowed data params are ${allowed.join('/')}`)
      }
    }
    config.data = data
  }
  info(`data: ${cfg.data}`)

  return config
}


let confirmConfig = (config) => {

  let m = `\nThe script will be run `
  if (!config.hasTimer) {
    m += 'infinitely, '
  } else {
    m += 'every '+clc.yellow.bold(config.timer)+' seconds '
    m += 'for '+clc.yellow.bold(config.audio_duration)+' seconds, '
  }
  m += `with *${config.data.join(', ')}* taken every *${config.sample_duration}* seconds.\n`
  m += "The data will be "
  if (config.send_timer) {
    m += `stored in file *data.txt* AND sent to *${config.server}* every *${config.send_timer}* seconds`
  } else {
    m += `stored in file *data.txt* without sending.`
  }
  m = m.replace(/\*(.+?)\*/gm, clc.yellow.bold('$1') )
  info( m )

  info( "\nPress <ENTER> to confirm or any other key to exit" )
  let y = readLineSync.prompt()
  if (y.length) {
    quit('You have cancelled the script execution')
  }

}


let include = function( location ) {

  let content = fs.readFileSync( './'+location );
  return content;

};




let quit2 = function(message, status = 0) {

  console.info( clc.red(message) );
  process.exit();

};



let env = () => {
  return 'config'
  if (process.env.USERNAME == 'KaiLight') {
    return 'deathnote';
  } else {
    return 'g2';
  }
};



let isDev = () => {
  return true;
  // return env() == 'deathnote'
};



rootRequire = function(file) {
  return require( path.join ( ROOT , file ) );
};


let isArray = function(obj) {
  return !!obj && Array === obj.constructor;
};


const sleep = ms => new Promise(res => setTimeout(res, ms));


global.sleep        = sleep;
global.loadConfig   = loadConfig;
global.confirmConfig = confirmConfig;
global.rootRequire  = rootRequire;
global.quit         = quit;
global.message      = message;
global.info         = info;
global.isArray      = isArray;
global.warn         = warn;
global.logHTML      = logHTML;
global.logJson      = logJson;
global.env          = env();
global.isDev        = isDev();
global.rand         = rand;
global.update       = update;


module.exports = utils;