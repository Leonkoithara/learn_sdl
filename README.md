# SDL2

This is a repository aimed to learn sdl2. This repository is following the
Lazy Foo Production tutorials for the Linux terminal environment although
the code has been changed slightly to suit myself.

## Installation steps

This works on Ubuntu (Debian system)

```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev
```

## Compiling and Running Programs

This project uses the GNU make for compilation.
```
make ARGS=<directory_name>
```
This will generate a binary file in the input directory with the same name.
To run the binary donot go into directory to run instead run it from project
root. E.g
```
./hello_world/hello_world
```
