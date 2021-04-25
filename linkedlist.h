# include "bloomfilter.h"


/* Struct that implements a LinkedList.
 * 
 * Id: HashId that is used to find the citizen and hers/his informations.
 * Next: Array of pointers that "shows" to the next populated node.
 *
*/
typedef struct node{
    int Id;
    struct node ** Next;
}LinkedList;

/*Initialize a new LinkedList
 *
*/
LinkedList * LLInit();

/* Takes a LinkedList and a Key that we want to insert and inserts it to the linkedlist.
 * 
 * List: LinkedList.
 * Key: Id of the citizen that we want to insert.
 * 
*/
void LLInsert(LinkedList * List, int Key);

/* LLInsert calls this function and it does all the "dirty work".
 * 
 * List: LinkedList.
 * Key: Id of the citizen that we want to insert.
 * 
*/
void LLInsertHelper(LinkedList ** List, int Key);

/* Takes a LinkedList and a Key and delete the nodes that corresponds to it.
 * 
 * List: LinkedList.
 * Key: Citizen id that we want to delete.
 * 
*/
void LLDelete(LinkedList * List, int Key);

/* Takes a LinkedList and a Level and prints the contents of the list for the given level.
 * 
 * List: Skiplist.
 * Level: The level of the linked list that we want to print
 * 
*/
void LLPrint(LinkedList * List, int Level);

/* Takes a LinkedList and frees all of it's contents.
 * 
 * List: LinkedList.
 * 
*/
void LLDestroy(LinkedList ** List);