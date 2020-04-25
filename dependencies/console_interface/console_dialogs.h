#include "../database_manager/database.h"

void main_dialog(WorkerDatabase* db);

WorkerEntry new_entry_dialog(WorkerDatabase* db, bool* successfull);

int remove_entry_dialog(WorkerDatabase* db);

int modification_entry_dialog(WorkerDatabase* db, WorkerEntry* target);