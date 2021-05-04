# include "Interface.h"

int M;
Citizens ** Table;

Date * CreateDate(char * DateStr)
{
    Date * Timing=(Date *)calloc(1, sizeof(Date));
    if (!strcmp(DateStr, "Today")){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        Timing->Days=tm.tm_mday; Timing->Month=tm.tm_mon + 1; Timing->Year=tm.tm_year+1900;
        return Timing;
    }
    
    const char * s="-";
    char **Array;
    Array=(char**)malloc(3*sizeof(char *)); // make an arry of strings with 30 characters each string.
    for(int i=0 ; i< 3 ; i++){
        Array[i]=(char*)malloc(50*sizeof(char));
    }
    BreakString(&Array, DateStr, s, 3);
    if (atoi(Array[0])<=30 && atoi(Array[0])>0 && atoi(Array[1])>0 && atoi(Array[1])<=12){
        Timing->Days=atoi(Array[0]); Timing->Month=atoi(Array[1]); Timing->Year=atoi(Array[2]);
        for (int i = 0; i < 3; i++){
            free(Array[i]);
        }
        free(Array);
        return Timing;
    }
    else{
        printf("Wrong Date\n"); 
        for (int i = 0; i < 3; i++){
            free(Array[i]);
        }
        free(Array);
        return NULL;
    }
}

void PrintDate(Date * Timing)
{
    if (Timing != NULL){
        printf("%d-%d-%d\n", Timing->Days, Timing->Month, Timing->Year);
        return;
    }
    printf("\n"); return;
}

bool CheckDate(Date * Timing1, Date * Timing2)
{
    if (Timing1->Year > Timing2->Year){
        return true;                
    }
    else if (Timing1->Year == Timing2->Year){
        if (Timing1->Month > Timing2->Month){
            return true;
        }
        else if (Timing1->Month == Timing2->Month){
            if (Timing1->Days >= Timing2->Days){
                return true;
            }            
        }
    }
    
    return false;
}

void HTCreate(int Size)
{
    M=Size/5;
    Table=(Citizens **)calloc(M, sizeof(Citizens *));
}

Citizens * NewRecord(int Id, char * FirstName, char * LastName, char * Country, int Age, char * Virus, bool Vaccinated, char * DateStr)
{
    Citizens * NewNode=(Citizens *)calloc(1, sizeof(Citizens));
    NewNode->citizenId=Id; NewNode->Age=Age; NewNode->Vaccinated=Vaccinated; NewNode->Next=NULL;
    NewNode->FirstName=(char *)calloc(strlen(FirstName)+1, sizeof(char)); strcpy(NewNode->FirstName, FirstName);
    NewNode->LastName=(char *)calloc(strlen(LastName)+1, sizeof(char)); strcpy(NewNode->LastName, LastName);
    NewNode->Country=(char *)calloc(strlen(Country)+1, sizeof(char)); strcpy(NewNode->Country, Country);
    NewNode->Virus=(char *)calloc(strlen(Virus)+1, sizeof(char)); strcpy(NewNode->Virus, Virus);
    if (!Vaccinated){
        NewNode->Timing=NULL;  return NewNode;
    }
    NewNode->Timing=CreateDate(DateStr);
    if(NewNode->Timing == NULL){
        return NULL;
    }
    return NewNode;
}

// I took it from the book "Algorithms in c" 
int Hash(int Item)
{
    return Item % M;
}

int HTInsert(int Id, char * FirstName, char * LastName, char * Country, int Age, char * Virus, bool Vaccinated, char * DateStr)
{
    int i = Hash(Id);
    if(!strcmp(DateStr, NULLstring)){
        strcpy(DateStr, "Today");
    }
    if(Table[i] == NULL){   //if the table that we want is null make it using the new function
        Table[i]=NewRecord(Id, FirstName, LastName, Country, Age, Virus, Vaccinated, DateStr);
        if (Table[i] == NULL){
            return 0;
        }
        return 1;
    }
    else{
        Citizens * Temp=Table[i];
        while (Temp->Next != NULL){ //find the end of the tables chain
            Temp=Temp->Next;
        }
        Temp->Next=NewRecord(Id, FirstName, LastName, Country, Age, Virus, Vaccinated, DateStr);
        if (Temp->Next == NULL){
            return 0;
        }
        return 1;
    }
}

//Searches for a citizen.If the citizen doesn't return null
// If the citizen do exist return the citizen.
Citizens * HTSearch(int Item, char * Virus)
{
    Citizens * Temp = Table[Hash(Item)];
    if(Temp == NULL){
        return NULL;
    }
    while (Temp != NULL ){
        if (Temp->citizenId == Item && !strcmp(Temp->Virus, Virus)){ // A citizen can have been inserted multiple times. Therefore, I check and the virus.
            return Temp;
        }
        Temp=Temp->Next;
    }
    return NULL;
}

void HTPrint()
{
    for (int i = 0; i < M; i++){
        Citizens * Temp = Table[i];
        printf("- %d %s %s %s %d %s %d ", Temp->citizenId, Temp->FirstName, Temp->LastName, Temp->Country, Temp->Age, Temp->Virus, Temp->Vaccinated); PrintDate(Temp->Timing);
        Temp=Temp->Next;
        while (Temp != NULL ){
            printf("- %d %s %s %s %d %s %d ",Temp->citizenId,Temp->FirstName, Temp->LastName, Temp->Country, Temp->Age, Temp->Virus, Temp->Vaccinated); PrintDate(Temp->Timing);
            Temp=Temp->Next;
        }
    }
    return;
}

void HTDestroy()
{
    for(int i=0 ; i<M ; i++){
        if (Table[i] != NULL){
            if(Table[i]->Next == NULL){ // if the table[i] that we are trying to free doesn't have a list just free the table[i]
                free(Table[i]->FirstName); free(Table[i]->LastName); free(Table[i]->Country); free(Table[i]->Virus); free(Table[i]->Timing); 
                free(Table[i]);
            }
            else{ //if it has. Traverse the list and free every single node.
                Citizens * Current=Table[i];
                Citizens * Next;
                while (Current!= NULL){
                    Next=Current->Next;
                    free(Current->FirstName); free(Current->LastName); free(Current->Country); free(Current->Virus); free(Current->Timing);
                    free(Current);
                    Current=Next;
                }
            }
        }
    }
    free(Table); // Free the public pointer 
    return;
}