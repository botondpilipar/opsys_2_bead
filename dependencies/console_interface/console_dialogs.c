#include "console_dialogs.h"
#include "input_handlers.h"
#include <worker_database_managers.h>
#include <day_enum_conversion.h>
#include <worker_entry.h>
#include <Processes.h>

#define WORKER_ENTRY_BINARY_FILE "resources/entries.bin"
#define DAILY_TASK_TEXT_FILE "resources/vineyard_tasks.txt"

void display_main_menu(void)
{
    printf("[1] Új munkajelentkezés\n");
    printf("[2] Megléve adatok törlése\n");
    printf("[3] Adatok módosítása\n");
    printf("[4] Elérhető munkák naponta\n");
    printf("[5] Nap indítása\n");
    printf("[6] Kilépés\n");
    printf("Menüpont: >> ");
}

void print_avaliable_jobs(WorkerDatabase* db)
{
    printf("Elérhető munkák:\n");
    for(int i = 0; i<WORK_DAYS; ++i)
    {
        printf("%s ", from_workday(i));
        printf("%d\n", db->jobs_avaliable[i]);
    }
}

void mainDialog(WorkerDatabase* db)
{
    char input_buffer[BUFFER_SIZE];
    int menu_option = 0;

    do
    {
        display_main_menu();

        fgets(input_buffer, BUFFER_SIZE, stdin);
        int success = sscanf(input_buffer, "%d", &menu_option);
        while(!success
             && strlen(input_buffer) > 0)
        {
            printf("Hibás bemenet, próbálja újra\n");
            display_main_menu();
            fgets(input_buffer, BUFFER_SIZE, stdin);
        }

        switch (menu_option)
        {
            case DEFAULT_DIALOG_OPTION:
                break;
            case NEW_ENTRY_DIALOG_OPTION:
            {
                bool new_entry_success = false;
                WorkerEntry entry = newEntryDialog(db, &new_entry_success);
                if(new_entry_success)
                {
                    bool added = addEntry(db, &entry);
                    if(added)
                        printf("Sikeres művelet!\n");
                    else
                        printf("Helytelen bejegyzés!\n");
                }
                printf("Visszatérés a főmenübe\n");
            }
                break;
            case REMOVE_ENTRY_DIALOG_OPTION:
            {   
                int remove_at = removeEntryDialog(db);
                if(remove_at < 0)
                    printf("Sikertelen művelet!\n");
                else
                {
                    bool removed = removeEntry(db, remove_at);
                    if(removed)
                        printf("Sikeres eltávolítás az adatbázisból\n");
                    else
                        printf("Nem sikerült az eltávolítás\n");
                }
                printf("Visszatérés a főmenübe\n");
            }
                break;
            case MODIFY_ENTRY_DIALOG_OPTION:
            {
                char name [NAME_MAX_LENGTH];
                char address [ADDRESS_MAX_LENGTH];
                WorkDay days [WORK_DAYS];
                WorkerEntry start_entry = createEntry(name, address, days, 0, true);
                int modify_at = modificationEntryDialog(db, &start_entry);
                
                if(modify_at >= 0)
                {
                    if(modifyEntry(db, modify_at, &db->entries[modify_at], &start_entry))
                        printf("Sikeres művelet\n");
                    else
                        printf("Sikertelen művelet\n");
                } else {
                    printf("Sikertelen művelet\n");
                }
                printf("Visszatérés a főmenübe\n");
            }
                break;
            case AVAILABLE_JOBS_DIALOG_OPTION:
            {
                print_avaliable_jobs(db);
            }
                break;
            case START_DAY_DIALOG_OPTION:
            {
                startDayDialog(db, DAILY_TASK_TEXT_FILE);
            }
                break;
            case EXIT_DIALOG_OPTION:
                break;
            default:
                printf("Nem olvasható bemenet, kérem próbálja újra\n");
                break;
        }
    } while (menu_option != DEFAULT_DIALOG_OPTION && menu_option != EXIT_DIALOG_OPTION);
}
    

WorkerEntry newEntryDialog(WorkerDatabase* db, bool* successfull)
{
    char name [NAME_MAX_LENGTH];
    char address [ADDRESS_MAX_LENGTH];
    WorkDay daysWorking[WORK_DAYS];
    int numberOfDays = 0;
    bool isRegistered = false;

    bool name_success = handle_name(name);
    bool address_success = handle_address(address);
    bool registeredSuccess = handle_registered(&isRegistered);
    bool daysWorkingSuccess = true;

    if(isRegistered)
    {
        daysWorkingSuccess = handle_daysWorking(daysWorking, &numberOfDays);
    }
    *successfull = name_success && address_success && registeredSuccess && daysWorkingSuccess;
    if(*successfull)
    {
        bool already_in_db = false;
        indexLookup(db, name, address, &already_in_db);
        if(already_in_db)
            printf("A bejegyzés már az adatbázisban van.\n");
        else
            return createEntry(name, address, daysWorking, numberOfDays, isRegistered);
    }

    return createEntry("", "", NULL, 0, false);
}

int removeEntryDialog(WorkerDatabase* db)
{
    char name [NAME_MAX_LENGTH];
    char address [ADDRESS_MAX_LENGTH];
    bool entry_found = false;

    bool name_success = handle_name(name);
    bool address_success = handle_address(address);

    if(name_success && address_success)
    {
        int result =  indexLookup(db, name, address, &entry_found);
        if(entry_found)
            return result;
    }
    return -1;
}

int modificationEntryDialog(WorkerDatabase* db, WorkerEntry* target)
{
    char old_name [NAME_MAX_LENGTH];
    char old_address [ADDRESS_MAX_LENGTH];
    char new_name [NAME_MAX_LENGTH];
    char new_address [ADDRESS_MAX_LENGTH];
    WorkDay new_daysWorking [WORK_DAYS];
    int new_numberOfDays = 0;

    printf("Kérem adja meg meglévő adatait\n");
    bool name_success = handle_name(old_name);
    bool address_success = handle_address(old_address);

    if(name_success && address_success)
    {
        bool found = false;
        int index = indexLookup(db, old_name, old_address, &found);

        if(found)
        {
            printf("Szerepel az adatbázisban\n");
            printf("Kérem adja meg új adatait:\n");

            bool isRegistered = false;

            bool new_name_success = handle_name(new_name);
            bool new_address_success = handle_address(new_address);
            bool newIsRegisteredSuccess = handle_registered(&isRegistered);
            bool new_working_days_success = true;

            if(new_name_success && new_address_success && newIsRegisteredSuccess && isRegistered)
            {
                new_working_days_success = handle_daysWorking(new_daysWorking, &new_numberOfDays);
            }
            
            if(new_name_success 
                && new_address_success 
                && new_working_days_success
                && newIsRegisteredSuccess)
            {
                strcpy(target->address, new_address);
                strcpy(target->name, new_name);
                target->isRegistered = isRegistered;
                target->numberOfDays = new_numberOfDays;
                memcpy(target->daysWorking, new_daysWorking, WORK_DAYS*sizeof(WorkDay));

                return index;
            }
        }
    }

    return -1;
}

void
startDayDialog(WorkerDatabase* database,
               const char* dailyTaskTextFile)
{
    char dayString [MAX_DAY_STR_LENGTH];
    char buffer [BUFFER_SIZE];

    printf("Munkanap: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%s", dayString);

    while(!is_working_day(dayString))
    {
        puts("Helytelen munkanap formátum"
             "Próbálja a következőket: hétfő kedd szerda csütörtök péntek szombat vasárnap");
        fgets(buffer, BUFFER_SIZE, stdin);
        sscanf(buffer, "%s", dayString);
    }

    WorkDay day = to_workday(dayString);
    officeProcess(database, dailyTaskTextFile, day);
    sleep(3);
}
