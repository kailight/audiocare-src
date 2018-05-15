const { exec } = require('child_process')


let AudioCare = {
  start: () => {
    info('AudioCare.start()')
    let config = global.config
    let cmd
    if (process.platform == 'win32') {
      //  ${process.cwd()}
      cmd = `aubio\\build\\examples\\AudioCare.exe hello.wav`
    } else {
      cmd = `./aubio/build/examples/AudioCare -i ./hello.wav > ./data.txt`
    }
    info(cmd)
    info(config)

    let loop = () => {
      info('loop()')
      exec(cmd, (error, stdout, stderr) => {
        info(error, stdout, stderr)
        console.log(stdout);
        console.log(`${stderr}`);
        if (error !== null) {
          quit(`AudioCare C Script exec error: ${error}`);
        }
      })
      info('-loop()')
    }
    loop()
    // If timer is set - repeat
    if (config.timer) {
      setInterval(loop,config.timer*1000)
    }
    // Liker.run();
  }
}



module.exports = AudioCare