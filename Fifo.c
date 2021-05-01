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

int Fifo_write(int Num, void * Input, int size)
{
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return -1;
    }

    int fd=open(fifo_name, O_WRONLY | O_SYNC);
    if(fd<0 && errno == ENXIO){
        perror("open failed:");
        return -1;
    }

    if(write(fd, (char *)Input, size)<0){
        perror("write failed");
        return -1;
    }
    close(fd);
    return 0;
}

void * Fifo_read(int Num, int buffer, int * fd)
{
    void * Input=calloc(buffer, sizeof(void));
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return NULL;
    }

    *fd=open(fifo_name, O_RDONLY);
    if(*fd<0){
        perror("open failed:");
        return NULL;
    }
    if(read(*fd, Input, buffer)<0){
        perror("read failed");
        close(*fd);
        return NULL;
    }
    return Input;
}