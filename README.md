Benchmarking parallel DHT storage techniques
====
---
| Abha Chaudhary  |  Chenfei Lin |  Adil Ansari |
----

####Objective

Milestone was to get a search a few libraries in MPI or OpenMP, understand the functioning, set them up and if possible, get a working model. Following are the APIs:

> c-mpi-code

> xmpp-mpi

> chimera

####C-MPI-CODE
1. URL: [c-mpi](http://c-mpi.sourceforge.net) 
2. **ls** into c-mpi-code directory
3. `./setup.sh`
4. `./configure --config-cache --enable-table-dense-1 --enable-tests --with-mpi=/usr`
    + In case of error with *lcrypto*, `sudo apt-get install libssl-dev`
5. `make tests`
	+ this will make the tests
6. `make D=1 test_results`
	+ in case of zsh error, `sudo apt-get install zsh`
7. `make clean` in case to re-run
8. To run specific tests
    + `make D=1 test/cmpi/test-startup.out`
        + test-startup is a basic startup and shutdown test
    + `make D=1 test/cmpi/test-tables.out`
        + test-tables is to debug tables
    + `make D=1 test/cmpi/test-putget.out`
        + test-putget is to test small put() and get()
    + _make clean_ each time before running test
    + _output_ for each test can be viewed in corresponding __*.out__ file

####XMPP-MPI
1. URL: [xmpp-mpi](http://apps.man.poznan.pl/trac/xmpp-mpi)
2. **ls** into xmpp-mpi directory
3. `./configure`
	+ If GnuTLS error, retry with `./configure --with-ssl=openssl`
	+ In case of openSSL error, retry with `./configure --with-ssl=no`
	+ In case of _gthread-2.0_ error, `sudo apt-get install libglib2.0-dev`
4. `make install`

####CHIMERA
1. URL: [chimera](http://current.cs.ucsb.edu/projects/chimera)
2. **ls** into chimera directory
3. `./configure --prefix='curent directory'`
4. `make`
5. **ls** into `/test`
6. `sha1_keygen [number of keys] [hostname]` will generate a set of pairs of ports and keys
7. `./test [port] [key]`, where post is the port and key is from pair generated above. You can then send messages.

####ZHT
1. URL: [ZHT](https://bitbucket.org/xiaobingo/iit.datasys.zht-mpi.git)
2. Download Google C++ protocol buffers :[here](https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.gz) and follow installation instructions from README
3. 
