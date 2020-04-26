#include "input_control.h"


int
match(const char *string, char *pattern)
{
    int    status;
    regex_t    re;


    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);
    }
    return(1);
}

bool is_name(const char* source)
{
    if(source == NULL || strlen(source) == 0)
        return false;

    char* name_pattern_ecma = "^([A-Z]([A-Za-z])*(\\.)?(\\s)?)*$";
    int result = match(source, name_pattern_ecma);
    
    if(result == 1)
        return true;

    return false;
}

bool is_address(const char* source)
{
    if(source == NULL || strlen(source) == 0)
        return false;
    char* address_pattern = 
        "^[A-Z]([A-Za-z])*(\\.)?(\\s)?(([A-Za-z])*(\\.)?(\\s)?|[1-9]([0-9])*(\\.)?(\\s)?)*[1-9]([0-9])*(\\.)?(\\s)?$";

    int result =  match(source, address_pattern);
    return result;
}

bool is_working_day(char* source)
{
    char days [WEEK_LENGTH][MAX_DAY_STR_LENGTH+2] =
        {"^hétfő$", "^kedd$", "^szerda$", "^csütörtök$", "^péntek$", "^szombat$", "^vasárnap$"};

    bool any_matches = false;
    for(int i = 0; i<WORK_DAYS && !any_matches; ++i)
    {
        any_matches = any_matches || match(source, days[i]);
    }
    return any_matches;
}

bool to_working_days(char* source, WorkDay* destination, int* number_of_days)
{
    char tokenized [BUFFER_SIZE][MAX_DAY_STR_LENGTH];
    char* delimiters = " \t";
    int token_iterator = 0;
    bool valid = true;

    char* token = strtok(source, delimiters);

    if(strlen(token) <= MAX_DAY_STR_LENGTH)
        strcpy(tokenized[token_iterator], token);
    else
        valid = false;        

    while(tokenized[token_iterator] != NULL && token_iterator < BUFFER_SIZE && valid)
    {
        ++token_iterator;
        token = strtok(NULL, delimiters);
        if(token != NULL && strlen(token) <= MAX_DAY_STR_LENGTH)
            strcpy(tokenized[token_iterator], token);
        else if(token == NULL)
            break;
        else
            valid = false;
    }

    if(token_iterator > WORK_DAYS)
        valid = false;
    else
        *number_of_days = token_iterator;


    for(int i = 0; i<token_iterator && valid; ++i)
    {
        valid = valid && is_working_day(tokenized[i]);

        if(valid)
            destination[i] = to_workday(tokenized[i]);
    }

    return valid;
}
