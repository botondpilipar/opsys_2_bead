#include <pch.h>
#include <database_macros.h>
#include <database.h>
#include <working_days.h>
#include <day_enum_conversion.h>

#define REGEX_MAX_LENGTH 300
#define WEEK_LENGTH 7

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

bool
is_name(const char* source);

bool
is_address(const char* source);

bool
to_working_days(char* source, WorkDay* destination, int* number_of_days);

bool
isApellative(char* source);

bool
isProperNoun(char* source);
