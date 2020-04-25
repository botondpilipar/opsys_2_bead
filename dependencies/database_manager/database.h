
#ifndef DATABASE_H
#define DATABASE_H
#include <pch.h>
#include "worker_entry.h"

typedef struct WorkerDatabase
{
    WorkerEntry* registeredEntries;
    WorkerEntry* unregisteredEntries;
    int jobs_avaliable[WORK_DAYS];
    size_t registeredEntryNumber;
    size_t registeredEntryCapacity;
    size_t unregisteredEntryNumber;
    size_t unregisteredEntryCapacity;
    const size_t entryMemorySize;
    FILE* diskStorage;
} WorkerDatabase;


/*
typedef struct AbstractLinearDatabase
{
    void* entries;
    Constraint constraint;
    size_t entryCapacity;
    const size_t entryMemorySize;
    FILE* storage;
    sem_t semaphore;
} AbstractLinearDatabase;

typedef struct WorkerEntryConstraintObject
{
    unsigned int* employed_per_day;
    size_t size;
} WorkerEntryConstraintObject;


typedef struct Constraint
{
    bool (*predicate)(void*);
    void* constraintObject;
} Constraint;

*/

#endif
