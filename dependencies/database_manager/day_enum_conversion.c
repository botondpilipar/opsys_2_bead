#include <pch.h>
#include "day_enum_conversion.h"

WorkDay to_workday(const char* from)
{
    if(strcmp(from, "hétfő") == 0)
    {
        return MON;
    }
    if(strcmp(from, "kedd") == 0)
    {
        return TUE;
    }
    if(strcmp(from, "szerda") == 0)
    {
        return WED;
    }
    if(strcmp(from, "csütörtök") == 0)
    {
        return THU;
    }
    if(strcmp(from, "péntek") == 0)
    {
        return FRI;
    }
    if(strcmp(from, "szombat") == 0)
    {
        return SAT;
    }
    if(strcmp(from, "vasárnap") == 0)
    {
        return SUN;
    }

    return 0;
}
const char* from_workday(WorkDay day)
{
    switch (day)
    {
    case MON:
        return "hétfő";
        break;
    case TUE:
        return "kedd";
        break;
    case WED:
        return "szerda";
        break;
    case THU:
        return "csütörtök";
        break;
    case FRI:
        return "péntek";
        break;
    case SAT:
        return "szombat";
        break;
    case SUN:
        return "vasárnap";
        break;
    default:
        return 0;
        break;
    }
}
