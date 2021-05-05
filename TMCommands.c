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
        free(Answer[0]); free(Answer);
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