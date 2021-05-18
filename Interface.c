# include "Interface.h"

void TTY(Virus * Vlist, Country * Clist)
{ 
    char logname[20];
    if(snprintf(logname, sizeof(logname), "logfile.%d", getpid())<0){
        return ;
    }
    FILE * fp;
    fp=fopen(logname, "w");

    char **Array;
    Array=(char**)malloc(6*sizeof(char *)); // make an arry of strings with 30 characters each string.
    for(int i=0 ; i< 6 ; i++){
        Array[i]=(char*)malloc(50*sizeof(char));
    }

    void (*oldhandler)(int);
    oldhandler = signal(SIGINT, signal_iq);
    oldhandler = signal(SIGQUIT, signal_iq);
    oldhandler = signal(SIGCHLD, signal_chld);

    Country * CTemp=Clist->Next;
    while (CTemp!=NULL){
        if (strcmp(CTemp->CName, NULLstring)){
            fprintf(fp, "%s\n", CTemp->CName);
        }
        CTemp=CTemp->Next;
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
            
            int * Log=(int *)calloc(3, sizeof(int)); Log=MCLog(MonitorList);
            fprintf(fp, "TOTAL TRAVEL REQUESTS %d\nACCEPTED %d\nREJECTED %d", Log[0], Log[1], Log[2]);
            free(Log); fclose(fp);

            Clist=Clist->Next;
            while(Clist != NULL){
                kill(Clist->pid, SIGKILL);
                Clist=Clist->Next;
            }  
            break;
        }

        BreakString(&Array, Answer, " ", 6);// Array has the userts input
        // Checks for commands and if the call is complete.
        
        if ( !strcmp(Array[0], "travelRequest") ){
            if(!strcmp(Array[5], NULLstring)){
                printf("Wrong command. travelRequest is called like:\ntravelRequest citizenID date countryFrom countryTo virusName \n\n"); continue;
            }
            travelRequest(MonitorList, Vlist, Array, Clist);
        }
        else if ( !strcmp(Array[0], "travelStats") ){
            if(!strcmp(Array[3], NULLstring)){
                printf("Wrong command. addVaccinationRecords is called like:\naddVaccinationRecords country\n\n"); continue;
            }
            travelStat(MonitorList, Clist, Array[1], Array[2], Array[3], Array[4]);
        }
        else if ( !strcmp(Array[0], "addVaccinationRecords") ){
            if(!strcmp(Array[1], NULLstring)){
                printf("Wrong command. addVaccinationRecords is called like:\naddVaccinationRecords country\n\n"); continue;
            }
            Country * Temp=CountrySearch(Clist, Array[1]);
            SendSignal(Temp, SIGUSR1);
        }
        else if ( !strcmp(Array[0], "searchVaccinationStatus") ){
            if(!strcmp(Array[1], NULLstring)){
                printf("Wrong command. searchVaccinationStatus is called like:\nsearchVaccinationStatus citizenID\n\n"); continue;
            }
            searchVaccinationStatus(Vlist, Clist, Array);
        }
        else{
            printf("There is no %s command. Try again.\n", Array[0]);
        }
        

        int status;
        if(interrupt_flag_iq){
            
            int * Log=(int *)calloc(3, sizeof(int)); Log=MCLog(MonitorList);
            fprintf(fp, "TOTAL TRAVEL REQUESTS %d\nACCEPTED %d\nREJECTED %d", Log[0], Log[1], Log[2]);
            free(Log); fclose(fp);

            Country * CTemp=Clist->Next;
            while (CTemp != NULL){
                kill(CTemp->pid, SIGINT);
                CTemp=CTemp->Next;
            }
            break;
        }
        printf("\n");
    }

    MCDestroy(MonitorList);
    for (int i = 0; i < 6; i++){
        free(Array[i]);
    }
    free(Array);
    return;
}
