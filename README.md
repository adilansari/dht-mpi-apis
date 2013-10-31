##C-MPI-CODE
1. URL: [c-mpi](http://c-mpi.sourceforge.net) 
2. **ls** into c-mpi-code directory
3. ./setup.sh
4. ./configure --config-cache --enable-table-dense-1 --enable-tests --with-mpi=/usr
	+ In case of error with lcrypto, install libssl-dev
5. make D=1 test_results
	+ in case of zsh error, install zsh
