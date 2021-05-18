# include "Interface.h"

void travelRequest(MonitorCheck * MonitorList, Virus * Vlist, char ** Array, Country * Clist)
{
    char * Id=Array[1]; char * TravelDate=Array[2]; char * CountryFrom=Array[3]; char * CountryTo=Array[4]; char * VirusName=Array[5];
    Virus * VTemp=Vlist;
    while (VTemp != NULL){
        if (!strcmp(VTemp->VirusName, VirusName)){
            int exist=bloomBitExist(&(VTemp->filter), Id);
            /* The bloomfilter returns true so, go to country monitor and search there.*/
            if (exist){
                int fd, monitorId=CountryId(Clist, CountryFrom), Length; // Get the monitorId that corresponds to the country.

                // Send the commands and the citizen id.
                void * input=serialize_commands(Array, &Length);
                Fifo_writeCommands(monitorId, input, Length, &fd); close(fd); free(input);

                // Receive the result from the monitor.
                void * Input=Fifo_readCommands(monitorId, &fd); close(fd);
                char ** Answer=unserialize_commands(Input); free(Input);

                Date * RequestDate=CreateDate(TravelDate); // Make the Date struct for the TravelDate.
                if(!strcmp(Answer[0], "NO")){ // If monitor returned no, insert it as rejected to the MonitorList and return.
                    printf("REQUEST REJECTED – YOU ARE NOT VACCINATED\n");
                    
                    MCInsert(MonitorList, VirusName, CountryFrom, false, true, RequestDate);
                    
                    close(fd);
                    free(Answer[0]); free(Answer[1]); free(Answer);
                    return;
                }
                else{// Otherwise, check for the time that has passed from the vaccination.
                    char * CitizenDate=(char *)calloc(11, sizeof(char)); sprintf(CitizenDate, "%s-%s-%s", Answer[0], Answer[1], Answer[2]);

                    Date * VaccDate=CreateDate(CitizenDate); // Make the Date struct for the citizen vaccination date.
                    free(CitizenDate);
                    if(CheckDateDiffer(VaccDate, RequestDate)){ // If the function that calculates the time passed returns true, then the citizen is ok.
                        printf("REQUEST ACCEPTED – HAPPY TRAVELS\n");

                        // Insert the request to the MonitorList a accepted and return.
                        MCInsert(MonitorList, VirusName, CountryFrom, true, false, RequestDate);
                        
                        close(fd);
                        free(Answer[0]); free(Answer[1]); free(Answer[2]); free(Answer[3]); free(Answer); free(VaccDate);
                        return;
                    }
                    // Otherwise, print a message and insert the request to the MonitorList as rejected and return.
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
    // Create the Date struct for the dates that the user gives as limits.
    Date * Timing1=CreateDate(Date1), * Timing2=CreateDate(Date2);

    if ( !strcmp(CountryName, NULLstring) ){ // If the user didn't give us a country.
        Country * CTemp=Clist->Next;
        while (CTemp!=NULL){ // Traverse the country list. For every country check the MonitorList and print the stats that we found.
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
    // If the user gave us a country, traverse the MonitorList for this country and print the stats that we found.
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
    while (Temp != NULL){ // Traverse the country list and send the command to every monitor.
        if (Temp->Id != -1){
            void * input=serialize_commands(Array, &size);
            Fifo_writeCommands(Temp->Id, input, size, &fd); close(fd);

            void * Input=Fifo_readCommands(Temp->Id, &fd); close(fd);
            char ** Answer=unserialize_commands(Input);

            if ( !strcmp(Answer[0], Array[1])){ // If the monitor returned information about the citizen, print them.
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