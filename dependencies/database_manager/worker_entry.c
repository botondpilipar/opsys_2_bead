#include <pch.h>
#include "worker_entry.h"

// Pure implementation functions

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

    return false;
}

void
rotateWorkDay(size_t firstAfterRotation,
              size_t size,
              WorkDay* days)
{
    // first half of the array until rotate position
    WorkDay firstPart[firstAfterRotation];

    for(size_t i = 0; i<firstAfterRotation; i++)
    {
        firstPart[i] = days[i];
    }
    for(size_t i = firstAfterRotation; i<size; ++i)
    {
        days[i - firstAfterRotation] = days[i];
        days[i] = firstPart[i - firstAfterRotation];
    }
}

// Interface functions
WorkerEntry
createEntry(const char* name,
             const char* address,
             const WorkDay* daysWorking,
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
    if(!entry->isRegistered)
        return true;

    if(entry->numberOfDays > 0 &&
            entry->numberOfDays <= WORK_DAYS)
    {
        return hasOnlyUniqueElements(entry->daysWorking,
                                     entry->numberOfDays);
    }
    return false;
}

bool
canWorkOnDay(const WorkerEntry* entry, WorkDay day)
{
    if(!entry->isRegistered)
    {
        return true;
    }

    bool isAvaliable = false;

    for(size_t i = 0; i<entry->numberOfDays && !isAvaliable; ++i)
    {
        if(day == entry->daysWorking[i])
        {
            isAvaliable = true;
        }
    }

    return isAvaliable;
}

void
sendToWork(WorkerEntry* entry, WorkDay day)
{
    if(!entry->isRegistered)
    {
        entry->numberOfDays = 0;
        return;
    }

    if(canWorkOnDay(entry, day))
    {
        size_t workDayPosition = 0;
        for(size_t i = 0; i<entry->numberOfDays; ++i)
        {
            if(entry->daysWorking[i] == day)
            {
                workDayPosition = i;
            }
        }
        entry->daysWorking[workDayPosition] =
                entry->daysWorking[entry->numberOfDays - 1];
        entry->numberOfDays -= 1;
    }
}
