# include "General.h"

typedef struct MonitorInterface{
    char * VirusName;
    char * CountryName;
    int Accepted;
    int Rejected;
    Date * RequestDate;
    struct MonitorInterface * Next;
}MonitorCheck;

MonitorCheck * MCInit();

void MCInsert(MonitorCheck * MonitorList, char * VName, char * CountryName, bool Accepted, bool Rejected, Date * RequestDate);

void MCPrint(MonitorCheck * MonitorList, char * VName, char * CountryName);

void MCDestroy(MonitorCheck * MonitorList);


/* Takes a virus list and a country list and starts the TTY
 * that gives us the interface that we need in order to use some commands.
 * 
 * Vlist: Virus list. (See Virus.h)
 * CList: Country list. (See Country.h)
 * 
*/
void TTY(Virus * Vlist, Country * Clist);

/* Implemention of the travelRequest command, for the travelmonitor.
 * travelRequest: Takes an id and a virus. Checks at the bloom filter that has been send to it, if the id hasn't been inserted and prints the suitable message.
 *                If it has been inserted, call the travelRequestmonitor(MonitorInterface.h)
 * 
 * Id: Citizens Id that we want to see hers/his vaccination status. In string form
 * VirusName: Name of the virus that we want to check.
 * 
*/
void travelRequest(MonitorCheck * MonitorList , Virus * Vlist, char ** Array, Country * Clist);

/* Implemention of the travelStats command.
 * travelStat: Prints the stats for the request that have been made for travel.
 * 
 * MonitorList: MonitorCheck list.
 * Clist: Country list.
 * VirusName: Name of the virus that we want to check.
 * Date1: from date.
 * Date2: to date.
 * Country: Country name.
 * 
*/
void travelStat(MonitorCheck * MonitorList, Country * Clist, char * VirusName, char * Date1, char * Date2, char * Country);

/* Implemention of the searchVaccinationStatus command.
 * searchVaccinationStatus: Prints the information that we have about a citizen.
 * 
 * Vlist: Virus list.
 * Clist: Country list.
 * Array: array of strings. Contains the command name and the citizen id.
 * 
*/
void searchVaccinationStatus(Virus * Vlist, Country * Clist, char ** Array);
