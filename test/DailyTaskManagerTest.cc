#include <gtest/gtest.h>
#include <filesystem>

extern "C"
{
    #include <DailyTaskManager.h>
}

bool operator==(DailyTask l, DailyTask r)
{
    return memcmp(&l, &r, sizeof(DailyTask) == 0);
}

bool 
notOneOfRemaining(DailyTaskManager* manager, DailyTask* selected)
{
    bool found = false;
    for(int i = 0; i<manager->taskNumber && !found; ++i)
    {
        if(memcmp(&manager->tasks[i], selected, sizeof(DailyTask) == 0))
        {
            found = true;
        }
    }
    return found;
}

class DailyTaskManagerTest : public ::testing::Test
{
protected:
    const char* fileLocation = "resources/test_resource/testVineyardTasks.txt";
    const unsigned int tasksInFile = 4;
    const unsigned int vineyardsInFile = 3;

    const char* taskMettszes = "mettszes";
    const char* taskHorolas = "horolas";
    const char* taskTavasziNyitas = "tavaszi nyitas";
    const char* taskPermetezes = "rugyfakaszto permetezes";

    const char* vineyardJeno = "Jeno telek";
    const char* vineyardSelyem = "Selyem telek";
    const char* vineyardMalom = "Malom telek";
    const char* vineyardLovas = "Lovas dulo";
    const char* vineyardSzula = "Szula";

    const DailyTask task1 = createDailyTask(taskMettszes, vineyardJeno);
    const DailyTask task2 = createDailyTask(taskPermetezes, vineyardSelyem);
    const DailyTask task3 = createDailyTask(taskMettszes, vineyardMalom);
    const DailyTask task4 = createDailyTask(taskTavasziNyitas, vineyardMalom);

    DailyTaskManager manager;
};

TEST_F(DailyTaskManagerTest, TaskCreation)
{
    DailyTask task = createDailyTask(taskHorolas, vineyardJeno);

    ASSERT_STREQ(task.task, taskHorolas);
    ASSERT_STREQ(task.vineyard, vineyardJeno);
    ASSERT_NE(task.task, taskHorolas);
    ASSERT_NE(task.vineyard, vineyardJeno);
}

TEST_F(DailyTaskManagerTest, ManagerCreation)
{
    std::filesystem::path file(fileLocation);
    ASSERT_TRUE(std::filesystem::exists(file));

    manager = createDailyTaskManager(fileLocation);

    EXPECT_EQ(manager.taskNumber, tasksInFile);
    EXPECT_GT(manager.taskCapacity, 0);

    EXPECT_STREQ(task1.task, manager.tasks[0].task);
    EXPECT_STREQ(task1.vineyard, manager.tasks[0].vineyard);
    EXPECT_STREQ(task2.task, manager.tasks[1].task);
    EXPECT_STREQ(task2.vineyard, manager.tasks[1].vineyard);
    EXPECT_STREQ(task3.task, manager.tasks[2].task);
    EXPECT_STREQ(task3.vineyard, manager.tasks[2].vineyard);
    EXPECT_STREQ(task4.task, manager.tasks[3].task);
    EXPECT_STREQ(task4.vineyard, manager.tasks[3].vineyard);

    cleanupDailyTaskManager(&manager);
}

TEST_F(DailyTaskManagerTest, SelectTasks)
{
    manager = createDailyTaskManager(fileLocation);

    DailyTask lastSelected = selectTask(&manager, 3);

    EXPECT_STREQ(task4.task, lastSelected.task);
    EXPECT_STREQ(task4.vineyard, lastSelected.vineyard);

    DailyTask firstSelected = selectFirstTask(&manager);

    EXPECT_STREQ(task1.task, firstSelected.task);
    EXPECT_STREQ(task1.vineyard, firstSelected.vineyard);

    DailyTask randomSelected = selectRandomTask(&manager);
    DailyTask* found = std::find(manager.tasks,
                                 manager.tasks + manager.taskNumber,
                                 randomSelected);
    EXPECT_EQ(found, manager.tasks + manager.taskNumber);

    cleanupDailyTaskManager(&manager);
}
