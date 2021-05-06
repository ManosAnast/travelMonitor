# include "serialize.h"

typedef struct TMInterface{
    char * VirusName;
    char * CountryName;
    int Accepted;
    int Rejected;
    Date * RequestDate;
    struct TMInterface * Next;
}MonitorCheck;

MonitorCheck * MCInit();

void MCInsert(MonitorCheck * MonitorList, char * VName, char * CountryName, bool Accepted, bool Rejected, Date * RequestDate);

void MCPrint(MonitorCheck * MonitorList, char * VName, char * CountryName);

void MCDestroy(MonitorCheck * MonitorList);