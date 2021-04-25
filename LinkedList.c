# include "linkedlist.h"

LinkedList * LLInit()
{
    LinkedList * New=(LinkedList *)malloc(sizeof(LinkedList));
    New->Id=0; New->Next=(LinkedList **)calloc(1, sizeof(LinkedList *));
    return New;
}


void LLInsert(LinkedList * List, int Key)
{
    LLInsertHelper(&List, Key);
    return;
}


void LLInsertHelper(LinkedList ** List, int Key) // It is used only for the 0th level.
{
    LinkedList * Temp= *List, * Previous, * NewNode;
    Temp->Id -= 1;
    while (Temp->Next[0] != NULL && Temp->Id < Key){
        Previous = Temp;
        Temp=Temp->Next[0];
    }
    /*Make a new node*/
    NewNode=(LinkedList *)calloc(1, sizeof(LinkedList));
    NewNode->Next=(LinkedList **)calloc(1, sizeof(LinkedList *));
    NewNode->Id=Key;

    /*Connect the new node to the correct position*/
    if(Temp->Next[0] == NULL && Temp->Id < Key){ // The new node becomes the last node.
        NewNode->Next[0]=NULL;
        Temp->Next[0]=NewNode;
    }
    else if (Temp->Id > Key){        
        Previous->Next[0]=NewNode;
        NewNode->Next[0]=Temp;
    }
    return;
}

void LLPrint(LinkedList * List, int Level)
{
    LinkedList * Temp=List;
    while (Temp != NULL){
        printf("%d",Temp->Id);
        Temp=Temp->Next[Level];
        if (Temp != NULL){
            printf(" -> ");
        }
    }
    printf("\n");
    return;
}

void LLDestroy(LinkedList ** List)
{
    LinkedList * Current=*List, *Next;
    while (Current!= NULL){
        Next=Current->Next[0];
        free(Current->Next);
        free(Current);
        Current=Next;
    }
    return;
}