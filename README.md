# E1.31 (sACN) to AdaLight Bridge Server

## Compiling

To compile the bridging server, simply use the ```make``` utility:

```shell
$ make

# clean
$ make clean
```

You can install the binary file wherever you want.

## Running

To run the program, you must supply two options:

```
Usage: e131-adalight-bridge [-d device] [-u universe]

Options:
  -d device      serial device for communicating with AdaLight
  -u universe    DMX universe number to bridge to AdaLight

All options are mandatory.
```

**Note:** because each DMX universe has a maximum of 512 channels and each RGB LED takes 3 channels (one per color), the maximum number of controllable AdaLight LEDs is **170**.

Example invocation for AdaLight LEDs attached to ```/dev/ttyACM0``` using DMX universe ```1```:

```shell
$ ./e131-adalight-bridge -d /dev/ttyACM0 -u 1
```
