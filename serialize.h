# include "Interface.h"
#define INITIAL_SIZE 32


typedef struct Buffer{
    void * Data;
    int Next;
    size_t Size;
}Buffer;

Buffer *new_buffer();

void bufferDestroy(Buffer *b);

void reserve_space(Buffer *b, size_t bytes);

void serialize_int(int x, Buffer *b);

void serialize_string(char * x, Buffer *b);
