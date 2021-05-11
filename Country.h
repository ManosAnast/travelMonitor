# include "Virus.h"

/* Struct that implements a country list.
 * It keeps the population for the country.
 * 
 * CName: country name.
 * Population: number of citizens that have been inserted for the country.
 *
*/
typedef struct Country{
    char * CName;
    int Id;
    int pid;
    struct Country * Next;
}Country;


/* Creates a country list and returns a pointer to it.
 * 
*/
Country * CountryCreate();

/* Takes a country, search for it and returns the node.
 * 
 * CList: Country list.
 * Name: Name of the country that we want to search.
 * 
*/
Country * CountrySearch(Country * CList, char * Name);

/* Takes a country and inserts it to the country list.
 * 
 * CList: Country list.
 * Name: Name of the country that we want to insert.
 * 
*/
void CountryInsert(Country ** CList, char * CName, int Id, int pid);

char * FixName(char * CName);

int CountryId(Country * CList, char * CName);

char * CountryFind(Country * CList, int monitorId);

/* Delete all the country lists nodes.
 * 
*/
void CountryDestroy(Country ** CList);
