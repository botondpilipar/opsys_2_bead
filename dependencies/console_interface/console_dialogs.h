#include <database.h>

#define BUFFER_SIZE 1024
#define DEFAULT_DIALOG_OPTION 0
#define NEW_ENTRY_DIALOG_OPTION 1
#define REMOVE_ENTRY_DIALOG_OPTION 2
#define MODIFY_ENTRY_DIALOG_OPTION 3
#define AVAILABLE_JOBS_DIALOG_OPTION 4
#define START_DAY_DIALOG_OPTION 5
#define EXIT_DIALOG_OPTION 6

void
mainDialog(WorkerDatabase* db);

WorkerEntry
newEntryDialog(WorkerDatabase* db, bool* successfull);

int
removeEntryDialog(WorkerDatabase* db);

int
modificationEntryDialog(WorkerDatabase* db, WorkerEntry* target);

void
startDayDialog(WorkerDatabase* database,
               const char* dailyTaskTextFile);
