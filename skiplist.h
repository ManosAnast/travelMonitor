# include "linkedlist.h"

/* Struct that implements a SkipList.
 * 
 * CurrLevel: Max level of current skip list
 * Header: A pointer at the LinkedList (see linkedlist.h) that implements the list and it's levels.
 *
*/
typedef struct skiplist{
    int CurrLevel;
    LinkedList * Header;
}SkipList;

/*Initialize a new SkipList
 *
*/
SkipList * SLInit();

/* Takes a SkipList and it makes all the levels that are needed.
 * 
 * SList: Skiplist.
 * 
*/
void SLInsert(SkipList * Slist);

/* SLInsert calls this function and it does all the "dirty work".
 * 
 * SList: Skiplist.
 * 
*/
void SLInsertHelper(SkipList ** Slist);

/* Takes a SkipList and prints it's levels and contents.
 * 
 * SList: Skiplist.
 * 
*/
void SLPrint(SkipList * Slist);

/* Takes a SkipList and an Id and returns the linkedlist node that corresponds to it.
 * 
 * SList: Skiplist.
 * Id: Citizen id that we want to find.
 * 
*/
LinkedList * SLSearch(SkipList * Slist, int Id);

/* Takes a SkipList and frees all of it's contents.
 * 
 * SList: Skiplist.
 * 
*/
void SLDestroy(SkipList ** SList);

/* Takes a SkipList and an Id and delete the nodes that corresponds to it.
 * 
 * SList: Skiplist.
 * Id: Citizen id that we want to delete.
 * 
*/
void SLDelete(SkipList ** Slist, int Id);

/* Finds the log(base 2) of the number that you give to it.
 *
*/
unsigned int Log(unsigned int n);