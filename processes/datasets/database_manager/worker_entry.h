#ifndef WORKER_ENTRY_H
#define WORKER_ENTRY_H

#include "database_macros.h"

#include "./working_days.h"
#include "../etc/pch.h"

typedef struct WorkerEntry
{
    char name[NAME_MAX_LENGTH];
    char address[ADDRESS_MAX_LENGTH];
    WorkDay days_working[WORK_DAYS];
    int number_of_days;
} WorkerEntry;

WorkerEntry create_entry(const char* name, const char* address, WorkDay* days_working, int number_of_days);

bool is_valid_entry(WorkerEntry* entry);
#endif