# Funtenna: Reference Implementation

This repository contains the code for the reference funtenna implementation.

It transmits Hamming-coded data from a BeagleBone's GPIO pin, bit-banged at
about 12.5 MHz. This data is received on a USRP2, then demodulated in GNU Radio.

## Materials

* BeagleBone (Rev2) running Ångström Linux
* Antenna: we used a long wire wrapped in an oval around a small wooden cross.
* Ettus research USRP2 (this is discontinued, but any GNU Radio-compatible SDR
  board that can receive at 12.5 MHz will do)
* Computer with GNU Radio >=3.7

## Steps/setup

1. Hook up a wire/antenna to pin 8.46 on the Beaglebone (consult the
[reference manual](http://beagleboard.org/static/beaglebone/latest/Docs/Hardware/BONE_SRM.pdf)).

2. Copy `beaglebone/gpio` to BeagleBone; compile the kernel module.
[This](http://nomel.tumblr.com/post/30357133735/beaglebone-tutorial-how-to-compile-kernel-modules-and)
is a good resource for installing the dependencies necessary for compiling a
kernel module and compiling. Once that's done, load the kernel module and begin
transmitting.

    make
    insmod gpio.ko
    mknod /dev/gpio c 60 0

Now, the `repeat` executable will broadcast a `10101010` bit pattern over and
over. This helps the AGC in the GNU Radio circuit adjust to the appropriate
level.

3. Power on and connect USRP.

4. Compile and install GNU Radio blocks found in `gr-cw_binary` and
`gr-hamming` directories (if this fails at any step, it's probably due to
missing dependencies):

    cd gr-cw_binary
    mkdir build
    cd build
    # NB: the install prefix option is important to make sure that GRC can find
    # your blocks; look for /usr/share/gnuradio/grc (it might be in
    # /usr/local/share/gnuradio/grc)
    cmake -D CMAKE_INSTALL_PREFIX=/usr ../
    make
    make test
    sudo make install # if all looks good
    sudo ldconfig

5. Start GNURadio companion. Open flow found in `demod/online-demod.grc`. Run
the flow. Wait for the levels in the scope to settle.

6. Tail `/tmp/bitpattern`.

7. On the BeagleBone, ^C the `repeat` executable and run `./send_message 'test
message here'`. If all is well, the message should appear in `/tmp/bitpattern`.

## Implementation Details

### BeagleBone

The BeagleBone code is a kernel module that writes either alternating `10`s or
`00`s to memory-mapped IO for the BeagleBone's GPIO pin 8.46. The number of
repetitions can be set. The kernel module reads from a `char` array, toggling
the pin for a nonzero value or holding the pin low for a zero. This broadcasts
the given bit-pattern using
[on-off keying](https://en.wikipedia.org/wiki/On-off_keying).

There are two userspace programs to broadcast data. One sends out a `10101010`
bit pattern on repeat until interrupted; this allows the receiving end to adjust
their levels. The other program encodes data it is given via the command line
into packets:

* A fixed 6-byte prefix: `0xaaaaaaaacccc`
* One unsigned byte to indicate the length of the message
* Length * 2 bytes that include the data sent, Hamming[7,4]-coded for error
  correction

The BeagleBone broadcasts at 12.5 MHz.

### USRP

We used an Ettus Research USRP2 (discontinued) with a homemade antenna. We were
able to observe a clear signal from at least 30 feet away before signal
processing.

### GNU Radio

We used GNU Radio for signal processing and demodulation; the flow graph
includes the following:

* USRP Source centered at 12.49 MHz
* Band Pass Filter to isolate the BeagleBone signal
* Automatic Gain Control to adjust to the level of the received signal
* Moving Average to smooth the signal
* Threshold to convert the signal into binary responses
* Sampling logic (our blocks) to turn the binary samples into a stream of bits
* Packet logic (our blocks) to recognize packets
* Hamming decoder (our blocks) perform error correction and detection
* Digital logic to combine nibbles into bytes and write them to a file
