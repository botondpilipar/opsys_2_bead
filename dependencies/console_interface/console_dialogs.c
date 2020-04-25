#include "console_dialogs.h"
#include "input_handlers.h"
#include "../database_manager/worker_database_managers.h"
#include "../database_manager/worker_entry.h"

void display_main_menu(void)
{
    printf("[1] Új munkajelentkezés\n");
    printf("[2] Megléve adatok törlése\n");
    printf("[3] Adatok módosítása\n");
    printf("[4] Elérhető munkák naponta\n");
    printf("[5] Kilépés\n");
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

void main_dialog(WorkerDatabase* db)
{
    char input_buffer[1024];
    int menu_option = 0;

    do
    {
        display_main_menu();

        fgets(input_buffer, 1024, stdin);
        int success = sscanf(input_buffer, "%d", &menu_option);
        while(!success
             && strlen(input_buffer) > 0)
        {
            printf("Hibás bemenet, próbálja újra\n");
            display_main_menu();
            fgets(input_buffer, 1024, stdin);
        }

        switch (menu_option)
        {
            case 0:
                break;
            case 1:
            {
                bool new_entry_success = false;
                WorkerEntry entry = new_entry_dialog(db, &new_entry_success);
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
            case 2:
            {   
                int remove_at = removeEntry_dialog(db);
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
            case 3:
            {
                char name [NAME_MAX_LENGTH];
                char address [ADDRESS_MAX_LENGTH];
                WorkDay days [WORK_DAYS];
                WorkerEntry start_entry = createEntry(name, address, days, 0, true);
                int modify_at = modification_entry_dialog(db, &start_entry);
                
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
            case 4:
            {
                print_avaliable_jobs(db);
            }
                break;
            default:
                printf("Nem olvasható bemenet, kérem próbálja újra\n");
                break;
        }
    } while (menu_option != 0 && menu_option != 5);
}
    

WorkerEntry new_entry_dialog(WorkerDatabase* db, bool* successfull)
{
    char name [NAME_MAX_LENGTH];
    char address [ADDRESS_MAX_LENGTH];
    WorkDay daysWorking[WORK_DAYS];
    int numberOfDays = 0;

    bool name_success = handle_name(name);
    bool address_success = handle_address(address);
    bool daysWorking_success = handle_daysWorking(daysWorking, &numberOfDays);

    *successfull = name_success && address_success && daysWorking_success;
    if(*successfull)
    {
        bool already_in_db = false;
        indexLookup(db, name, address, &already_in_db);
        if(already_in_db)
            printf("A bejegyzés már az adatbázisban van.\n");
        else
            return createEntry(name, address, daysWorking, numberOfDays, true);
    }

    return createEntry("", "", NULL, 0, true);
}

int removeEntry_dialog(WorkerDatabase* db)
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
        else
            return -1;
    } else {
        return -1;
    }
}

int modification_entry_dialog(WorkerDatabase* db, WorkerEntry* target)
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

            bool new_name_success = handle_name(new_name);
            bool new_address_success = handle_address(new_address);
            bool new_working_days_success = 
                handle_daysWorking(new_daysWorking, &new_numberOfDays);
            
            if(new_name_success 
                && new_address_success 
                && new_working_days_success)
            {
                strcpy(target->address, new_address);
                strcpy(target->name, new_name);
                target->numberOfDays = new_numberOfDays;
                memcpy(target->daysWorking, new_daysWorking, WORK_DAYS*sizeof(WorkDay));

                return index;
            }
        }
    }

    return -1;
}
