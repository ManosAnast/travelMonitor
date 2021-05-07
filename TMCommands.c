# include "Interface.h"

void travelRequestMonitor(int monitorId, char * Id, char * VirusName)
{
    Date * VaccDate;
    int fd;

    Citizens * Rec=HTSearch(atoi(Id), VirusName); 
    if (Rec->Vaccinated){
        VaccDate=Rec->Timing;
        PrintDate(VaccDate);
    }
    else{
        VaccDate=NULL;
    }

    int Length;
    if (VaccDate==NULL){
        char ** Answer=(char **)calloc(2, sizeof(char));
        Answer[0]=(char *)calloc(2, sizeof(char));
        strcpy(Answer[0], "NO");
        Answer[1]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer);
    }
    else{
        char ** Answer=(char **)calloc(4, sizeof(char *));
        Answer[0]=(char *)calloc(2, sizeof(char));sprintf(Answer[0], "%d", VaccDate->Days);
        Answer[1]=(char *)calloc(2, sizeof(char));sprintf(Answer[1], "%d", VaccDate->Month);
        Answer[2]=(char *)calloc(4, sizeof(char));sprintf(Answer[2], "%d", VaccDate->Year);
        Answer[3]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer);
    }
    close(fd);
}

void searchVaccinationStatusMonitor(int monitorId, char * Id)
{
    Citizens * Rec = HTSearchID(atoi(Id));
    int Length, fd;
    // printf("monitor%d\n", monitorId);
    if (Rec != NULL){
        char ** Answer=(char **)calloc(6, sizeof(char *));
        Answer[0]=(char *)calloc(4, sizeof(char));sprintf(Answer[0], "%d", Rec->citizenId);
        Answer[1]=(char *)calloc(strlen(Rec->FirstName), sizeof(char));strcpy(Answer[1], Rec->FirstName);
        Answer[2]=(char *)calloc(strlen(Rec->LastName), sizeof(char));strcpy(Answer[2], Rec->LastName);
        Answer[3]=(char *)calloc(2, sizeof(char));sprintf(Answer[3], "%d", Rec->Age);
        Answer[4]=(char *)calloc(strlen(Rec->Virus), sizeof(char));strcpy(Answer[4], Rec->Virus);
        if(Rec->Timing != NULL){
            Answer[5]=(char *)calloc(10, sizeof(char));sprintf(Answer[5], "%d-%d-%d", Rec->Timing->Days, Rec->Timing->Month, Rec->Timing->Year);
        }
        else{
            Answer[5]=(char *)calloc(19, sizeof(char));strcpy(Answer[5], "NOT YET VACCINATED");
        }
        // printf("2monitor%d\n", monitorId);
        void * input=serialize_commands(Answer, &Length);
        Fifo_writeCommands(monitorId, input, Length, &fd);
        // printf("3monitor%d\n", monitorId);
        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer[4]); free(Answer[5]); free(Answer);
        return;
    }
    char ** Answer=(char **)calloc(2, sizeof(char));
    Answer[0]=(char *)calloc(2, sizeof(char));
    strcpy(Answer[0], "NO");
    Answer[1]=(char *)calloc(strlen(NULLstring), sizeof(char));strcpy(Answer[3], NULLstring);
    void * input=serialize_commands(Answer, &Length);
    Fifo_writeCommands(monitorId, input, Length, &fd);
    free(Answer[0]); free(Answer[1]); free(Answer);
    return;
}