# include "Virus.h"

int serialize_bloom(bloom filter, char * VirusName, int count, void ** output);

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int *size);

int receivebloomtest(int id, Virus * Vlist);

int receive_bloom(int id, Virus * Vlist, void * input, int fd, int * count);

int send_bloom(int monitorId, Virus * Vlist);

int BytestoRead(int size, int times);

void * serialize_commands(char ** Array, int * Length);

char ** unserialize_commands(void * input);

void * serialize_citizen(Citizens * Rec, int * Length);
