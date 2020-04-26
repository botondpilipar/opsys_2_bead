#include <database.h>

#define BUFFER_SIZE 1024
#define DEFAULT_DIALOG_OPTION 0
#define NEW_ENTRY_DIALOG_OPTION 1
#define REMOVE_ENTRY_DIALOG_OPTION 2
#define MODIFY_ENTRY_DIALOG_OPTION 3
#define AVAILABLE_JOBS_DIALOG_OPTION 4
#define EXIT_DIALOG_OPTION 5

void main_dialog(WorkerDatabase* db);

WorkerEntry new_entry_dialog(WorkerDatabase* db, bool* successfull);

int removeEntry_dialog(WorkerDatabase* db);

int modification_entry_dialog(WorkerDatabase* db, WorkerEntry* target);
