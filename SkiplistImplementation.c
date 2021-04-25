# include "skiplist.h"

SkipList * SLInit()
{
    SkipList * SList;
    SList=(SkipList *)calloc(1,sizeof(SkipList));
    SList->CurrLevel=0; SList->Header=LLInit();
    return SList;
}


void SLInsert(SkipList * Slist)
{
    if(Slist!=NULL){
        Slist->CurrLevel=Log( Slist->Header->Id * (-1) ); // Give the currlevel the log of the number of entries that the list has.
        Slist->Header->Id=-1;
        SLInsertHelper(&Slist);
    }
    return;
}


void SLInsertHelper(SkipList ** Slist)
{
    SkipList * STemp= *Slist;
    LinkedList * LTemp= STemp->Header; // LTemp reprsents the previous level of the skiplist.
    LinkedList * LList= STemp->Header; // LList represents the current level of the skiplist.
    srand(time(NULL));   // Initialize for "toss a coin" operation.
    int CurrLevel=1;
    int level=STemp->CurrLevel;
    while (CurrLevel < level){

        while (LTemp != NULL){ // Traverse the previous level list.
            if (rand() % 2){ // If true, add the node to the current level list.
                LList->Next=(LinkedList **)realloc(LList->Next, (CurrLevel+1)*sizeof(LinkedList *));
                LList->Next[CurrLevel]=NULL;
                LList->Next[CurrLevel]=LTemp;
                LList=LList->Next[CurrLevel];
            }
            LTemp=LTemp->Next[CurrLevel-1]; 
        }
        LList->Next=(LinkedList **)realloc(LList->Next, (CurrLevel+1)*sizeof(LinkedList *));
        LList->Next[CurrLevel]=NULL;
        CurrLevel += 1;
        // Give to LTemp and LList the header, in order to start again from the top
        LTemp=STemp->Header;
        LList=LTemp;
    }
    return;
}


LinkedList * SLSearch(SkipList * Slist, int Id)
{
    int MaxLevel=Slist->CurrLevel;
    LinkedList * Temp=Slist->Header;

    for (int i = MaxLevel-1; i >= 0; i--){ // Start from the max level.
        while (Temp->Next[i] != NULL && Temp->Id < Id){ // Traverse the level.
            Temp=Temp->Next[i];
        }
        if (Temp->Id == Id){ // The first time that you find the node that you want, return.
            return Temp;
        }
        Temp=Slist->Header;
    }
    return NULL;
}

void SLDelete(SkipList ** Slist, int Id)
{
    int MaxLevel=(*Slist)->CurrLevel;
    LinkedList * Temp=(*Slist)->Header, * Previous;
    LinkedList * Keeper=SLSearch((*Slist), Id);
    if (Keeper == NULL){
        return;
    }
    
    for (int i = MaxLevel-1; i >= 0; i--){ // Start from the max level.

        while (Temp->Next[i] != NULL && Temp->Id < Id){ // Traverse the level.
            Previous = Temp;
            Temp=Temp->Next[i];
        }
        if (Temp->Id == Id){ // The first time that you find the node that you want break and free the node.
            Previous->Next[i]=Temp->Next[i];
            break;
        } 
        Temp=(*Slist)->Header;
    }
    free(Keeper->Next);
    free(Keeper);
    return;
}

void SLPrint(SkipList * Slist)
{
    if(Slist!= NULL){
        int MaxLevel=Slist->CurrLevel;
        
        for (int i = 0; i < MaxLevel; i++){
            LLPrint(Slist->Header, i);
        }
    }
    return;
}

void SLDestroy(SkipList ** SList)
{
    if(*SList != NULL){
        LLDestroy(&(*SList)->Header);
        free(*SList);
    }
    return;
}

unsigned int Log(unsigned int n) 
{ 
    return (n > 1) ? 1 + Log(n / 2) : 0; 
} 