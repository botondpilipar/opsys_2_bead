#pragma once

#include <gtest/gtest.h>

class WorkerEntryTest : public ::testing::Test
{
protected:
//    virtual void SetUp();
//    virtual void TearDown();

    const char* entryName1 = "Peter";
    const char* entryName2 = "Paul";
    const char* entryAddress1 = "Work Street 2.";
    const char* entryAddress2 = "Home Street 12.";
    const WorkDay entryWorkDays1[WORK_DAYS] = {MON, TUE, WED, THU, FRI, SAT, SUN};
    const WorkDay entryWorkDays2[WORK_DAYS] = {MON, WED, TUE, FRI, THU};

    const WorkDay invalidDays1[WORK_DAYS] = {MON, MON, TUE, TUE, FRI, FRI, SAT};
    const WorkDay invalidDays2[WORK_DAYS] = {};
};

class DatabaseManagerTest : public WorkerEntryTest
{
protected:
    const char* entryName1 = "Peter";
    const char* entryName2 = "Paul";
    const char* entryName3 = "John";
    const char* entryName4 = "Matthew";
    const char* entryName5 = "Howard";
    const char* entryName6 = "Gt Dave";
    const char* entryAddress1 = "Work Street 1.";
    const char* entryAddress2 = "Work Street 2.";
    const char* entryAddress3 = "Work Street 3.";
    const char* entryAddress4 = "Work Street 4.";
    const char* entryAddress5 = "Work Street 5.";
    const char* entryAddress6 = "Work Street 6.";
    const WorkDay entryWorkdays1 [4] = {MON, TUE, WED, THU};
    const WorkDay entryWorkdays2 [3] = {FRI, SAT, SUN};
    const WorkDay entryWorkdays3 [1] = {TUE};
    const WorkDay entryWorkdays4 [1] = {WED};
    const WorkDay entryWorkdays5 [1] = {SUN};
};

