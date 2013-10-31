
#include <stdio.h>

int stream_to_buffer(char* buffer, int length, FILE* stream);
void buffer_to_stream(char* buffer, int length, FILE* stream);
int file_to_buffer(char** buffer, char* name);
void eat_whitespace(FILE* stream);
