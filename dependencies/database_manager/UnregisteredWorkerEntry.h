#ifndef UNREGISTERE_WORKER_ENTRY_H
#define UNREGISTERE_WORKER_ENTRY_H

#include <pch.h>
#include "database_macros.h"

typedef struct UnregisteredWorkerEntry
{
    char name[NAME_MAX_LENGTH];
    char address [ADDRESS_MAX_LENGTH];
} UnregisteredWorkerEntry;

UnregisteredWorkerEntry
createdUnregisteredWorker(const char* name, const char* address);

#endif
