#!/bin/bash
echo "Running Audiocare Main Script"

aubio\build\examples\audiocare.exe -B 2048 -H 2048 -d 111111111 -i hello.wav

echo "Whoop! Done"