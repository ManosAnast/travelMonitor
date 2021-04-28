# include "Interface.h"

int serialize_bloom(bloom filter, void ** output);

bloom unserialize_bloom(void * input);