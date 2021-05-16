# include "Interface.h"

LinkedList * LLInit()
{
    LinkedList * New=(LinkedList *)malloc(sizeof(LinkedList));
    New->Id=0; New->Next=NULL;
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
    while (Temp->Next != NULL && Temp->Id < Key){
        Previous = Temp;
        Temp=Temp->Next;
    }
    /*Make a new node*/
    NewNode=(LinkedList *)calloc(1, sizeof(LinkedList));
    NewNode->Id=Key;

    /*Connect the new node to the correct position*/
    if(Temp->Next == NULL && Temp->Id < Key){ // The new node becomes the last node.
        NewNode->Next=NULL;
        Temp->Next=NewNode;
    }
    else if (Temp->Id > Key){        
        Previous->Next=NewNode;
        NewNode->Next=Temp;
    }
    return;
}

LinkedList * LLSearch(LinkedList * List, int Item)
{
    LinkedList * Temp=List;
    while (Temp != NULL){
        if (Temp->Id == Item){
            return Temp;
        }
        Temp=Temp->Next;
    }
    return NULL;
}

void LLPrint(LinkedList * List, int Level)
{
    LinkedList * Temp=List;
    while (Temp != NULL){
        printf("%d",Temp->Id);
        Temp=Temp->Next;
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
        Next=Current->Next;
        free(Current);
        Current=Next;
    }
    return;
}