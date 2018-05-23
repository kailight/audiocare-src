# Audiocare-Src

## C script for compilation

You would need to compile C script for any custom processor/OS

E.g ARMv6, ARMv7, etc

We use **/audiocare-src** dir in the root for the sake of simplicity

```bash
> sudo bash
> cd /
> git clone https://github.com/kailight/audiocare-src.git
> cd /audiocare-src
> chmod -R 777 .
> cd aubio
> ./scripts/get_waf.sh
# jackd source library is required for compilation, so we install
# first update apt (on ZeroW libjack-dev actually didn't install without update)
> apt-get update
# then
> apt install libjack-dev
> ./waf configure
# Configuration script should find jack, or no-go
# So one of the task output strings should read
# Checking for 'jack'                      : yes
# if this is true, build
> ./waf build
```

Test the script

```bash
# add libs location to path
export LD_LIBRARY_PATH=/audiocare-src/aubio/build/src
# run the script
/audiocare-src/aubio/build/examples/audiocare
```
**Should output zeroes for all values without jackd running**

### Testing with jackd

In order to test script with jackd we need 2 SSH sessions

If using putty, open session, then right-click on window title and select "Duplicate Session"

In terminal 1 run 
    
    export LD_LIBRARY_PATH=/audiocare-src/aubio/build/src
    /audiocare-src/aubio/build/examples/audiocare

AFTER that in terminal 2 run

    /usr/bin/jack_connect system:capture_1 aubio:in_1
    
Copy compiled file and library into */audiocare/c* folder manually
It is fine to overwrite

(not tested)

```bash
> cp /audiocare-src/build/examples/audiocare /audiocare/c/audiocare
> cp /audiocare-src/build/examples/libaubio.so /audiocare/c/libaubio.so
> cp /audiocare-src/build/examples/libaubio.so.5 /audiocare/c/libaubio.so.5
```

Also please send these files to my email so I could add them into package

*Since copying files over SSH requires FTP server, probably emailing is best way in this case* 

(not tested)

```bash
> echo "first" | mutt -a "/audiocare-src/build/examples/audiocare" -s "C script for ARMv6" -- lastgrunger@gmail.com
> echo "second" | mutt -a "/audiocare-src/build/examples/libaubio.so" -s "Library for ARMv6" -- lastgrunger@gmail.com
> echo "third" | mutt -a "/audiocare-src/build/examples/libaubio.so.5" -s "Library2 for ARMv6" -- lastgrunger@gmail.com
```