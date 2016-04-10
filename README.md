# E1.31 (sACN) to AdaLight Bridge Server

## Compiling

To compile the bridging server, simply use the ```make``` utility:

```shell
$ make

# clean
$ make clean
```

## Running

To run the program, you must supply three options:

```
Usage: e131-adalight-bridge [-d device] [-n num_leds] [-u universe]

Options:
  -d device      serial device to use to communicate with AdaLight
  -d num_leds    number of RGB LEDs connected to AdaLight
  -u universe    DMX universe number to respond to

All options are mandatory.
```

**Note:** because each DMX universe has a maximum of 512 channels and each RGB LED takes 3 channels (one per color), the maximum number of controllable AdaLight LEDs is **170**.

Example invocation for 100 LEDs AdaLight attached to /dev/ttyACM0:

```shell
$ ./e131-adalight-bridge -d /dev/ttyACM0 -u 1 -n 100
```
