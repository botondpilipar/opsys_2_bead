
#ifndef DATABASE_H
#define DATABASE_H
#include <pch.h>
#include "worker_entry.h"

typedef struct WorkerDatabase
{
    WorkerEntry* entries;
    int jobs_avaliable[WORK_DAYS];
    size_t entryNumber;
    size_t entryCapacity;
    const size_t entryMemorySize;
    FILE* diskStorage;
} WorkerDatabase;

#endif
