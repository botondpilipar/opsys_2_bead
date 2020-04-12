#ifndef DAY_ENUM_CONVERSION_H
#define DAY_ENUM_CONVERSION_H

#include "working_days.h"

WorkDay to_workday(const char* from);

const char* from_workday(WorkDay day);

#endif