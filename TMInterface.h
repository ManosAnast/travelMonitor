# include "TMCommands.h"

typedef struct TMInterface{
    char * VirusName;
    int Accepted;
    int Rejected;
    struct TMInterface * Next;
}MonitorCheck;

void MCInit(MonitorCheck * MonitorList);

void MCInsert(MonitorCheck * MonitorList, char * VName, bool Accepted, bool Rejected);

void MCPrint(MonitorCheck * MonitorList, char * VName);

void MCDestroy(MonitorCheck * MonitorList);