# include "Interface.h"

MonitorCheck *  MCInit()
{
    // Allocate a node and give null values.
    MonitorCheck * MonitorList=(MonitorCheck *)calloc(1, sizeof(MonitorCheck));
    MonitorList->VirusName=(char *)calloc(strlen(NULLstring)+1, sizeof(char)); strcpy(MonitorList->VirusName, NULLstring);
    MonitorList->CountryName=(char *)calloc(strlen(NULLstring)+1, sizeof(char)); strcpy(MonitorList->CountryName, NULLstring);
    MonitorList->Accepted=0; MonitorList->Rejected=0;
    MonitorList->RequestDate=NULL;
    MonitorList->Next=NULL;
    return MonitorList;
}

void MCInsert(MonitorCheck * MonitorList, char * VName, char * CName, bool Accepted, bool Rejected, Date * RequestDate)
{
    MonitorCheck * Temp=MonitorList;
    while (Temp->Next!=NULL){
        // If there has been an identical travel request, update the data for accept and reject.
        if ( !strcmp(Temp->VirusName, VName) && !strcmp(Temp->CountryName, CName) && CheckDateEq(RequestDate, Temp->RequestDate)){
            Temp->Accepted+=(int)Accepted; Temp->Rejected+=(int)Rejected;
            return;
        }
        Temp=Temp->Next;
    }
    // If there hasn't been an identical travel request, make a new node.
    MonitorCheck * NewNode=(MonitorCheck *)calloc(1, sizeof(MonitorCheck));
    NewNode->VirusName=(char *)calloc(strlen(VName)+1, sizeof(char)); strcpy(NewNode->VirusName, VName);
    NewNode->CountryName=(char *)calloc(strlen(CName)+1, sizeof(char)); strcpy(NewNode->CountryName, CName);
    NewNode->Accepted=(int)Accepted; NewNode->Rejected=(int)Rejected;
    NewNode->RequestDate=RequestDate;
    NewNode->Next=NULL;
    Temp->Next=NewNode;
    return;
}

void MCPrint(MonitorCheck * MonitorList, char * VName, char * CName)
{
    MonitorCheck * Temp=MonitorList;
    while (Temp->Next!=NULL){
        if ( !strcmp(Temp->VirusName, VName) ){
            printf("TOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n", Temp->Accepted+Temp->Rejected, Temp->Accepted, Temp->Rejected);
            return;
        }
        Temp=Temp->Next;
    }
    printf("There is no %s virus at this country\n", VName);
    return;
}

void MCDestroy(MonitorCheck * MonitorList)
{
    MonitorCheck * Current=MonitorList, *Next;
    while (Current!= NULL){
        Next=Current->Next;
        free(Current->CountryName); free(Current->VirusName); free(Current->RequestDate); free(Current);
        Current=Next;
    }
    return;
}

int * MCLog(MonitorCheck * MonitorList)
{
    MonitorCheck * Temp=MonitorList;
    int *Array=(int *)calloc(3, sizeof(int));
    while (Temp->Next!=NULL){
        Array[0]+=Temp->Accepted+Temp->Rejected; Array[1]+=Temp->Accepted; Array[2]+=Temp->Rejected;
        Temp=Temp->Next;
    }   
    return Array;
}