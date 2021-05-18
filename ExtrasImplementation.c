# include "General.h"

Virus * VirusInit()
{
    Virus * VList=(Virus *)calloc(1, sizeof(Virus));
    char * Vname=(char *)calloc(2, sizeof(char));
    VList->VirusName=Vname;
    VList->vaccinated_persons=LLInit();
    VList->not_vaccinated_persons=LLInit();
    strcpy(VList->VirusName,NULLstring); VList->Next=NULL;
    return VList;
}

void VirusInsert(Virus ** VList, char * CitizenId, char * VName, bool Vaccinated, char * DateStr)
{
    Virus * Temp=*VList;
    int Id=atoi(CitizenId);
    while (Temp->Next != NULL && strcmp(Temp->VirusName, VName)){ // Traverse the virus list
        Temp=Temp->Next;
    }
    if (Temp->Next == NULL && strcmp(Temp->VirusName, VName)){ // If the virus node that we want doesn't exist (Reached last node).
        Virus *  NewNode=(Virus *)calloc(1, sizeof(Virus));
        NewNode->VirusName=(char *)calloc(strlen(VName)+1, sizeof(char)); strcpy(NewNode->VirusName, VName);

        // Initialize everything that has to be initialized.
        LinkedList * List=LLInit();
        
        LLInsert(List, Id);
        bloom filter=bloomInit(BloomNum);
        NewNode->filter=filter;
        
        // If citizen has been vaccinated, insert him/her to bloom filter and vaccinated_persons skiplist. Otherwise, insert him/her to not_vaccinated_persons skiplist.
        if (Vaccinated){
            NewNode->vaccinated_persons=List;
            bloomSetBit(&filter, CitizenId);
        }
        else{
            NewNode->not_vaccinated_persons=List;
        }
        Temp->Next=NewNode;
    }
    else if ( !strcmp(Temp->VirusName, VName) ){ // If the virus node that we want exist.

        if (Vaccinated){ // If the persons that was inserted, is vaccinated
            LinkedList * llist=Temp->vaccinated_persons;
            
            // Insert the citizen to the bloom filter.
            bloom filter=Temp->filter;
            bloomSetBit(&filter, CitizenId);
            Temp->filter=filter;

            if (llist == NULL){ // If there wasn't any vaccinated person for the virus, initialize the vaccinated_persons skiplist and insert to it.
                llist=LLInit();
                LLInsert(llist, Id);
                Temp->vaccinated_persons=llist;
            }
            else{ // Otherwise, just insert to vaccinated_persons skiplist.
                LLInsert(llist, Id);
                Temp->vaccinated_persons=llist;
            }
        }
        else{ // If the persons that was inserted, hasn't been vaccinated.
            LinkedList * llist=Temp->not_vaccinated_persons;
            
            if (llist == NULL){ // If there wasn't any not vaccinated person for the virus, initialize the not_vaccinated_persons skiplist and insert to it.
                llist=LLInit();
                LLInsert(llist, Id);
                Temp->not_vaccinated_persons=llist;
            }
            else{ // Otherwise, just insert to not_vaccinated_persons skiplist.
                LLInsert(llist, Id);
                Temp->not_vaccinated_persons=llist;
            }
        }
    }
    
} 


void VirusInsertBloom(Virus ** VList, char * VName, bloom filter)
{
    Virus * Temp=*VList;
    while (Temp->Next != NULL){
        Temp=Temp->Next;
    }
    Virus * NewNode=(Virus *)calloc(1, sizeof(Virus));
    NewNode->filter=filter;
    NewNode->VirusName=(char *)calloc(strlen(VName)+1, sizeof(char)); strcpy(NewNode->VirusName, VName);
    NewNode->vaccinated_persons=NULL; NewNode->not_vaccinated_persons=NULL;
    Temp->Next=NewNode;
    return;
}


int VirusCount(Virus * VList)
{
    Virus * VTemp=VList->Next;
    int count=0;
    while (VTemp != NULL){
        count+=1;
        VTemp=VTemp->Next;
    }
    return count;
}


// void VirusSkipList(Virus ** VList)
// {
//     Virus * Temp= *VList;
//     Temp=Temp->Next;
    
//     while (Temp != NULL){
//         SLInsert(Temp->vaccinated_persons); 
//         SLInsert(Temp->not_vaccinated_persons);
//         Temp=Temp->Next;
//     }
    
// }


Virus * VirusFind(Virus * Vlist, char * VirusName)
{
    Virus * Temp= Vlist;
    while (Temp != NULL){
        printf("VirusName: %s\n", Temp->VirusName);
        if (!strcmp(Temp->VirusName, VirusName)){
            return Temp;
        }
        Temp=Temp->Next;
    }
    return Temp;
}

void VirusDestroy(Virus ** VList)
{
    Virus * Current=(*VList), *Next;
    while (Current!= NULL){ 
        Next=Current->Next;
        free(Current->VirusName); 
        LLDestroy(&(Current->vaccinated_persons)); 
        LLDestroy(&(Current->not_vaccinated_persons)); 
        bloomDestroy(&(Current->filter)); 
        free(Current);
        Current=Next;
    }
    return;
}

void VirusPrint(Virus * Vlist)
{
    Virus * Temp=Vlist;
    while (Temp != NULL)
    {
        printf("%s, %d, %d\n", Temp->VirusName, Temp->vaccinated_persons==NULL, Temp->not_vaccinated_persons==NULL);
        Temp=Temp->Next;
    }
    return;
}

Country * CountryCreate()
{
    Country * CList=(Country *)calloc(1, sizeof(Country)); CList->Next=NULL;
    CList->Id=-1; CList->pid=-1;
    CList->CName=(char *)calloc(20, sizeof(char)); strcpy(CList->CName, NULLstring);
    return CList;
}

Country * CountrySearch(Country * CList, char * Name)
{
    while (CList->Next != NULL && strcmp(CList->CName, Name)){
        CList=CList->Next;
    }
    return CList;
}

void CountryInsert(Country ** CList, char * CName, int Id, int pid)
{
    char * Name=FixName(CName);
    int Length=strlen(Name);
    Country * Temp=*CList;
    while (Temp->Next != NULL){
        if (Temp->Id == Id){
            Temp->CName=(char *)realloc(Temp->CName, (Length+1)*sizeof(char)); strcpy(Temp->CName, Name);
            free(Name);
            return;
        }
        Temp=Temp->Next;
    }
    Country * NewNode=(Country *)calloc(1, sizeof(Country));
    NewNode->CName=(char *)calloc(Length+1, sizeof(char)); strcpy(NewNode->CName, Name);
    NewNode->Id=Id; NewNode->pid=pid; NewNode->Next=NULL;
    Temp->Next=NewNode;
    free(Name);
    return;
}

char * FixName(char * CName)
{
    char **Array=(char **)calloc(3, sizeof(char *));
    for (int i = 0; i < 3; i++)
    {
        Array[i]=(char *)calloc(20, sizeof(char));
    }
    
    BreakString(&Array, CName, "/", 3);
    char * Country=(char *)calloc(strlen(Array[2])+1, sizeof(char)); strcpy(Country, Array[2]);
    
    for (int i = 0; i < 3; i++){
        free(Array[i]);
    }
    free(Array);
    return Country;
}

int CountryId(Country * CList, char * CName)
{
    Country * Temp=CList;
    while (Temp != NULL){
        if(!strcmp(Temp->CName, CName)){
            return Temp->Id;
        }
        Temp=Temp->Next;
    }
    return -1;
}

char * CountryFind(Country * CList, int monitorId)
{
    Country * Temp=CList;
    while (Temp != NULL){
        if(Temp->Id == monitorId){
            return Temp->CName;
        }
        Temp=Temp->Next;
    }
    return NULLstring;
}

void CountryDestroy(Country ** CList)
{
    Country * Current=*CList, *Next;
    while (Current!= NULL){
        Next=Current->Next;
        free(Current->CName); free(Current);
        Current=Next;
    }
    return;
}