# include "Interface.h"

int serialize_bloom(bloom filter, char ** CountryName, void ** output);

bloom unserialize_bloom(void * input, char ** CountryName, int buffer, int *size);

bloom receive_bloom(int id, char ** CountryName, int buffer);

int send_bloom(int monitorId, int buffer, bloom filter, char * CountryName);

int BytestoRead(int size, int times, int buffer);