
#include <stdio.h>

void* makedata(int n)
{
  char* result = malloc(n*sizeof(char));
  for (int i = 0; i < n; i++)
    result[i] = 'X';
  return (void*) result;
}

char* makekey(int i)
{
  char* result = malloc(16);
  sprintf(result, "%i", i);
  return result;
}
