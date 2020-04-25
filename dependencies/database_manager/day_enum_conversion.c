#include <pch.h>
#include "day_enum_conversion.h"

WorkDay to_workday(const char* from)
{
    if(strcmp(from, "hétfő") == 0)
    {
        return MON;
    }
    else if(strcmp(from, "kedd") == 0)
    {
        return TUE;
    }
    else if(strcmp(from, "szerda") == 0)
    {
        return WED;
    }
    else if(strcmp(from, "csütörtök") == 0)
    {
        return THU;
    }
    else if(strcmp(from, "péntek") == 0)
    {
        return FRI;
    }
    else if(strcmp(from, "szombat") == 0)
    {
        return SAT;
    }
    else if(strcmp(from, "vasárnap") == 0)
    {
        return SUN;
    }
    else
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
