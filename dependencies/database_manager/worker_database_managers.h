#ifndef WORKER_DATABASE_MANAGERS_H
#define WORKER_DATABASE_MANAGERS_H

#include "database.h"
#include <pch.h>
#include "day_enum_conversion.h"

// Creation and deletion

/**
 * @brief create_db
 * @param entry_location
 * @return
 */
WorkerDatabase
createDb(const char* entryLocation);

WorkerDatabase
initializeDb(const char* entry_location,
             const char* jobs_required_location);

void
cleanupDb(WorkerDatabase* database);

int
populateJobsRequired(const char* file,
                     int* days);

int
populateEntries(WorkerDatabase* db);

// Copying

void
copyDatabase(WorkerDatabase* dest,
             WorkerDatabase* src);

// Modification

WorkerEntry
lookup(const WorkerDatabase* database,
       const char*name,
       const char* address,
       bool* result);

bool
addEntry(WorkerDatabase* database,
         WorkerEntry* entry);

bool
expandDatabase(WorkerDatabase* database);

int
indexLookup(const WorkerDatabase* database,
            const char* name,
            const char* address,
            bool* result);

bool
removeEntry(WorkerDatabase* database,
            int index);

bool
modifyEntry(WorkerDatabase* database,
            int modify_at,WorkerEntry* old_entry,
            WorkerEntry* new_entry);

// Logging

char*
logDatabase(WorkerDatabase* database);

#endif


