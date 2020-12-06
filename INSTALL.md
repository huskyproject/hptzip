
Please read the files (especially README.Makefiles) in the
husky-common (huskybse) package FIRST!

Instruction to build hptzip
---------------------------

1. Put the hptzip package in the directory where the other packages of fido
   husky reside:

  Unix, beos, possible cygwin:
  ----------------------------
```sh
	/usr/src/packages/   -> huskybse/
                             -> huskylib/
                             -> hptzip/
                             -> smapi/
                             -> fidoconfig/
                             -> hpt/
                             -> htick/
                             ...some other
```

   Windows, dos, os/2 & etc:
   -------------------------
```sh
	d:\husky\            -> huskylib\
                             -> hptzip\
                             -> smapi\
                             -> fidoconf\
                             -> hpt\
                             -> htick\
                             ...some other
```
2. Determine which makefile you need to use to compile (and install if necessary)
   hptzip using selected makefile, as in the following examples:

   Example 1 (use common makefile (named 'Makefile'): requires huskymak.cfg (see
              huskybse); unixes only):
```sh
	$ make
	$ make install
```
   Example 2 (use platfom-specific makefile):
```sh
   unix:
	$ make -f makefile.lnx
	$ make -f makefile.lnx install
   dos:
        d:\husky\hptzip\make>make -f makefile.mvc
```
2.1. Build using cmake:
```sh
  	$ mkdir build
  	$ cd build
  	$ cmake .. -DBUILD_SHARED_LIBS=OFF
```
  The last command will prepare for building a static library. If you want to build
  a dynamic library, then you have to run
```sh  
    	$ cmake ..
```
  Be shure to build all Husky projects the same way, either statically or
  dynamically.
```sh  
	$ cmake --build .
  	$ make install
```
  make distrib rpm,deb,tgz:
```sh
	cpack -G RPM 
	cpack -G DEB 
	cpack -G TGZ
```
3. (For UNIXes only) Ensure /usr/local/lib/ is in /etc/ld.so.conf

4. (For UNIXes only) Execute ldconfig as root

You are ready. Now you can install software which uses hptzip.
