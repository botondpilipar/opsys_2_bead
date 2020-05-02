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
             const WorkDay* daysWorking,
             size_t numberOfDays,
             bool isRegistered);

/**
 * @brief Determines whether parameter is valid logically
 * @param entry WorkerEntry struct
 * @return Returns true if entry is logically valid, false otherwise
 */
bool
isValidEntry(WorkerEntry* entry);


/**
 * @brief Determines whether worker has the parameter day
 * in their array of days
 * @param entry
 * @param day Working day from enum
 * @return If worker is unregistered, the function returns ture
 * If the worker is registered, the returns true if the day
 * parameter is found among worker's days.
 */
bool
canWorkOnDay(const WorkerEntry* entry,
             WorkDay day);

/**
 * @brief Sends worker to work by removing the day from its
 * array of registered days
 * @param entry Worker the function sends to work
 * If the worker is unregistered, it will simply change its state
 * for later to be cleaned up by handler function.
 * @param day The day which should be removed it present
 */
void
sendToWork(WorkerEntry* entry, WorkDay day);

#endif
