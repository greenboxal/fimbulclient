FimbulwinterClientCPP
=====================

Dependencies
------------
See 3rdparty/README.md.

Building
--------
After resolving all dependencies, just open FimbulwinterClient.sln, select the build target and build the solution.

About Linux
-----------
Due to problems with compilers(missing features), by this time I won't provide real cross-platform build.

YA3DE and FimbulwinterClient uses mainly the __declspec(property) extension from MSVC that are not present in other compilers.

Maybe this extension will be included in the next C++ Technical Report, if included I'll make the needed changes to build it in Linux as well on other platforms(if possible). 

In this mean time, I'll make all possible to run perfectly over Wine/Cedega.
