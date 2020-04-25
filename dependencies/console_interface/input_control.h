#include <pch.h>
#include <database_macros.h>
#include <database.h>
#include <working_days.h>
#include <day_enum_conversion.h>

bool is_name(const char* source);

bool is_address(const char* source);

bool to_working_days(char* source, WorkDay* destination, int* number_of_days);