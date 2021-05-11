# include "Interface.h"

void signal_usr(int signo)
{
    interrupt_flag_usr = 1;

    // int size, fd;
    // char ** Array=(char **)calloc(1, sizeof(char *));
    // Array[0]=(char *)calloc(strlen(NULLstring), sizeof(char)); strcpy(Array[0], NULLstring);
    // void * Input=serialize_commands(Array, &size);
    // Fifo_writeCommands(0, Input, size, &fd); close(fd);
    // free(Array[0]); free(Array);
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
