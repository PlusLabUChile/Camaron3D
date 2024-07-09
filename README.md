# Camaron

3D Mesh visualization software, support for Linux and Windows (Mingw64/Msys2) environments

## Dependencies

* Qt5.15, including qmake and QTest
* g++
* make


## Compilation

To compile the project, generate the project Makefiles using qmake, then execute make as follows.

	$ qmake
	$ make

To enable ubsan static analysis, the following CONFIG parameter must be includedwhen using qmake:

	$ qmake "CONFIG += ubsan"

This option will generate binaries which are not compatible with the standard builds, thus is recommended to execute "make clean" before compiling with this option.

## Testing

To run tests, project must be compiled with the Test build, which will create its own executable
	
	$ qmake "CONFIG += test"
	$ make

By default the test executable will only run unit tests, to enable benchmarks include "-B" as a command line parameter. In the case of export tests its possible to mantain temporal data by using the "-k" option in the same manner

