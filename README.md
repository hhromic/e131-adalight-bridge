# E1.31 (sACN) to AdaLight Bridge Server

## Compiling

To compile the bridging server, simply use the `make` utility:

```shell
$ make

# clean
$ make clean
```

You can install the binary file wherever you want.

## Running

To run the program, you must supply at least a serial device and baud rate to use:

```
Usage: e131-adalight-bridge [-h] [-u universe] -d device -b baud_rate
Forward E1.31 (sACN) network data to an AdaLight serial device.

Options:
  -h             show this help screen
  -u universe    DMX universe number for Multicast (default: 1)
  -d device      serial device for communicating with AdaLight
  -b baud_rate   baud rate for the serial device
```

**Note:** because each DMX universe has a maximum of 512 channels and each RGB LED takes 3 channels (one per color), the maximum number of controllable AdaLight LEDs is **170**.

Example invocation for AdaLight LEDs attached to `/dev/ttyACM0` at `115200` baud using DMX universe `2`:

```shell
$ ./e131-adalight-bridge -d /dev/ttyACM0 -b 115200 -u 2
```
