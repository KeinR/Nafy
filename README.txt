So the build process is a little involved...

First and foremost, you need a C/C++ compiler: The GNU GCC compiler (g++) is what I use,
and if you don't want any problems, use that.
Linux distros likely comes packaged with it, however Mac and Windows are different and
will (often) require you to install it yourself.

Next, the libraries. I included what libraries I could here, but some are too big.
GLFW -> https://www.glfw.org/
OpenAL Soft -> https://openal-soft.org/
Freetype -> https://www.freetype.org/download.html
The DLLs/Shared objects must be on the system PATH or in the same directory as the exacutable.
Library files (libglfw3dll.a, libfreetype.dll.a, ...) must be on the linker path. Add a directory
to the linker search path by adding -L<path> to the `CFLAGS` variable in `Makefile`
You _may_ have to change "`pkg-config --cflags freetype2`" to the "include" dir of your freetype download with -I<dir> to `CFLAGS`
You will have to add the "include" dir of your GLFW download with -I<dir> to `CFLAGS`.
You will have to add the "include" dir of your OpenAL-Soft download with -I<dir> to `CFLAGS`.
If they don't have precompiled binaries, you must build those with GCC. This will likely not be the case for Windows.

Next, make sure you have GNU make installed.

Finally, you're ready to build the thing. Open terminal in the same dir as `Makefile` and type in
"make release". If you installed GNU make, it should start compiling the object files to ./bin/*,
and at the end linking them all and creating ./nafy.exe 

You're done! Run nafy.exe to see the short demo of the framework so far.
