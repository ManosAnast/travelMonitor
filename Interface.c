# include "Interface.h"

void TTY(Virus * Vlist, Country * Clist)
{ 
    char **Array;
    Array=(char**)malloc(9*sizeof(char *)); // make an arry of strings with 30 characters each string.
    for(int i=0 ; i< 9 ; i++){
        Array[i]=(char*)malloc(50*sizeof(char));
    }
    MonitorCheck * MonitorList=MCInit();
    while (1){
        char Answer[100];
        printf("Give a command\n");
        fgets(Answer,100,stdin);
        if(( strlen(Answer)>0 ) && (Answer[strlen(Answer) - 1])=='\n'){
            Answer[strlen(Answer)-1]='\0';
        }
        if(!strcmp(Answer, "exit")){
            BreakString(&Array, Answer, " ", 2);// Array has the userts input
            int fd, Length;
            Clist=Clist->Next;
            while(Clist != NULL){
                void * input=serialize_commands(Array, &Length);
                Fifo_writeCommands(Clist->Id, input, Length, &fd);
                Clist=Clist->Next;
            }  
            break;
        }
        BreakString(&Array, Answer, " ", 9);// Array has the userts input
        // Checks for commands and if the call is complete.
        
        if ( !strcmp(Array[0], "travelRequest") ){
            if(!strcmp(Array[1], NULLstring)){
                printf("Wrong command. vaccineStatus is called like:\ntravelRequest citizenID date countryFrom countryTo virusName \n\n"); continue;
            }
            travelRequest(MonitorList, Vlist, Array, Clist);
            nothing();
        }
        else if ( !strcmp(Array[0], "travelStat") ){
            travelStat(MonitorList, Clist, Array[1], Array[2], Array[3], Array[4]);
        }
        // else if ( !strcmp(Array[0], "searchVaccinationStatus") ){
        //     if(!strcmp(Array[1], NULLstring)){
        //         printf("Wrong command. vaccineStatus is called like:\nsearchVaccinationStatus citizenID\n\n"); continue;
        //     }
        //     searchVaccinationStatus(Vlist, Clist, Array);
        // }
        
        
        printf("\n");
    }
    for (int i = 0; i < 9; i++){
        free(Array[i]);
    }
    free(Array);
    return;
}


void TTYMonitor(Virus * Vlist, int id, int buffer)
{
    int fd;
    char fifo_name[100];
    if(snprintf(fifo_name, sizeof(fifo_name), "./fifo/TravelMonitor%d", id)<0){
        return;
    }
    mkfifo(fifo_name, 0666);
    
    MonitorCheck * MonitorList;
    MCInit(MonitorList);
    while (1){
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

        FD_ZERO(&fds); // Clear FD set for select
        FD_SET(fd, &fds);
        timeout.tv_sec = 60; /* One minute */
        timeout.tv_usec = 0; /* and no millionths of seconds */

        if((s=select(maxfd + 1, &fds, NULL, NULL, &timeout)) < 0){
            perror("Select"); exit(EXIT_FAILURE);
        }
        else if (!s){
            continue;
        }
        
        if (FD_ISSET(fd, &fds)){
            res=read(fd, Input, buffer);
            if(res<0){
                perror("read failed");
                close(fd);
                return;
            }
        }
        else{
            continue;
        }
        
        if (res == 0){
            continue;
        }
        close(fd);

        char ** Array=unserialize_commands(Input);

        if(!strcmp(Array[0], "exit")){
            for (int i = 0; i < 6; i++){
                free(Array[i]);
            }
            free(Array);
            break;
        }
        if (!strcmp(Array[0], "travelRequest")){
            travelRequestMonitor(id, Array[1], Array[5]);
            for (int i = 0; i < 5; i++){
                free(Array[i]);
            }
            free(Array);
        }
        
    }
    

}