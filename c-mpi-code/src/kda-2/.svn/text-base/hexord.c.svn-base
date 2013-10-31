
/**
   HEX XOR DIFF 
*/

#include <assert.h> 
#include <stdio.h>
#include <stdlib.h> 

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define XOR(id1, id2) (((int) id1) ^ ((int) id2))

int
main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("usage: hexord <hex1> <hex2>\n");
    printf("result: integer representation of XOR(hex1,hex2)\n"); 
    exit(1); 
  }

  int x1,x2;
  sscanf(argv[1], "%X", &x1);
  sscanf(argv[2], "%X", &x2);
  printf("%i\n", XOR(x1,x2));

  return 0; 
}
