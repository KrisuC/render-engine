# render-engine

A simple renderer.

## how to build

To build and run on *nix systems (macOS / Linux / FreeBSD):

```
mkdir cmake-build
cd cmake-build
cmake ..
make -j16
./engine
```

To build on windows, see:

[build for visual studio on windows](./doc/How-to-build-under-M$-Windows.md)

If you success, you will see something like these:

![screenshot1](doc/screenshot1.png)

![screenshot3](doc/screenshot3.png)

![screenshot2](doc/screenshot2.png)


## TODO list:

- Scene graph
- A message queue
- Fix Texture to support PBR pipeline
- Fix Mesh and Model
- ...
- More BxDF, more lighting models, yes, yes!
