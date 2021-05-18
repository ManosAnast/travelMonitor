# include "MonitorInterface.h"

int monitorId, buffer, BloomNum;
volatile sig_atomic_t interrupt_flag_usr, interrupt_flag_iq, interrupt_flag_kill ;

int main(int args, char * argv[])
{
    int fd;

    monitorId=atoi(argv[1]); buffer=atoi(argv[2]); BloomNum=atoi(argv[3]);

    char * Country;
    Country=(char *)Fifo_read(monitorId, &fd);

    interrupt_flag_usr =0; interrupt_flag_iq=0; interrupt_flag_kill=0;

    Start(Country);
    free(Country);
    return 0;
}