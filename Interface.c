# include "Interface.h"

void TTY(Virus * Vlist, Country * Clist)
{ 
    char **Array;
    Array=(char**)malloc(9*sizeof(char *)); // make an arry of strings with 30 characters each string.
    for(int i=0 ; i< 9 ; i++){
        Array[i]=(char*)malloc(50*sizeof(char));
    }
    while (1){
        char Answer[100];
        printf("Give a command\n");
        fgets(Answer,100,stdin);
        if(( strlen(Answer)>0 ) && (Answer[strlen(Answer) - 1])=='\n'){
            Answer[strlen(Answer)-1]='\0';
        }
        if(!strcmp(Answer, "exit")){
            break;
        }
        BreakString(&Array, Answer, " ", 9);// Array has the userts input
        // Checks for commands and if the call is complete.
        
        if (!strcmp(Array[0], "travelRequest")){
            if(!strcmp(Array[1], NULLstring)){
                printf("Wrong command. vaccineStatus is called like:\ntravelRequest citizenID date countryFrom countryTo virusName \n\n"); continue;
            }
            travelRequest(Vlist, Array, Clist);
        }
        
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
            printf("Select ttymonitor monitor%d\n", id);
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

        for (int i = 0; i < 6; i++){
            printf("%s\n", Array[i]);
            if (!strcmp(Array[i], NULLstring)){
                break;
            }
        }
        

        if(!strcmp(Array[0], "exit")){
            for (int i = 0; i < 6; i++){
                free(Array[i]);
            }
            free(Array);
            break;
        }
        if (!strcmp(Array[0], "travelRequest")){
            char * Id=(char *)calloc(strlen(Array[1]), sizeof(char)); 
            char * VirusName=(char *)calloc(strlen(Array[5]), sizeof(char));
            strcpy(Id, Array[1]); strcpy(VirusName, Array[5]);
            for (int i = 0; i < 5; i++){
                free(Array[i]);
            }
            free(Array);
            Date * VaccDate/*=(Date *)calloc(1, sizeof(Date))*/;
            printf("Before vaccinateStatus 1\n");

            Citizens * Rec=HTSearch(atoi(Id), VirusName); 
            printf("Before vaccinateStatus 2\n");
            printf("name:%s, id:%d\n", VirusName, atoi(Id));
            if (Rec->Vaccinated){
                VaccDate=Rec->Timing;
                PrintDate(VaccDate);
            }
            else{
                VaccDate=NULL;
            }

            int Length;
            fd=open(fifo_name, O_WRONLY);
            if(fd<0){
                perror("open failed:");
                return;
            }
            printf("VaccDate %d\n", VaccDate==NULL);
            if (VaccDate==NULL){
                char ** Answer=(char **)calloc(2, sizeof(char));
                Answer[0]=(char *)calloc(2, sizeof(char));
                strcpy(Answer[0], "NO");
                Answer[1]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
                void * input=serialize_commands(Answer, &Length);
                if(write(fd, input, Length)<0){
                    perror("write failed"); return;
                }
                free(Answer[0]); free(Answer);
            }
            else{
                char ** Answer=(char **)calloc(4, sizeof(char *));
                Answer[0]=(char *)calloc(2, sizeof(char));sprintf(Answer[0], "%d", VaccDate->Days);
                Answer[1]=(char *)calloc(2, sizeof(char));sprintf(Answer[1], "%d", VaccDate->Month);
                Answer[2]=(char *)calloc(4, sizeof(char));sprintf(Answer[2], "%d", VaccDate->Year);
                Answer[3]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
                void * input=serialize_commands(Answer, &Length);
                if(write(fd, input, Length)<0){
                    perror("write failed"); return;
                }
                free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer);
            }
            close(fd);
        }
        
    }
    

}