Benchmarking parallel DHT storage techniques
====
---
| Abha Chaudhary  |  Chenfei Lin |  Adil Ansari |
----

####Objective

Milestone was to get a search a few libraries in MPI or OpenMP, understand the functioning, set them up and if possible, get a working model. Following are the APIs:

> C-MPI-CODE

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
