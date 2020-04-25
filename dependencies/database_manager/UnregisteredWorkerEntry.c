#include "UnregisteredWorkerEntry.h"

UnregisteredWorkerEntry
createdUnregisteredWorker(const char* name, const char* address)
{
    UnregisteredWorkerEntry entry;

    if(name != NULL)
        strcpy(entry.name, name);
    if(address != NULL)
        strcpy(entry.address, address);
    return entry;
}
