# include "serialize.h"

struct Buffer *new_buffer() {
    struct Buffer *b = malloc(sizeof(Buffer));

    b->Data = malloc(INITIAL_SIZE);
    b->Size = INITIAL_SIZE;
    b->Next = 0;

    return b;
}

void bufferDestroy(Buffer *b)
{
    free(b->Data);
    free(b);
}

void reserve_space(Buffer *b, size_t bytes)
{
    if((b->Next + bytes) > b->Size) {
        /* double size to enforce O(lg N) reallocs */
        b->Data = realloc(b->Data, b->Size * 2);
        b->Size *= 2;
    }
}

void serialize_int(int x, Buffer *b) 
{
    x = htonl(x);

    reserve_space(b, sizeof(int));

    memcpy( ( (char *)b->Data ) + b->Next, &x, sizeof(int));
    b->Next += sizeof(int);
}

void serialize_string(char * x, Buffer *b)
{
    
}