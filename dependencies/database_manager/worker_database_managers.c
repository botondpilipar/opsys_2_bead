#include "worker_database_managers.h"
#include <pch.h>

// Utility

void logout_worker(WorkerDatabase* database, WorkerEntry* logout)
{
    for(int i = 0; i<logout->number_of_days; ++i)
    {
        database->jobs_avaliable[logout->days_working[i]] += 1;
    }
    return;
}

void login_worker(WorkerDatabase* database, WorkerEntry* login)
{
    for(int i = 0; i<login->number_of_days; ++i)
    {
        database->jobs_avaliable[login->days_working[i]] -= 1;
    }
    return;
}

// Creation and deletion

WorkerDatabase create_db(const char* entry_location)
{
    WorkerDatabase setup = {.entry_mem_size = sizeof(WorkerEntry)};
    setup.entry_capacity = INIT_DATABASE_LENGTH;
    setup.disk_storage = fopen(entry_location, "rb+");
    setup.entries = (WorkerEntry*)calloc(INIT_DATABASE_LENGTH, setup.entry_mem_size);
    setup.entry_number = 0;

    return setup;
}

WorkerDatabase initialize_db(const char* entry_location, const char* jobs_required_location)
{
    WorkerDatabase setup = create_db(entry_location);
    populate_jobs_required(jobs_required_location, setup.jobs_avaliable);
    populate_entries(&setup);
    
    return setup;
}

void cleanup_db(WorkerDatabase* database)
{
    if(database != NULL)
    {
        if(database->entries != NULL)
            free(database->entries);
        if(database->disk_storage != NULL)
            fclose(database->disk_storage);
        return;
    }
    else
        return;
}

int populate_jobs_required(const char* file, int* days)
{
    FILE* jobs_required = fopen(file, "r");

    if(!jobs_required)
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i<WORK_DAYS; ++i)
    {
        days[i] = 0;
    }

    int job_iterator = 0;
    while(!feof(jobs_required) && job_iterator < WORK_DAYS)
    {
        char  num_str [MAX_DIGIT_WORKER_REQUIRED];
        int num_pos = 0;
        
        do
        {
            num_str[num_pos] = fgetc(jobs_required);
        } while (!feof(jobs_required) && num_str[num_pos++] != '\n');
        
        num_str[num_pos] = 0;
        int number_read = atoi(num_str);

        days[job_iterator++] = number_read;
    }

    fclose(jobs_required);
    return EXIT_SUCCESS;
}
int populate_entries(WorkerDatabase* db)
{
    if(db->disk_storage == NULL)
    {
        return EXIT_FAILURE;
    }
    
    fflush(db->disk_storage);
    fseek(db->disk_storage, 0, SEEK_SET);
    size_t successfully_read = 0;

    while(successfully_read = 
        fread(db->entries, db->entry_mem_size,
        db->entry_capacity, db->disk_storage))
    {
        db->entry_number += successfully_read;
        if(db->entry_number == db->entry_capacity)
        {
            expand_database(db);
        }
    }

    successfully_read = db->entry_number;

    for(int i = 0; i<successfully_read; ++i)
    {
        if(is_valid_entry(&db->entries[i]))
        {
            for(int j = 0; j<db->entries[i].number_of_days; ++j)
            {
                --db->jobs_avaliable[db->entries[i].days_working[j]];
            }
        }
        else
            --db->entry_number;
    }
    return EXIT_SUCCESS;
}

// Copying

void copy_database(WorkerDatabase* dest, WorkerDatabase* src)
{
    while(dest->entry_capacity < src->entry_capacity)
    {
        expand_database(dest);
    }

    memcpy(dest->entries, src->entries, src->entry_mem_size*src->entry_number);
    dest->disk_storage = src->disk_storage;
    dest->entry_number = src->entry_number;
    memcpy(dest->jobs_avaliable, src->jobs_avaliable, WORK_DAYS*sizeof(int));
}

// Modification

WorkerEntry lookup(const WorkerDatabase* database, const char*name, const char* address, bool* result)
{
    bool found = false;
    WorkerEntry result_entry;
    for(int i = 0; i<database->entry_number && !found; ++i)
    {
        int name_diff = strcmp(database->entries[i].name, name);
        int address_diff = strcmp(database->entries[i].address, address);
        if(name_diff == 0
            && address_diff == 0)
        {
            found = true;
            result_entry = database->entries[i];
        }
    }
    *result = found;
    return result_entry;
}
bool expand_database(WorkerDatabase* database)
{
    if(database->entry_capacity != database->entry_number)
    {
        return false;
    }
    else
    {
        WorkerEntry* old_entries = database->entries;
        database->entry_capacity = 2*database->entry_capacity + 1;
        database->entries = realloc(old_entries,    (database->entry_capacity)*database->entry_mem_size);
        return true;
    }
}
bool add_entry(WorkerDatabase* database, WorkerEntry* entry)
{
    if(database->entry_number == database->entry_capacity)
        expand_database(database);
    if(!is_valid_entry(entry))
        return false;

    bool no_full_days = true;
    for(int i = 0; i<entry->number_of_days; ++i)
    {
        if(database->jobs_avaliable[entry->days_working[i]] == 0)
        {
            no_full_days = false;
        }
    }

    if(no_full_days)
    {
        for(int i = 0; i<entry->number_of_days; ++i)
        {
            --database->jobs_avaliable[entry->days_working[i]];
        }
        WorkerEntry new_entry;
        memcpy(&new_entry, entry, database->entry_mem_size);
        database->entries[database->entry_number] = new_entry;

        fseek(database->disk_storage, database->entry_number*database->entry_mem_size, SEEK_SET);
        int wrote = fwrite(&new_entry, database->entry_mem_size, 1, database->disk_storage);

        ++database->entry_number;

        return wrote;
    }
    else
        return false;
    
}

int index_lookup(const WorkerDatabase* database, const char* name, const char* address, bool* result)
{
    bool found = false;
    int result_index = 0;
    for(int i = 0; i<database->entry_number && !found; ++i)
    {
        int name_diff = strcmp(database->entries[i].name, name);
        int address_diff = strcmp(database->entries[i].address, address);
        if(name_diff == 0
            && address_diff == 0)
        {
            found = true;
            result_index = i;
        }
    }
    *result = found;
    return result_index;
}

bool remove_entry(WorkerDatabase* database, int collapse_position)
{
    if(collapse_position >= database->entry_number || collapse_position < 0)
        return false;

    logout_worker(database, &database->entries[collapse_position]);

    WorkerEntry* last = &database->entries[database->entry_number - 1];
    size_t last_byte_position = (database->entry_number - 1)*database->entry_mem_size;

    if(collapse_position != database->entry_number - 1)
    {
        fseek(database->disk_storage, database->entry_mem_size*collapse_position, SEEK_SET);
        fwrite(last, database->entry_mem_size, 1, database->disk_storage);
        database->entries[collapse_position] = *last;
    }

    last->number_of_days = 0;
    fseek(database->disk_storage, last_byte_position, SEEK_SET);
    fwrite(last, database->entry_mem_size, 1, database->disk_storage);
    database->entry_number--;

    return true;
}

bool modify_entry(WorkerDatabase* database, int modify_at, WorkerEntry* old_entry, WorkerEntry* new_entry)
{
    if(modify_at < 0 || modify_at >= database->entry_number)
        return false;

    logout_worker(database, old_entry);
    login_worker(database, new_entry);

    fseek(database->disk_storage, database->entry_mem_size*modify_at, SEEK_SET);
    int wrote = fwrite(new_entry, database->entry_mem_size, 1, database->disk_storage);
    if(wrote != 1)
        return false;
    database->entries[modify_at] = *new_entry;

    return true;
}

char* log_database(WorkerDatabase* database)
{
    printf("Elérhető munkák: ");
    for(int i = 0; i < 7; ++i)
    {
        const char* day = from_workday(database->jobs_avaliable[i]);
    }
}