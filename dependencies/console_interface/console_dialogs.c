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

// void printf_workers(WorkerDatabase* db)
// {
//     for(int i = 0; i<db->entry_number; ++i)
//     {
//         puts(db->entries[i].name);
//         puts(db->entries[i].address);
//         for(int j = 0; j<db->entries[i].days_working; ++j)
//         {
//             puts()
//         }
//     }
// }
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
                    bool added = add_entry(db, &entry);
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
                int remove_at = remove_entry_dialog(db);
                if(remove_at < 0)
                    printf("Sikertelen művelet!\n");
                else
                {
                    bool removed = remove_entry(db, remove_at);
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
                WorkerEntry start_entry = create_entry(name, address, days, 0);
                int modify_at = modification_entry_dialog(db, &start_entry);
                
                if(modify_at >= 0)
                {
                    if(modify_entry(db, modify_at, &db->entries[modify_at], &start_entry))
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
    WorkDay days_working[WORK_DAYS];
    int number_of_days = 0;

    bool name_success = handle_name(name);
    bool address_success = handle_address(address);
    bool days_working_success = handle_days_working(days_working, &number_of_days);

    *successfull = name_success && address_success && days_working_success;
    if(*successfull)
    {
        bool already_in_db = false;
        index_lookup(db, name, address, &already_in_db);
        if(already_in_db)
            printf("A bejegyzés már az adatbázisban van.\n");
        else
            return create_entry(name, address, days_working, number_of_days);
    }

    return create_entry("", "", NULL, 0);
}

int remove_entry_dialog(WorkerDatabase* db)
{
    char name [NAME_MAX_LENGTH];
    char address [ADDRESS_MAX_LENGTH];
    bool entry_found = false;

    bool name_success = handle_name(name);
    bool address_success = handle_address(address);

    if(name_success && address_success)
    {
        int result =  index_lookup(db, name, address, &entry_found);
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
    WorkDay new_days_working [WORK_DAYS];
    int new_number_of_days = 0;

    printf("Kérem adja meg meglévő adatait\n");
    bool name_success = handle_name(old_name);
    bool address_success = handle_address(old_address);

    if(name_success && address_success)
    {
        bool found = false;
        int index = index_lookup(db, old_name, old_address, &found);

        if(found)
        {
            printf("Szerepel az adatbázisban\n");
            printf("Kérem adja meg új adatait:\n");

            bool new_name_success = handle_name(new_name);
            bool new_address_success = handle_address(new_address);
            bool new_working_days_success = 
                handle_days_working(new_days_working, &new_number_of_days);
            
            if(new_name_success 
                && new_address_success 
                && new_working_days_success)
            {
                strcpy(target->address, new_address);
                strcpy(target->name, new_name);
                target->number_of_days = new_number_of_days;
                memcpy(target->days_working, new_days_working, WORK_DAYS*sizeof(WorkDay));

                return index;
            }
        }
    }

    return -1;
}