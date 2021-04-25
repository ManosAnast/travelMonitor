# include "Interface.h"

int Level, BloomNum;

int main()
{
    // Fifo_init(0);
    Virus * Vlist=Fifo_read(0);
    printf("I'm inside the monitor program\n");
    printf("%d\n", Vlist==NULL);
    return 0;
}