
/**
 * Test gossip functionality.
 * */

#include <mpi.h>
#include <gossip.h>

#define MASK1  ((uint64_t)1 << 0)
#define MASK2  ((uint64_t)1 << 1)
#define MASK3  ((uint64_t)1 << 2)

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

/* examples of configuring gossip */
/*******************************************************************/

    /* control where the output goes, if desired */

    /* stderr by default, no need to enable */
    /* gossip_enable_stderr(); */
    /* to log to a particular file */
    /* gossip_enable_file("test.log", "w"); */
    /* to log to syslog */
    /* gossip_enable_syslog(LOG_USER); */

    /* control what timestamp format to use */

    /* usec granularity */
    /* gossip_set_logstamp(GOSSIP_LOGSTAMP_USEC); */
    /* turn off timestamp */
    /* gossip_set_logstamp(GOSSIP_LOGSTAMP_NONE); */
    /* use prettier, lower granularity for system services */
    /* gossip_set_logstamp(GOSSIP_LOGSTAMP_DATETIME); */
    /* like usec, but also shows thread number */
    /* gossip_set_logstamp(GOSSIP_LOGSTAMP_THREAD); */

    /* like usec, but also shows mpi rank, default for grayskull */
    /* gossip_set_logstamp(GOSSIP_LOGSTAMP_MPI); */

    /* control what mask levels get printed, one and two for this test */
    /* all masks off by default */
    gossip_set_debug_mask(1, MASK1|MASK2);


/* examples of internal code using gossip */
/*******************************************************************/

    gossip_err("Error message.\n");
    gossip_err("Error message, printf style: %d.\n", 12345);
    gossip_lerr("Error message, with line info.\n");

    /* examples of printing debugging messages */
    gossip_debug(MASK1, "Debug mask 1.\n");
    gossip_debug(MASK2, "Debug mask 2.\n");
    gossip_debug(MASK3, "Debug mask 3.\n");

    gossip_ldebug(MASK1, "Debug mask 1, with line info.\n");
    gossip_ldebug(MASK2, "Debug mask 2, with line info.\n");
    gossip_ldebug(MASK3, "Debug mask 3, with line info.\n");

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
