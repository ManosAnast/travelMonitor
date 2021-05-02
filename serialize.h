# include "Interface.h"

int serialize_bloom(bloom filter, char ** VirusName, int count, void ** output);

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int buffer, int *size);

int receive_bloom(int id, Virus * Vlist, int buffer);

int send_bloom(int monitorId, int buffer, Virus * Vlist);

int BytestoRead(int size, int times, int buffer);