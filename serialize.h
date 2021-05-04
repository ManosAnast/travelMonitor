# include "Country.h"

int serialize_bloom(bloom filter, char ** VirusName, int count, void ** output);

bloom unserialize_bloom(void * input, char ** VirusName, int * count, int buffer, int *size);

int receivebloomtest(int id, Virus * Vlist, int buffer);

int receive_bloom(int id, Virus * Vlist, int buffer, void * input, int fd, int * count);

int send_bloom(int monitorId, int buffer, Virus * Vlist);

int BytestoRead(int size, int times, int buffer);

void * serialize_commands(char ** Array, int * Length);

void * serialize_commandsint(char ** Array, int * Length);

char ** unserialize_commands(void * input);