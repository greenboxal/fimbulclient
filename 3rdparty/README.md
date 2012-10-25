Dependencies
============

Current user suplied dependencies:
* Boost
* V8 Engine

Boost
-----
Boost is a very large library, instead of copying Boost files you need to set the BOOSTDIR environment variable to the root folder containing Boost lib and include directories.
You can download boost from [BoostPro Computing](http://www.boostpro.com/download/).

V8 Engine
---------
You need to build V8 engine in both Debug and Release and copy output libraries and include files to 3rdparty/V8 following the next diagram:

- 3rdparty\
  - V8\
    - Debug\
	  - v8_base.lib
	- Release
      - v8_base.lib
    - Include
      - v8.h
      - v8-debug.h
      - v8-preparser.h
      - v8-profiler.h
      - v8stdint.h
      - v8-testing.h
