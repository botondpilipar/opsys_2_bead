#include "worker_database_managers.h"
#include <pch.h>

// Utility

void logout_worker(WorkerDatabase* database, WorkerEntry* logout)
{
    if(logout->isRegistered)
    {
        for(size_t i = 0; i<logout->numberOfDays; ++i)
        {
            database->jobs_avaliable[logout->daysWorking[i]] += 1;
        }
    }
}

void login_worker(WorkerDatabase* database, WorkerEntry* login)
{
    if(login->isRegistered)
    {
        for(size_t i = 0; i<login->numberOfDays; ++i)
        {
            database->jobs_avaliable[login->daysWorking[i]] -= 1;
        }
    }
}

// Creation and deletion

WorkerDatabase
createDb(const char* entryLocation)
{
    WorkerDatabase setup =
    {
        .entryMemorySize = sizeof(WorkerEntry)
    };
    setup.entryCapacity = INIT_DATABASE_LENGTH;
    setup.diskStorage = fopen(entryLocation, "rb+");
    setup.entries =
            (WorkerEntry*)calloc(INIT_DATABASE_LENGTH, setup.entryMemorySize);
    setup.entryNumber = 0;

    return setup;
}

WorkerDatabase initializeDb(const char* entry_location, const char* jobs_required_location)
{
    WorkerDatabase setup = createDb(entry_location);
    populateJobsRequired(jobs_required_location, setup.jobs_avaliable);
    memcpy(setup.jobs_required, setup.jobs_avaliable, WORK_DAYS*sizeof(int));
    populateEntries(&setup);
    
    return setup;
}

void cleanupDb(WorkerDatabase* database)
{
    if(database != NULL)
    {
        if(database->entries != NULL)
            free(database->entries);
        if(database->diskStorage != NULL)
            fclose(database->diskStorage);
    }
}

int populateJobsRequired(const char* file, int* days)
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
int populateEntries(WorkerDatabase* db)
{
    if(db->diskStorage == NULL)
    {
        return EXIT_FAILURE;
    }
    
    fflush(db->diskStorage);
    fseek(db->diskStorage, 0, SEEK_SET);
    size_t successfully_read = 0;

    while(successfully_read = 
        fread(db->entries, db->entryMemorySize,
        db->entryCapacity, db->diskStorage))
    {
        db->entryNumber += successfully_read;
        if(db->entryNumber == db->entryCapacity)
        {
            expandDatabase(db);
        }
    }

    for(size_t i = 0; i<db->entryNumber; ++i)
    {
        bool validEntry = isValidEntry(&db->entries[i]);
        bool isRegisteredEntry = db->entries[i].isRegistered;

        if(isValidEntry && isRegisteredEntry)
        {
            login_worker(db, &db->entries[i]);
        }
        else if(!validEntry)
        {
            --db->entryNumber;
        }

    }
    return EXIT_SUCCESS;
}

// Copying

void copyDatabase(WorkerDatabase* dest, WorkerDatabase* src)
{
    while(dest->entryCapacity < src->entryCapacity)
    {
        expandDatabase(dest);
    }

    memcpy(dest->entries, src->entries, src->entryMemorySize*src->entryNumber);
    dest->diskStorage = src->diskStorage;
    dest->entryNumber = src->entryNumber;
    memcpy(dest->jobs_avaliable, src->jobs_avaliable, WORK_DAYS*sizeof(int));
}

// Modification

WorkerEntry lookup(const WorkerDatabase* database, const char*name, const char* address, bool* result)
{
    bool found = false;
    WorkerEntry result_entry;
    for(size_t i = 0; i<database->entryNumber && !found; ++i)
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
bool expandDatabase(WorkerDatabase* database)
{
    if(database->entryCapacity != database->entryNumber)
    {
        return false;
    }

    WorkerEntry* old_entries = database->entries;
    database->entryCapacity = 2*database->entryCapacity + 1;
    database->entries = realloc(old_entries,    (database->entryCapacity)*database->entryMemorySize);
    return true;
}
bool addEntry(WorkerDatabase* database, WorkerEntry* entry)
{
    if(database->entryNumber == database->entryCapacity)
        expandDatabase(database);
    if(!isValidEntry(entry))
        return false;

    bool no_full_days = true;
    if(entry->isRegistered)
    {
        for(size_t i = 0; i<entry->numberOfDays; ++i)
        {
            if(database->jobs_avaliable[entry->daysWorking[i]] == 0)
            {
                no_full_days = false;
            }
        }
    }

    if(no_full_days)
    {
        login_worker(database, entry);
        WorkerEntry new_entry;
        memcpy(&new_entry, entry, database->entryMemorySize);
        database->entries[database->entryNumber] = new_entry;

        fseek(database->diskStorage, database->entryNumber*database->entryMemorySize, SEEK_SET);
        int wrote = fwrite(&new_entry, database->entryMemorySize, 1, database->diskStorage);

        ++database->entryNumber;

        return wrote;
    }

    return false;
    
}

int indexLookup(const WorkerDatabase* database, const char* name, const char* address, bool* result)
{
    bool found = false;
    int result_index = 0;
    for(int i = 0; i<database->entryNumber && !found; ++i)
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

bool removeEntry(WorkerDatabase* database, int collapse_position)
{
    if(collapse_position >= database->entryNumber || collapse_position < 0)
        return false;

    logout_worker(database, &database->entries[collapse_position]);

    WorkerEntry* last = &database->entries[database->entryNumber - 1];
    size_t last_byte_position = (database->entryNumber - 1)*database->entryMemorySize;

    if(collapse_position != database->entryNumber - 1)
    {
        fseek(database->diskStorage, database->entryMemorySize*collapse_position, SEEK_SET);
        fwrite(last, database->entryMemorySize, 1, database->diskStorage);
        database->entries[collapse_position] = *last;
    }

    last->numberOfDays = 0;
    fseek(database->diskStorage, last_byte_position, SEEK_SET);
    fwrite(last, database->entryMemorySize, 1, database->diskStorage);
    database->entryNumber--;

    return true;
}

bool modifyEntry(WorkerDatabase* database, int modify_at, WorkerEntry* old_entry, WorkerEntry* new_entry)
{
    if(modify_at < 0 || modify_at >= database->entryNumber)
        return false;

    if(new_entry->isRegistered && new_entry->numberOfDays == 0)
    {
        return removeEntry(database, modify_at);
    }

    logout_worker(database, old_entry);
    login_worker(database, new_entry);

    fseek(database->diskStorage, database->entryMemorySize*modify_at, SEEK_SET);
    int wrote = fwrite(new_entry, database->entryMemorySize, 1, database->diskStorage);
    if(wrote != 1)
        return false;
    database->entries[modify_at] = *new_entry;

    return true;
}

unsigned int
requestWorkers(WorkerDatabase* database,
               WorkerEntry* destination,
               unsigned int maxNumber,
               WorkDay day,
               bool isRegistered)
{
    int destinationIndexer = 0;
    for(unsigned i = 0;
        i < database->entryNumber
            && destinationIndexer < maxNumber; ++i)
    {
        if(database->entries[i].isRegistered == isRegistered
            && canWorkOnDay(&database->entries[i], day))
        {
            WorkerEntry workEntry = database->entries[i];
            if(database->entries[i].isRegistered == false)
            {
                removeEntry(database, i);
            }
            else
            {
                sendToWork(&workEntry, day);
                modifyEntry(database, i, &database->entries[i], &workEntry);
            }
            destination[destinationIndexer] = workEntry;
            destinationIndexer += 1;
            i = 0;
        }
    }
    return destinationIndexer;
}
