#ifndef WORKER_ENTRY_H
#define WORKER_ENTRY_H

#include <pch.h>

#include "database_macros.h"
#include "working_days.h"


typedef struct WorkerEntry
{
    char name[NAME_MAX_LENGTH];
    char address[ADDRESS_MAX_LENGTH];
    WorkDay daysWorking[WORK_DAYS];
    size_t numberOfDays;
    bool isRegistered;
} WorkerEntry;

/**
 * @brief Creates a WorkerEntry with every parameter filled
 * @param name  Worker name
 * @param address  Worker address
 * @param daysWorking  Array of WorkDay enumeration containing the days
 * the worker is willing to work
 * @param numberOfDays  Total number of days, length of daysWorking array
 * @param isRegistered  It is possible to add a worker who is not registered
 * to any particular day, thus daysWorking and numberOfDays can be invalid
 * @details Every sensitive data is copied
 * @return Created WorkerEntry
 */
WorkerEntry
createEntry(const char* name,
             const char* address,
             WorkDay* daysWorking,
             size_t numberOfDays,
             bool isRegistered);

/**
 * @brief Determines whether parameter is valid logically
 * @param entry
 * @return
 */
bool
isValidEntry(WorkerEntry* entry);



#endif
