#include "input_handlers.h"
#include "input_control.h"

void remove_trailing_newline(char* source)
{
    source[strlen(source) - 1] = 0;
}

bool handle_name(char* dest)
{
    char buffer [BUFFER_SIZE];

    printf("Név: >> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    remove_trailing_newline(buffer);
    while(strlen(buffer) != 0 && !is_name(buffer))
    {
        printf("Hibás bemenet \n Helyes példák: Laci, Dr. Szabo Konrad, Petofi Sandor\n");
        printf("Név: >> ");
        fgets(buffer,BUFFER_SIZE,  stdin);
        remove_trailing_newline(buffer);
    }

    if(strlen(buffer) == 0)
        return false;

    strcpy(dest, buffer);
    return true;
}

bool handle_address(char* dest)
{
    char buffer [BUFFER_SIZE];

    printf("Lakhely: >> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    remove_trailing_newline(buffer);
    while(strlen(buffer) != 0 && !is_address(buffer))
    {
        printf("Hibás bemenet \n Helyes példák: Munka utca 3., Budapest XVII. Jozsef utca\n");
        printf("Lakhely: >> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        remove_trailing_newline(buffer);
    }

    if(strlen(buffer) == 0)
        return false;
    strcpy(dest, buffer);
    return true;
}

bool handle_daysWorking(WorkDay* dest, int* number_of_days)
{
    char buffer [BUFFER_SIZE];
    printf("Vállalt napok: >> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    remove_trailing_newline(buffer);
    while(strlen(buffer) != 0 && !to_working_days(buffer, dest, number_of_days))
    {
        printf("Hibás bemenet: \n Helyes példák: hétfő kedd szerda \n vasárnap péntek hétfő\n");
        printf("Vállalt napok: >> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        remove_trailing_newline(buffer);
    }

    if(strlen(buffer) == 0)
        return false;

    return true;
}
