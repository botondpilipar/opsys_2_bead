#ifndef WORKER_DATABASE_MANAGERS_H
#define WORKER_DATABASE_MANAGERS_H

#include "./database.h"
#include "../etc/pch.h"
#include "./day_enum_conversion.h"

// Creation and deletion

WorkerDatabase create_db(const char* entry_location);

WorkerDatabase initialize_db(const char* entry_location, const char* jobs_required_location);

void cleanup_db(WorkerDatabase* database);

int populate_jobs_required(const char* file, int* days);

int populate_entries(WorkerDatabase* db);

// Copying

void copy_database(WorkerDatabase* dest, WorkerDatabase* src);

// Modification

WorkerEntry lookup(const WorkerDatabase* database, const char*name, const char* address, bool* result);

bool add_entry(WorkerDatabase* database, WorkerEntry* entry);

bool expand_database(WorkerDatabase* database);

int index_lookup(const WorkerDatabase* database, const char* name, const char* address, bool* result);

bool remove_entry(WorkerDatabase* database, int index);

bool modify_entry(WorkerDatabase* database, int modify_at,WorkerEntry* old_entry, WorkerEntry* new_entry);

// Logging

char* log_database(WorkerDatabase* database);

#endif