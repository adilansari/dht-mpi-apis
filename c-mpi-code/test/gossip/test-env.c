
/**
 * Test gossip functionality.
 * */

#include <mpi.h>
#include <gossip.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    printf(
"This test program will print an example debugging message on the final\n"
"line of output if the CMPI_DEBUG_MASK environment variable is set to include\n"
"\"example\" or \"all\" in its comma separated list of masks:\n"
    );

    gossip_debug_init();

    gossip_debug(MASK_EXAMPLE, "EXAMPLE MESSAGE\n");

    MPI_Finalize();
    return 0;
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ts=8 sts=4 sw=4 expandtab
 */
