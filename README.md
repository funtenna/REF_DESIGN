# Funtenna

## Steps/setup

1. Hook up a wire/antenna to pin 8.46 on the Beaglebone (consult the
[reference manual](http://beagleboard.org/static/beaglebone/latest/Docs/Hardware/BONE_SRM.pdf).

2. Copy `beaglebone/gpio` to BeagleBone; compile the kernel module.
[This](http://nomel.tumblr.com/post/30357133735/beaglebone-tutorial-how-to-compile-kernel-modules-and)
is a good resource for installing the dependencies necessary for compiling a
kernel module and compiling. Once that's done, load the kernel module and begin
transmitting.

    make
    insmod gpio.ko
    mknod /dev/gpio c 60 0

Now, the `repeat` executable will broadcast a `10101010` bit pattern over and
over. This helps the AGC in the Gnuradio circuit adjust to the appropriate
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
the flow. Wait for the levels in the scope to settle down to a peak of about 2.0.

6. Tail `/tmp/bitpattern`.

7. On the BeagleBone, ^C the `repeat` executable and run `./send_message 'test
message here'`. If all is well, the message should appear in `/tmp/bitpattern`.
