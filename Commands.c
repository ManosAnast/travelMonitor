# include "Interface.h"

void travelRequest(MonitorCheck * MonitorList, Virus * Vlist, char ** Array, Country * Clist)
{
    char * Id=Array[1]; char * TravelDate=Array[2]; char * CountryFrom=Array[3]; char * CountryTo=Array[4]; char * VirusName=Array[5];
    Virus * VTemp=Vlist;
    while (VTemp != NULL){
        if (!strcmp(VTemp->VirusName, VirusName)){
            int exist=bloomBitExist(&(VTemp->filter), Id);
            if (exist){
                /*Go to country monitor and search there.*/
                int fd, monitorId=CountryId(Clist, CountryFrom), Length;
                void * input=serialize_commands(Array, &Length);
                Fifo_writeCommands(monitorId, input, Length, &fd); close(fd); free(input);

                void * Input=Fifo_readCommands(monitorId, &fd); close(fd);
                char ** Answer=unserialize_commands(Input); free(Input);
                Date * RequestDate=CreateDate(TravelDate);
                if(!strcmp(Answer[0], "NO")){
                    printf("REQUEST REJECTED – YOU ARE NOT VACCINATED\n");
                    MCInsert(MonitorList, VirusName, CountryFrom, false, true, RequestDate);
                    close(fd);
                    free(Answer[0]); free(Answer[1]); free(Answer);
                    return;
                }
                else{
                    char * CitizenDate=(char *)calloc(11, sizeof(char));
                    sprintf(CitizenDate, "%s-%s-%s", Answer[0], Answer[1], Answer[2]);
                    Date * VaccDate=CreateDate(CitizenDate);
                    free(CitizenDate);
                    if(CheckDateDiffer(VaccDate, RequestDate)){
                        printf("REQUEST ACCEPTED – HAPPY TRAVELS\n");
                        MCInsert(MonitorList, VirusName, CountryFrom, true, false, RequestDate);
                        close(fd);
                        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer); free(VaccDate);
                        return;
                    }
                    printf("REQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE\n");
                    MCInsert(MonitorList, VirusName, CountryFrom, false, true, RequestDate);
                    free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer); free(VaccDate);
                }
                return;
            }
        }
        VTemp=VTemp->Next;
    }
    printf("REQUEST REJECTED – YOU ARE NOT VACCINATED\n");
    return;
}

void travelStat(MonitorCheck * MonitorList, Country * Clist, char * VirusName, char * Date1, char * Date2, char * CountryName)
{
    Date * Timing1=CreateDate(Date1), * Timing2=CreateDate(Date2);
    if ( !strcmp(CountryName, NULLstring) ){
        Country * CTemp=Clist->Next;
        while (CTemp!=NULL)
        {
            strcpy(CountryName, CTemp->CName);
            MonitorCheck * Temp = MonitorList;
            int accepted=0, rejected=0;
            while (Temp != NULL){
                if ( !strcmp(Temp->VirusName, VirusName) && !strcmp(Temp->CountryName, CountryName) && CheckDate(Temp->RequestDate, Timing1) && CheckDate(Timing2, Temp->RequestDate) ){
                    accepted+=(int)Temp->Accepted; rejected+=(int)Temp->Rejected;
                }
                Temp=Temp->Next;
            }
            printf("Country:%s\nTOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n", CountryName, accepted+rejected, accepted, rejected);
            CTemp=CTemp->Next;

        }
        free(Timing1); free(Timing2);
        return;
    }
    MonitorCheck * Temp = MonitorList;
    int accepted=0, rejected=0;
    while (Temp != NULL){
        if ( !strcmp(Temp->VirusName, VirusName) && !strcmp(Temp->CountryName, CountryName) && CheckDate(Temp->RequestDate, Timing1) && CheckDate(Timing2, Temp->RequestDate) ){
            accepted+=(int)Temp->Accepted; rejected+=(int)Temp->Rejected;
        }
        Temp=Temp->Next;
    }
    printf("TOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n", accepted+rejected, accepted, rejected);
    free(Timing1); free(Timing2);
    return;
}


void searchVaccinationStatus(Virus * Vlist, Country * Clist, char ** Array)
{
    int size, fd;
    Country * Temp=Clist;
    while (Temp != NULL){
        if (Temp->Id != -1){
            void * input=serialize_commands(Array, &size);
            Fifo_writeCommands(Temp->Id, input, size, &fd); close(fd);
            void * Input=Fifo_readCommands(Temp->Id, &fd); close(fd);
            char ** Answer=unserialize_commands(Input);
            if ( !strcmp(Answer[0], Array[1])){
                printf("%s %s %s %s\nAGE %s\n%s %s\n", Answer[0], Answer[1], Answer[2], Temp->CName, Answer[3], Answer[4], Answer[5]);
                free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer[4]); free(Answer[5]); free(Answer);
                free(input); free(Input);
                break;
            }
            free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer[4]); free(Answer[5]); free(Answer);
            free(input); free(Input);
        }
        
        Temp=Temp->Next;
    }
    return;
}

void VaccinateStatusBloom(Virus * Vlist, char * Id, char * VirusName)
{
    Virus * Temp = VirusFind(Vlist, VirusName);
    bloom filter= Temp->filter;
    int exist=bloomBitExist(&filter, Id);
    if (exist)
    {
        printf("MAYBE\n");
    }
    else{
        printf("NOT VACCINATED\n");
    }
    return;
}

Date * VaccinateStatus(Virus * Vlist, char * Id, char * VirusName)
{
    Virus * Temp = VirusFind(Vlist, VirusName);
    LinkedList * exist = LLSearch(Temp->vaccinated_persons, atoi(Id));
    if (exist != NULL)
    {
        // printf("VACCINATED ON "); 
        Citizens * Rec=HTSearch(atoi(Id), VirusName); 
        return Rec->Timing;
    }
    else{
        return NULL;
    }
    return NULL;
}