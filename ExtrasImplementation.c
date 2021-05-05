# include "Interface.h"

Virus * VirusInit()
{
    Virus * VList=(Virus *)calloc(1, sizeof(Virus));
    char * Vname=(char *)calloc(2, sizeof(char));
    VList->VirusName=Vname;
    VList->vaccinated_persons=SLInit();
    VList->not_vaccinated_persons=SLInit();
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
        SkipList * slist=SLInit();
        LinkedList * List=slist->Header;
        LLInsert(List, Id);
        bloom filter=bloomInit(BloomNum);
        NewNode->filter=filter;
        
        // If citizen has been vaccinated, insert him/her to bloom filter and vaccinated_persons skiplist. Otherwise, insert him/her to not_vaccinated_persons skiplist.
        if (Vaccinated){
            NewNode->vaccinated_persons=slist;
            bloomSetBit(&filter, CitizenId);
        }
        else{
            NewNode->not_vaccinated_persons=slist;
        }
        Temp->Next=NewNode;
    }
    else if ( !strcmp(Temp->VirusName, VName) ){ // If the virus node that we want exist.

        if (Vaccinated){ // If the persons that was inserted, is vaccinated
            SkipList * slist=Temp->vaccinated_persons;
            
            // Insert the citizen to the bloom filter.
            bloom filter=Temp->filter;
            bloomSetBit(&filter, CitizenId);
            Temp->filter=filter;

            if (slist == NULL){ // If there wasn't any vaccinated person for the virus, initialize the vaccinated_persons skiplist and insert to it.
                slist=SLInit();
                LinkedList * List=slist->Header;
                LLInsert(List, Id);
                Temp->vaccinated_persons=slist;
            }
            else{ // Otherwise, just insert to vaccinated_persons skiplist.
                LinkedList * List=slist->Header;
                LLInsert(List, Id);
                Temp->vaccinated_persons=slist;
            }
        }
        else{ // If the persons that was inserted, hasn't been vaccinated.
            SkipList * slist=Temp->not_vaccinated_persons;
            
            if (slist == NULL){ // If there wasn't any not vaccinated person for the virus, initialize the not_vaccinated_persons skiplist and insert to it.
                slist=SLInit();
                LinkedList * List=slist->Header;
                LLInsert(List, Id);
                Temp->not_vaccinated_persons=slist;
            }
            else{ // Otherwise, just insert to not_vaccinated_persons skiplist.
                LinkedList * List=slist->Header;
                LLInsert(List, Id);
                Temp->not_vaccinated_persons=slist;
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
    NewNode->VirusName=(char *)calloc(strlen(VName), sizeof(char)); strcpy(NewNode->VirusName, VName);
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


void VirusSkipList(Virus ** VList)
{
    Virus * Temp= *VList;
    Temp=Temp->Next;
    
    while (Temp != NULL){
        SLInsert(Temp->vaccinated_persons); 
        SLInsert(Temp->not_vaccinated_persons);
        Temp=Temp->Next;
    }
    
}


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
        SLDestroy(&(Current->vaccinated_persons)); 
        SLDestroy(&(Current->not_vaccinated_persons)); 
        bloomDestroy(&(Current->filter)); 
        free(Current);
        Current=Next;
    }
    return;
}


Country * CountryCreate()
{
    Country * CList=(Country *)calloc(1, sizeof(Country)); CList->Next=NULL; 
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

void CountryInsert(Country ** CList, char * CName, int Id)
{
    Country * Temp=*CList;
    while (Temp->Next != NULL){ 
        Temp=Temp->Next;
    }
    Country * NewNode=(Country *)calloc(1, sizeof(Country));
    NewNode->CName=(char *)calloc(strlen(CName)+1, sizeof(char)); NewNode->Id=Id;
    strcpy(NewNode->CName, FixName(CName)); NewNode->Next=NULL;
    Temp->Next=NewNode;
    return;
}

char * FixName(char * CName)
{
    int i, size=0;
    for ( i = strlen(CName); i > 0; i--){
        if(CName[i]=='/'){
            break;
        }
        size+=1;
    }
    char * Country=(char *)calloc(size+1, sizeof(char));
    for (int j = 0; j < size; j++){
        Country[j]=CName[++i];
    }
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