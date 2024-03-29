# include <time.h>
# include "Fifo.h"
# define NULLstring " "

extern int BloomNum;

/* Struct that implements a date.
*/
typedef struct date{
    int Days;
    int Month;
    int Year;
}Date;

/* Creates a new date.
 *
 * DateStr: String with the date that we want to create.
 *
*/
Date * CreateDate(char * DateStr);

/* Prints a date.
*
* Timing: A date node.
*
*/
void PrintDate(Date * Timing);

/* Checks if a date is before or after an other.
 * If Timing1 is before Timing2 it returns false. Otherwise, it returns true.
 *
 * Timing1 and Timing2: 2 nodes of type date.
 *
*/
bool CheckDate(Date * Timing1, Date * Timing2);

/* Checks if a date is before or after an other.
 * If Timing1 is before Timing2 it returns false. Otherwise, it returns true.
 *
 * Timing1 and Timing2: 2 nodes of type date.
 *
*/
bool CheckDateEq(Date * Timing1, Date * Timing2);

/* Checks if a date is before or after an other.
 * If Timing1 is before Timing2 it returns false. Otherwise, it returns true.
 *
 * Timing1 and Timing2: 2 nodes of type date.
 *
*/
bool CheckDateDiffer(Date * Timing1, Date * Timing2);

/* Struct that implements a hash table.
 * This is the main struct of the program. It keeps all the informations that we have for a citizen.
 *
 * CitizenId: Id of the citizen.
 * FirstName and LastName: This 2 strings, combined, keep the name of the citizen.
 * Country: Name of the Country that they live.
 * Age: Age of citizen.
 * Virus: Name of the virus 
 * Vaccinated: Boolean that represents the vaccination status of the citizen for the virus.
 * Timing: Date that the citizen has been vaccinated, if he/she has been. Otherwise, is NULL.
 *
*/
typedef struct hash{
    int citizenId;
    char * FirstName;
    char * LastName;
    char * Country;
    int Age;
    char * Virus;
    bool Vaccinated;
    Date * Timing;
    struct hash * Next;
}Citizens;

/* Creates the Hash Table
 *
 * Size: Number of records that have been inserted.
 *
*/
void HTCreate(int Size);

/* Makes a new node with the given attributes and returns it.
 *
 * CitizenId: Id of the citizen.
 * FirstName and LastName: This 2 strings, combined, keep the name of the citizen.
 * Country: Name of the Country that they live.
 * Age: Age of citizen.
 * Virus: Name of the virus 
 * Vaccinated: Boolean that represents the vaccination status of the citizen for the virus.
 * DateStr: String that has the date of vaccination.
 *
 * Return: 
 *  If everything goes fine, pointer to citizen.
 *  If there is a problem, NULL.
*/
Citizens * NewRecord(int Id, char * FirstName, char * LastName, char * Country, int Age, char * Virus, bool Vaccinated, char * DateStr);

/* Insert at the hash table a new node. In order to create the new node it uses the NewRecord function.
 * 
 * CitizenId: Id of the citizen.
 * FirstName and LastName: This 2 strings, combined, keep the name of the citizen.
 * Country: Name of the Country that they live.
 * Age: Age of citizen.
 * Virus: Name of the virus 
 * Vaccinated: Boolean that represents the vaccination status of the citizen for the virus.
 * DateStr: String that has the date of vaccination.
 *
 * Return: 
 *  If everything goes fine, 0.
 *  If there is a problem, -1.
*/
int HTInsert(int Id, char * FirstName, char * LastName, char * Country, int Age, char * Virus, bool Vaccinated, char * DateStr);

/* Prints all the nodes of a Table.
*/
void HTPrint();

/* Takes an Id(Item) and a Virus name and searches for the citizen that has this Id and is inserted with this virus.
 *
 * Item: Citizen Id.
 * Virus: Virus that the citizen that we want, has been inserted with.
 *
 * Return: 
 *  If citizen exist, pointer to citizen.
 *  If citizen doesn't exist, NULL.
*/
Citizens * HTSearch(int Item, char * Virus);

/* Takes a Id(Item) and a Virus name and searches for the citizen that has this Id and is inserted with this virus.
 *
 * Item: Citizen Id.
 * 
 * Return: 
 *  If citizen exist, pointer to citizen.
 *  If citizen doesn't exist, NULL.
*/
Citizens * HTSearchID(int Item);

/* Destroy all the hash table.
*/
void HTDestroy();