# include "Interface.h"

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

void VaccinateStatus(Virus * Vlist, char * Id, char * VirusName)
{
    Virus * Temp = VirusFind(Vlist, VirusName);
    LinkedList * exist = SLSearch(Temp->vaccinated_persons, atoi(Id));
    if (exist != NULL)
    {
        printf("VACCINATED ON "); 
        Citizens * Rec=HTSearch(atoi(Id), VirusName); PrintDate(Rec->Timing);
    }
    else{
        printf("NOT VACCINATED\n");
    }
    return;
}

void ListNonVaccinated(Virus * Vlist, char * VName)
{
    Virus * Temp=Vlist->Next;
    Temp=VirusFind(Vlist, VName);
    if(Temp == NULL){
        printf("Didn't find any skiplist for %s\n",VName);
        return;
    }
    LinkedList * List=Temp->not_vaccinated_persons->Header->Next[0]; // We want the whole list of persons. Therefore, I use the 0th level. 
    Citizens * Person;
    while (List != NULL){
        Person=HTSearch(List->Id, VName);
        if (Person != NULL){
            printf("%d %s %s %s %d\n",Person->citizenId, Person->FirstName, Person->LastName, Person->Country, Person->Age);
        }
        List=List->Next[0];
    }
    return;
}

void InsertCitizenRecord(Virus * Vlist, Country * CList, char ** Array)
{
    Virus * Temp = VirusFind(Vlist, Array[6]);
    bloom filter= Temp->filter;
    int exist=bloomBitExist(&filter, Array[1]); // Check the bloom filter.
    if (exist){ // Maybe vaccinated.
        Virus * Temp = VirusFind(Vlist, Array[6]);
        LinkedList * exist = SLSearch(Temp->vaccinated_persons, atoi(Array[1]));
        if (exist != NULL){ // If the citizen has been vaccinated, print and return.
            printf("VACCINATED ON\n"); 
            Citizens * Rec=HTSearch(atoi(Array[1]), Array[6]); PrintDate(Rec->Timing); return;
        }

        if (!strcmp(Array[7],"NO")){
            if(HTSearch(atoi(Array[1]), Array[6])!=NULL){ // Check for duplication
                printf("Citizen with %d has already been added for %s\n", atoi(Array[1]), Array[6]); return;
            }
            int Flag=HTInsert(atoi(Array[1]), Array[2], Array[3], Array[4], atoi(Array[5]), Array[6], false, Array[8]); // give the arguments to the htinsert.
            if(Flag){    
                VirusInsert(&Vlist, Array[1], Array[6], false, Array[8]); 
                CountryInsert(&CList, Array[4]);
                printf("Insertion has been successfully completed\n");
            }
        }
        else if (!strcmp(Array[7],"YES")){
            if(HTSearch(atoi(Array[1]), Array[6])!=NULL){ // Check for duplication
                printf("Citizen with %d has already been added for %s\n", atoi(Array[1]), Array[6]); return;
            }
            int Flag=HTInsert(atoi(Array[1]), Array[2], Array[3], Array[4], atoi(Array[5]), Array[6], true, Array[8]); // give the arguments to the htinsert.
            if(Flag){
                VirusInsert(&Vlist, Array[1], Array[6], true, Array[8]);
                CountryInsert(&CList, Array[4]);
                printf("Insertion has been successfully completed\n");
            }
        }  
    }
    else{ // If the citizen hasn't been vaccinated, procced with a normal insertion.
        if (!strcmp(Array[7],"NO")){
            if(HTSearch(atoi(Array[1]), Array[6])!=NULL){ // Check for duplication
                printf("Citizen with %d has already been added for %s\n", atoi(Array[1]), Array[6]);
            }
            else{    
                int Flag=HTInsert(atoi(Array[1]), Array[2], Array[3], Array[4], atoi(Array[5]), Array[6], false, Array[8]); 
                if (Flag){
                    VirusInsert(&Vlist, Array[1], Array[6], false, Array[8]);
                    CountryInsert(&CList, Array[4]);
                    printf("Insertion has been successfully completed\n");
                }
            }
        }
        else if (!strcmp(Array[7],"YES")){
            if(HTSearch(atoi(Array[1]), Array[6])!=NULL){ // Check for duplication
                printf("Citizen with %d has already been added for %s\n", atoi(Array[1]), Array[6]);
            }
            else{
                int Flag=HTInsert(atoi(Array[1]), Array[2], Array[3], Array[4], atoi(Array[5]), Array[6], true, Array[8]);
                if(Flag){
                    VirusInsert(&Vlist, Array[1], Array[6], true, Array[8]);
                    CountryInsert(&CList, Array[4]);
                    printf("Insertion has been successfully completed\n");
                }
            }
        } 
    }
    return;
}

void VaccinateNow(Virus * Vlist, Country * CList, char ** Array)
{
    Virus * Temp = VirusFind(Vlist, Array[6]);
    LinkedList * exist = SLSearch(Temp->vaccinated_persons, atoi(Array[1]));
    if (exist != NULL){ // If citizen is already vaccinated
        printf("ERROR: CITIZEN %s ALREADY VACCINATED ON ", Array[1]);
        Citizens * Rec=HTSearch(atoi(Array[1]), Array[6]);
        if (Rec != NULL){
            PrintDate(Rec->Timing);
        }
    }
    else{ // If citizen is not vaccinated delete him/her from not vaccinated skiplist, insert him/her to vaccinated skiplist and change the hash table.
        VirusInsert(&Vlist, Array[1], Array[6], true, Array[8]); 
        Citizens * Rec=HTSearch(atoi(Array[1]), Array[6]); 
        if (Rec != NULL){
            SLDelete(&(Temp->not_vaccinated_persons), atoi(Array[1]));
            Rec->Vaccinated=true; Rec->Timing=CreateDate("Today");
        }
        else{
            HTInsert(atoi(Array[1]), Array[2], Array[3], Array[4], atoi(Array[5]), Array[6], true, "Today");
        }
    }
    return;
}

void populationStatus(Virus * VList, Country * CList, char ** Array, bool Age)
{
    Virus * Temp=VList;
    int * Cat; // Array that keeps number of vaccinated citizens by age group.

    // If the user called the popStatusByAge command, Age is true and we want an array with 4 ints. 
    // If the user called the populationStatus command, Age is false and we want only one int. But, in order to be flexible
    // we use a pointer to an int. 
    if (Age){
        Cat=(int *)calloc(4, sizeof(int));
    }
    else{
        Cat=(int *)calloc(1, sizeof(int));
    }
    
    // Check if a country argument has been given.
    if (strcmp(Array[4], NULLstring)){ // If there is country arguments
        while (Temp->Next!=NULL && strcmp(Temp->VirusName, Array[2])){ // Find the country that the user wants
            Temp=Temp->Next;
        }
        if(Temp->Next == NULL && strcmp(Temp->VirusName, Array[2])){ 
            printf("ERROR: Couldn't find the virus that you gave\n");
            free(Cat);
            return;
        } 
        int Population=CalculatePopulation(Temp, Array[1], Temp->VirusName); // Find the number of people that are not vaccinated.
        Cat = CalculateVaccinated(Temp, Array[1], Age, Cat, Array[3], Array[4]); // Find the number of vaccinated persons by age
        PrintCat(Array[1], Cat, Age, Population);
        free(Cat);
        return;
    }
    while (Temp->Next!=NULL && strcmp(Temp->VirusName, Array[1])){ // Find the virus that we want
        Temp=Temp->Next;
    }
    if(Temp->Next == NULL && strcmp(Temp->VirusName, Array[1])){
        printf("ERROR: Couldn't find the virus that you gave\n");
        free(Cat);
        return;
    }
    CList=CList->Next;
    while (CList != NULL){
        int Population=CalculatePopulation(Temp, CList->CName, Temp->VirusName);
        Cat = CalculateVaccinated(Temp, CList->CName, Age, Cat, Array[2], Array[3]);
        if (Cat == NULL){
            return;
        }
        PrintCat(CList->CName, Cat, Age, Population);
        CList=CList->Next;
    }
    
    free(Cat);
    return;
}

int * CalculateVaccinated(Virus * Vlist, char * Country, bool Age, int * Array, char * Date1, char * Date2)
{
    LinkedList * Temp=Vlist->vaccinated_persons->Header->Next[0]; // We want the whole list of persons. Therefore, I use the 0th level. 
    Date * Timing1=CreateDate(Date1), * Timing2=CreateDate(Date2);
    Citizens * Record;
    while (Temp != NULL){ // Travers the linked list
        Record=HTSearch(Temp->Id, Vlist->VirusName); // Search for the hash table node that represents the citizen that we want
        if(Record == NULL){    
            printf("There is no citizen with %d id and %s virus \n", Temp->Id, Vlist->VirusName); return NULL;
        }
        if(!strcmp(Record->Country, Country) && CheckDate(Record->Timing, Timing1) && CheckDate(Timing2, Record->Timing)){ // If country and date are correct
            // Here is used the same principal as in populationStatus function and cat array.
            if(Age){ 
                InsertByAge(Array, Record->Age, Record->Vaccinated);
            }
            else{
                *Array += (int)Record->Vaccinated;
            }
        }
        Temp=Temp->Next[0];
    }
    free(Timing1); free(Timing2);
    return Array;
}

int CalculatePopulation(Virus * List1, char * Country, char * Virus)
{
    LinkedList * Temp=List1->not_vaccinated_persons->Header->Next[0];
    int Population=0;
    while(Temp!=NULL){
        if (!strcmp(HTSearch(Temp->Id, Virus)->Country, Country)){
            Population+=1;
        }
        Temp=Temp->Next[0];
    }
    return Population;
}


// Based on the age, makes the correct insertion.
void InsertByAge(int * Array, int recordAge, bool Vaccinated)
{
    if(recordAge < 20){
        Array[0] += (int)Vaccinated;
    }
    else if (recordAge >= 20 && recordAge < 40){
        Array[1] += (int)Vaccinated;
    }
    else if (recordAge >= 40 && recordAge < 60){
        Array[2] += (int)Vaccinated;
    }
    else if (recordAge >= 60){
        Array[3] += (int)Vaccinated;
    }
    return;
}

void PrintCat(char * Country, int * Array, bool Age, int Population)
{
    if (Age){
        Population += Array[0] + Array[1] + Array[2] + Array[3];
        printf("%s\n", Country);
        printf("0-20 %d %0.1f%c \n", Array[0], (double)Array[0]/(double)Population * 100, '%');
        printf("20-40 %d %0.1f%c \n", Array[1], (double)Array[1]/(double)Population * 100, '%');
        printf("40-60 %d %0.1f%c \n", Array[2], (double)Array[2]/(double)Population * 100, '%');
        printf("60+ %d %0.1f%c \n", Array[3], (double)Array[3]/(double)Population * 100, '%');
        for (int i = 0; i < 4; i++){
            Array[i]=0;
        }
        return;
    }
    Population += *Array;
    printf("%s:%d %0.1f%c \n", Country, *Array, (double)*Array/(double)Population * 100, '%');
    *Array=0;
    return;
}