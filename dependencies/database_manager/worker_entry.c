#include <pch.h>
#include "worker_entry.h"

WorkerEntry create_entry(const char* name, const char* address, WorkDay* days_working, int number_of_days)
{
    WorkerEntry entry =
    {
        .number_of_days = number_of_days
    };

    strcpy(entry.name, name);
    strcpy(entry.address, address);

    for(int i = 0; i<number_of_days; ++i)
        entry.days_working[i] = days_working[i];

    return entry;
}

int day_comparator(const void* l, const void* r)
{
    return *(WorkDay*)l - *(WorkDay*)r;
}

bool is_valid_entry(WorkerEntry* entry)
{
    if(entry->number_of_days != 0 &&
        entry->number_of_days <= WORK_DAYS &&
        entry->days_working != NULL &&
        entry->address != NULL &&
        entry->name !=  NULL)
    {
        qsort(entry->days_working, entry->number_of_days, sizeof(WorkDay), day_comparator);

        bool all_unique = true;
        for(int i = 0; i<entry->number_of_days -1 && all_unique; ++i)
        {
            if(entry->days_working[i] == entry->days_working[i+1])
            {
                all_unique = false;
            }
        }
        if(all_unique)
            return true;
        else
            return false;
        
    }
    else
        return false;    
}