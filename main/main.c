#include <pch.h>
#include <worker_database_managers.h>
#include <console_dialogs.h>

int main()
{
    WorkerDatabase db = initializeDb("resources/entries.bin","resources/employed_per_day.txt");

    main_dialog(&db);

    return 0;
}
