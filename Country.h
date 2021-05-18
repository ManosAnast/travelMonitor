# include "serialize.h"

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

/* Takes an input dir name and returns the countries name.
 * 
 * CName: input dir name.
 * 
*/
char * FixName(char * CName);

/* Takes a country list and a country name and returns the id that coresponds to that country name.
 * 
 * CList: Country list.
 * Name: Name of the country that we want to insert.
 * 
 * Returns:
 *  If the country name exists, the monitorid.
 *  If the countru name doesn't exists, -1.
*/
int CountryId(Country * CList, char * CName);

/* Delete all country lists nodes.
 * 
*/
void CountryDestroy(Country ** CList);
