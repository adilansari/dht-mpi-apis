
#include <inlist.h>

int cmpi_mode_select(int rank, int size, int nodes);
struct inlist* cmpi_mode_contacts(int rank, int size, int nodes);
int cmpi_mode_next(int rank, int size, int nodes);
int cmpi_mode_first_client(int rank, int size, int nodes);
struct inlist* cmpi_shutdown_list(int rank, int size, int nodes);
