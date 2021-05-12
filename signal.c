# include "Interface.h"

void SendSignal(Country * Temp, int signo)
{
    int size, fd;
    
    char ** Array=(char **)calloc(1, sizeof(char *));
    Array[0]=(char *)calloc(strlen(NULLstring), sizeof(char)); strcpy(Array[0], NULLstring);
    
    
    kill(Temp->pid, signo);

    void * input=serialize_commands(Array, &size);
    Fifo_writeCommands(Temp->Id, input, size, &fd); close(fd);

    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", Temp->Id)<0){
        return;
    }
    void * Input=calloc(100, sizeof(void)); 
    fd=open(fifo_name, O_RDONLY);
    if(fd<0){
        perror("open failed:");
        return;
    }
    struct timeval  timeout;
    fd_set fds;
    int maxfd=fd;
    int res=0, s;
    char buf[256];

    FD_ZERO(&fds); // Clear FD set for select
    FD_SET(fd, &fds);
    timeout.tv_sec = 60; /* One minute */
    timeout.tv_usec = 0; /* and no millionths of seconds */

    if((s=select(maxfd + 1, &fds, NULL, NULL, &timeout)) < 0){
        perror("Select"); exit(EXIT_FAILURE);
    }
    
    if (FD_ISSET(fd, &fds)){
        res=read(fd, Input, 100);
        if(res<0){
            perror("read failed");
            close(fd);
            return;
        }
    }
    
    // void * Input=Fifo_readCommands(Temp->Id, 100, &fd); close(fd);
    // char ** Answer=unserialize_commands(Input);
}

void signal_usr(int signo)
{
    interrupt_flag_usr = 1;
}

void signal_int(int signo)
{
}

void signal_chld(int signo)
{
    
}

void signal_quit(int signo)
{

}

// void signal_kill(int signo)
// {

// }
