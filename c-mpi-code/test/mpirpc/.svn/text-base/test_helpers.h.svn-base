
#define TEST_DMALLOC DMALLOC_SETUP(if (mpi_rank == 0) \
                {                                     \
                  SHOW_S(dmalloc_logpath);            \
                  dmalloc_shutdown();                 \
                  /* free(dmalloc_logpath); */        \
                  dmalloc_log_unfreed();              \
                });
