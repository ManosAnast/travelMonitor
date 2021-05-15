# include "Interface.h"

void SendSignal(Country * Temp, int signo)
{
    int size, fd;
    
    char ** Array=(char **)calloc(1, sizeof(char *));
    Array[0]=(char *)calloc(strlen(NULLstring), sizeof(char)); strcpy(Array[0], NULLstring);
    
    // printf("Not the array calloc\n");
    void * input=serialize_commands(Array, &size);
    // printf("1Not the array calloc\n");
    Fifo_writeCommands(Temp->Id, input, size, &fd); close(fd);
    // printf("2Not the array calloc\n");
    free(Array[0]); free(Array);
    // printf("3Not the array calloc\n");
      
    kill(Temp->pid, signo);
    
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", Temp->Id)<0){
        return;
    }
    void * Input=calloc(100, sizeof(void)); 
    fd=open(fifo_name, O_RDONLY);
    printf("4Not the array calloc %s\n", fifo_name);
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
    // printf("receive signal\n");
    interrupt_flag_usr = 1;
}

void signal_iq(int signo)
{
    interrupt_flag_iq=1;
}

void signal_chld(int signo)
{
    int wstat;
    pid_t	pid;

    while (1) {
        pid = wait3 (&wstat, WNOHANG, NULL );
        if (pid <= 0){
            return;
        }
    }
}

void signal_quit(int signo)
{

}

void signal_kill(int signo)
{
    printf("kill\n");
    interrupt_flag_kill=1;
}
