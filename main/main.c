#include <pch.h>
#include <worker_database_managers.h>
#include <console_dialogs.h>

int main()
{
    WorkerDatabase db = initializeDb("resources/entries.bin","resources/employed_per_day.txt");

    mainDialog(&db);

    return 0;
}
