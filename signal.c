# include "General.h"

void SendSignal(Country * Temp, int signo)
{
    int size, fd;
    
    char ** Array=(char **)calloc(1, sizeof(char *));
    Array[0]=(char *)calloc(strlen(NULLstring)+1, sizeof(char)); strcpy(Array[0], NULLstring);
    
    kill(Temp->pid, signo);
    
    void * input=serialize_commands(Array, &size);
    Fifo_writeCommands(Temp->Id, input, size, &fd); close(fd);
    free(input); free(Array[0]); free(Array);
      
    
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", Temp->Id)<0){
        return;
    }
    void * Input=calloc(buffer, sizeof(void)); 
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
    while(1){
        FD_ZERO(&fds); // Clear FD set for select
        FD_SET(fd, &fds);
        timeout.tv_sec = 60; /* One minute */
        timeout.tv_usec = 0; /* and no millionths of seconds */

        if((s=select(maxfd + 1, &fds, NULL, NULL, &timeout)) < 0){
            perror("Select"); exit(EXIT_FAILURE);
        }
        
        if (FD_ISSET(fd, &fds)){
            res=read(fd, Input, buffer);
            break;
        }
    }
    free(Input); close(fd);
    sleep(2);
    return;
}

void signal_usr(int signo)
{
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
    interrupt_flag_kill=1;
}
