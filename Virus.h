# include "skiplist.h"

/* Struct that implements a virus list. 
 * It keeps all the necessary informations that we need for every virus that gets inserted.
 *
 * VirusName: name virus.
 * vaccinated_persons: skiplist that keeps all the vaccinated citizens.
 * not_vaccianted_persons: skiplist that keeps all the citizens that haven't been vaccinated.
 * filter: bloom filter that keeps all the citizens that have been vaccinated for this virus.
 *
*/
typedef struct Interface{
    char * VirusName;
    SkipList * vaccinated_persons;
    SkipList * not_vaccinated_persons;
    bloom filter;
    struct Interface * Next;
}Virus;


/* Creates a virus list and returns a pointer to it.
 * 
*/
Virus * VirusInit();

/* Takes a country and inserts it to the country list.
 * 
 * VList: virus list.
 * CitzenId: citizens id (string type).
 * VName: Name of the virus we want to insert.
 * Vaccinated: boolean for the vaccinated status. true or false.
 * DateStr: Date string.
 * 
*/
void VirusInsert(Virus ** VList, char * CitizenId, char * VName, bool Vaccinated, char * DateStr);

void VirusInsertBloom(Virus ** VList, char * VName, bloom filter);

int VirusCount(Virus * VList);

/* Takes a virus and makes the skiplist for every skiplist that the virus has.
 * It makes all the levels that are needed.
 * 
 * VList: Virus list.
 * 
*/
void VirusSkipList(Virus ** Vlist);

/* Takes a virus list and deletes all of its nodes.
 * 
 * VList: Virus list.
 * 
*/
void VirusDestroy(Virus ** VList);

/* Takes a virus, search for it and returns the node.
 * 
 * VList: Virus list.
 * VirusName: Name of the virus that we want to search.
 * 
*/
Virus * VirusFind(Virus * Vlist, char * VirusName);

void VirusPrint(Virus * Vlist);