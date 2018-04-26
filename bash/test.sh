#!/bin/bash
echo "Running Audiocare Main Script"
# getting vars $device_id, $server, $timer, $sample_length, $sample_size, $send_timer, $data
. config
echo "Running with following parameters"
echo "device_id     : $device_id"
echo "server        : $server"
echo "timer         : $timer"
echo "audio_duration: $audio_duration"
echo "sample_size   : $sample_size"
echo "send_timer    : $send_timer"
echo "data          : $data"

. bash/functions.sh


gettime 115s

echo $result

echo 'timer:' $timer


echo $OSTYPE



if [[ "$OSTYPE" == 'msys' ]]; then
    ./aubio/build/examples/aubiocare.exe ./hello.wav
else
    # ./aubio/build/examples/aubiocare
    ./aubio/build/examples/aubiocare -B 2048 -H 2048 -v -i ./hello.wav
    # ./aubio/build/examples/aubiocare -i ./hello.wav
fi

echo "Whoop! Done"