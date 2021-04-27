# include "Interface.h"

int Level, BloomNum;

int main(int args, char * argv[])
{
    printf("I'm inside the monitor program %d\n", args);
    int monitorId=atoi(argv[1]);
    int buffer=atoi(argv[2]);
    BloomNum=atoi(argv[3]);
    char * Country=(char *)malloc(buffer*sizeof(char));
    Country=(char *)Fifo_read(monitorId, buffer);
    printf("monitor%d: %s\n", monitorId, Country);
    Start(Country);
    free(Country);
    return 0;
}