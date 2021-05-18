# include "Fifo.h"

// Make of a fifo name. This way, the name that the child and parent create, is the same.
bool make_fifo_name(int id, char * name, size_t name_max) 
{
    if(snprintf(name, name_max, "./fifo/Monitor%d", id)<0){
        return false;
    }
    return true;
}


int Fifo_init(int Num)
{
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return -1;
    }
    return mkfifo(fifo_name, 0666);
}

int Fifo_write(int Num, void * Input, int size, int * fd)
{
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return -1;
    }

    // Open fifo for write.
    *fd=open(fifo_name, O_WRONLY | O_SYNC);
    if(*fd<0 && errno == ENXIO){
        perror("open failed:");
        return -1;
    }

    // Write to fifo. If write return -1 error.
    if(write(*fd, Input, size)<0){
        perror("write failed");
        close(*fd);
        return -1;
    }
    return 0;
}

void * Fifo_read(int Num, int * fd)
{
    void * Input=calloc(buffer, sizeof(void));
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return NULL;
    }

    // Open fifo for read.
    *fd=open(fifo_name, O_RDONLY | O_SYNC);
    if(*fd<0){
        perror("open failed:");
        return NULL;
    }

    // Read from fifo. If read return -1 error.
    if(read(*fd, Input, buffer)<0){
        perror("read failed");
        close(*fd);
        return NULL;
    }
    return Input;
}

int Fifo_writeCommands(int Num, void * Input, int size, int * fd)
{
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", Num)<0){
        return -1;
    }

    // Open fifo for write.
    *fd=open(fifo_name, O_WRONLY);
    if(*fd<0 && errno == ENXIO){
        perror("open failed:");
        return -1;
    }

    // Write to fifo. If write return -1 error.
    if(write(*fd, (char *)Input, size)<0){
        perror("write failed");
        return -1;
    }
    return 0;
}

void * Fifo_readCommands(int Num, int * fd)
{
    void * Input=calloc(buffer, sizeof(void));
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", Num)<0){
        return NULL;
    }

    // Open fifo for read.
    *fd=open(fifo_name, O_RDONLY);
    if(*fd<0){
        perror("open failed:");
        return NULL;
    }

    // Read from fifo. If read return -1 error.
    if(read(*fd, Input, buffer)<0){
        perror("read failed");
        close(*fd);
        return NULL;
    }
    return Input;
}