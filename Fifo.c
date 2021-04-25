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

int Fifo_write(int Num, void * Input)
{
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return -1;
    }

    int fd=open(fifo_name, O_WRONLY);
    if(write(fd, Input, sizeof(Input))<0){
        return -1;
    }
    close(fd);
    return 0;
}

void * Fifo_read(int Num)
{
    void * Input;
    char fifo_name[100];
    if(!make_fifo_name(Num, fifo_name, sizeof(fifo_name))){
        return NULL;
    }

    int fd=open(fifo_name, O_RDONLY);
    if(read(fd, Input, 100)<0){
        return NULL;
    }
    close(fd);
    return Input;
}