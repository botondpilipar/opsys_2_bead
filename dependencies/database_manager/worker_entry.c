#include <pch.h>
#include "worker_entry.h"

// Pure implementational functions

unsigned int
day_comparator(void* l, void* r)
{
    return *(WorkDay*)l - *(WorkDay*)r;
}

int
memoryComparator(const void* left, const void* right)
{
    return memcmp(left, right, sizeof (left));
}

bool
hasOnlyUniqueElements(int* elements, size_t size)
{
    qsort(elements, size, sizeof (int), &memoryComparator);

    bool all_unique = true;
    for(size_t i = 0; i < size - 1 && all_unique; ++i)
    {
        if(elements[i] == elements[i+1])
        {
            all_unique = false;
        }
    }
    if(all_unique)
        return true;
    else
        return false;
}

// Interface functions
WorkerEntry
createEntry(const char* name,
             const char* address,
             WorkDay* daysWorking,
             size_t numberOfDays,
             bool isRegistered)
{
    WorkerEntry entry =
    {
        .numberOfDays = numberOfDays,
        .isRegistered = isRegistered
    };

    strcpy(entry.name, name);
    strcpy(entry.address, address);
    memcpy(entry.daysWorking, daysWorking, numberOfDays*sizeof (WorkDay));

    return entry;
}

bool isValidEntry(WorkerEntry* entry)
{
    if(entry->isRegistered)
        return true;
    else if(entry->numberOfDays > 0 &&
            entry->numberOfDays <= WORK_DAYS)
    {
        return hasOnlyUniqueElements(entry->daysWorking,
                                     entry->numberOfDays);
    }
    else
        return false;    
}


