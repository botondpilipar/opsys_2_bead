#include "input_control.h"

bool is_name(const char* source)
{
    if(source == NULL)
        return false;
    if(strlen(source) == 0)
        return false;

    int is_name = true;
    for(int i = 0; i<strlen(source); ++i)
    {
        char current = source[i];
        if(!isalpha(current) && !isspace(current))
        {
            is_name = false;
        }
    }
    return is_name;
}

bool is_address(const char* source)
{
    if(source == NULL)
        return false;
    if(strlen(source) == 0)
        return false;
        
    int is_address = true;
    for(int i = 0; i<strlen(source); ++i)
    {
        char current = source[i];
        if(!isalnum(current) && !isspace(current) && !(current == '.'))
        {
            is_address = false;
        }
    }
    return is_address;
}

bool to_working_days(char* source, WorkDay* destination, int* number_of_days)
{
    char tokenized [1024][MAX_DAY_STR_LENGTH];
    char* delimiters = " \t";
    int token_iterator = 0;
    int work_day_iterator = 0;
    bool valid = true;

    char* token = strtok(source, delimiters);

    if(strlen(token) < MAX_DAY_STR_LENGTH)
        strcpy(tokenized[token_iterator], token);
    else
        valid = false;        

    while(tokenized[token_iterator] != NULL && token_iterator < 1024 && valid)
    {
        ++token_iterator;
        token = strtok(NULL, delimiters);
        if(strlen(token) < MAX_DAY_STR_LENGTH)
            strcpy(tokenized[token_iterator], token);
        else
            valid = false;
    }

    if(token_iterator > WORK_DAYS)
    {
        valid = false;
    }

    *number_of_days = token_iterator;
    for(int i = 0; i<token_iterator && valid; ++i)
    {
        valid = valid && is_name(tokenized[i]) && strlen(tokenized[i]) <= MAX_DAY_STR_LENGTH;

        if(valid)
        {
            destination[work_day_iterator] = to_workday(tokenized[i]);
            ++work_day_iterator;
        }
    }

    return valid;
}