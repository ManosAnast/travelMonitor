# include "serialize.h"

int serialize_bloom(bloom filter, void ** output)
{
    int Bytes=filter.NumofBytes;
    int size=Bytes+2*sizeof(int);
    
    memcpy(*output, &Bytes, sizeof(int));

    char * Arr=filter.bits;
    memcpy(*output+sizeof(int), filter.bits, Bytes);

    return size;
}

bloom unserialize_bloom(void * input)
{
    bloom filter;
    memcpy(&(filter.NumofBytes), input, sizeof(int));

    filter.bits=(char *)calloc(filter.NumofBytes, sizeof(char));
    memcpy(filter.bits, input+sizeof(int), filter.NumofBytes);

    return filter;
}