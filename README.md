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
> apt install libjack-dev
> ./waf configure
# configuration script should find jack, or no-go
> ./waf build
```

Test the script

> export LD_LIBRARY_PATH=/audiocare-src/aubio/build/examples
> /audiocare-src/aubio/build/examples/audiocare

**Should output zeroes for all values without jackd running**

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