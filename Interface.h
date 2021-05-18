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


/* Implemention of the vaccinateStatusBloom command.
 * vaccinationStatusBloom: Takes an id and a virus. Checks at the virus bloomfilter if the id has been inserted and prints the suitable message.
 * 
 * Vlist: Virus list. (See Virus.h)
 * Id: Citizens Id that we want to see hers/his vaccination status. In string form
 * VirusName: Name of the virus that we want to check.
 * 
*/
void VaccinateStatusBloom(Virus * Vlist, char * Id, char * VirusName);

void travelRequest(MonitorCheck * MonitorList , Virus * Vlist, char ** Array, Country * Clist);

void travelStat(MonitorCheck * MonitorList, Country * Clist, char * VirusName, char * Date1, char * Date2, char * Country);

void searchVaccinationStatus(Virus * Vlist, Country * Clist, char ** Array);
