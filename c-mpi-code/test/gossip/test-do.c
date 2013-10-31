
/**
 * Test gossip_do() functionality.
 * */

#include <mpi.h>
#include <gossip.h>
#include <mpi_tools.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    whoami();

    printf(
"This test program will print an example debugging message on the final\n"
"line of output if the CMPI_DEBUG_MASK environment variable is set to include\n"
"\"example\" or \"all\" in its comma separated list of masks:\n"
    );

    gossip_debug_init();

    gossip_do(MASK_EXAMPLE, printf("EXAMPLE MESSAGE\n"));

    MPI_Finalize();
    return 0;
}

/*
 * Local variables:
 *  c-indent-level: 2
 *  c-basic-offset: 2
 * End:
 *
 * vim: ts=8 sts=4 sw=4 expandtab
 */
