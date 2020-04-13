
#ifndef DATABASE_H
#define DATABASE_H
#include <pch.h>
#include "worker_entry.h"

typedef struct WorkerDatabase
{
    WorkerEntry* entries;
    int jobs_avaliable[WORK_DAYS];
    size_t entry_number;
    int entry_capacity;
    const size_t entry_mem_size;
    FILE* disk_storage;
} WorkerDatabase;
#endif