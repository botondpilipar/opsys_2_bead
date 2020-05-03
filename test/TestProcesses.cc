extern "C"
{
    #include <pch.h>
    #include <Processes.h>
    #include <DailyTaskManager.h>
}

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "TestFixtures.hh"

#define DAILY_TASK_RESOURCE1 "resources/test_resource/dailyTasks.txt"
#define DAILY_TASK_RESOURCE2 "resources/test_resource/vineyard_tasks.txt"
#define TEST_EMPLOYED "resources/test_resource/employed_per_day1.txt"
#define TEST_ENTRIES "resources/test_resource/test_entries.bin"

void
makeEmptyFile(const char* location)
{
    FILE* file = fopen(location, "w+");
    fclose(file);
}

TEST(TestProcess, VineyardOwner)
{
    int input[2];
    pipe(input);

    FILE* dailyTaskFile = fopen(DAILY_TASK_RESOURCE1, "w+");
    fprintf(dailyTaskFile, "Jeno telek\n");
    fprintf(dailyTaskFile, "mettszes\n");
    fclose(dailyTaskFile);

    pid_t pid = fork();
    if(pid > 0)
    {
        vineyardOwnerProcess(DAILY_TASK_RESOURCE1, input, pid);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(input[1]);
        DailyTask onlyTaskInFile = createDailyTask("mettszes", "Jeno telek");
        DailyTask resultTask = createDailyTask("asdf", "asdf");
        ssize_t successfullRead = read(input[0], &resultTask, sizeof(DailyTask));

        ASSERT_GT(successfullRead, 0);
        EXPECT_STREQ(onlyTaskInFile.task, resultTask.task);
        EXPECT_STREQ(onlyTaskInFile.vineyard, resultTask.vineyard);
    }
}

TEST_F(DatabaseManagerTest, TaskmasterProcess)
{
    int workerListOutput[2];
    int dailyTaskOutput[2];
    int taskMasterProcessReturn;
    pipe(workerListOutput);
    pipe(dailyTaskOutput);
    
    makeEmptyFile(TEST_ENTRIES);

    WorkerDatabase database = initializeDb(TEST_ENTRIES, TEST_EMPLOYED);

    DailyTask task = createDailyTask("mettszes", "Jeno telek");
    
    WorkerEntry unregistered1
        = createEntry(entryName4, entryAddress4, entryWorkdays2, 0, false);
    WorkerEntry unregistered2
        = createEntry(entryName5, entryAddress5, entryWorkdays2, 0, false);
    WorkerEntry unregistered3
        = createEntry(entryName6, entryAddress6, entryWorkdays2, 0, false);
    WorkerEntry registered1
        = createEntry(entryName1, entryAddress1, entryWorkdays1, 4, true);

    addEntry(&database, &unregistered1);
    addEntry(&database, &unregistered2);
    addEntry(&database, &unregistered3);

    pid_t pid = fork();
    if(pid > 0)
    {
        taskMasterProcessReturn 
            = taskMasterProcess(&database, THU, workerListOutput, dailyTaskOutput);
        exit(0);
    }
    else
    {
        close(workerListOutput[0]);
        close(dailyTaskOutput[0]);
        write(workerListOutput[1], &registered1, sizeof(WorkerEntry));
        write(dailyTaskOutput[1], &task, sizeof(DailyTask));
        wait(NULL);
    }
}