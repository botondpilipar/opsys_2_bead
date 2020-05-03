#include "DailyTaskManager.h"

void
removeNewlineFromEnd(char* source)
{
    size_t size = strlen(source);

    if(source[size-1] == '\n')
        source[strlen(source)-1] = 0;
}

bool
parseDailyTasks(DailyTaskManager* manager)
{
    if(manager->diskStorage == NULL)
        return false;

    char lineBuffer[BUFFER_SIZE];
    fgets(lineBuffer, BUFFER_SIZE, manager->diskStorage);

    while(!feof(manager->diskStorage))
    {
        // Read vineyard
        char vineyard [VINEYARD_MAX_LENGTH];
        if(isProperNoun(lineBuffer))
        {
            removeNewlineFromEnd(lineBuffer);
            strcpy(vineyard, lineBuffer);
        }

        while(fgets(lineBuffer, BUFFER_SIZE, manager->diskStorage) != NULL
              && isApellative(lineBuffer))
        {
            removeNewlineFromEnd(lineBuffer);
            DailyTask newTask = createDailyTask(lineBuffer, vineyard);
            addDailyTask(manager, &newTask);
        }
    }

    return true;
}

DailyTask
createDailyTask(const char* task, const char* vineyard)
{
    DailyTask dailyTask;
    strcpy(dailyTask.task, task);
    strcpy(dailyTask.vineyard, vineyard);

    return dailyTask;
}

DailyTaskManager
createDailyTaskManager(const char* location)
{
    DailyTaskManager manager;
        manager.diskStorage = fopen(location, "r+");
        manager.tasks = (DailyTask*)malloc(DEFAULT_TASK_SIZE * sizeof(DailyTask));
        manager.taskNumber = 0;
        manager.taskCapacity = DEFAULT_TASK_SIZE;

    parseDailyTasks(&manager);

    return manager;
}

bool addDailyTask(DailyTaskManager* manager, DailyTask* task)
{

    if(manager == NULL || task == NULL)
        return false;

    if(manager->taskCapacity == manager->taskNumber)
        expandDailyTaskManager(manager);

    memcpy(&manager->tasks[manager->taskNumber], task, sizeof (DailyTask));
    manager->taskNumber += 1;

    return true;
}

bool
expandDailyTaskManager(DailyTaskManager* manager)
{
    if(manager->taskCapacity > manager->taskNumber)
        return true;

    DailyTask* newTasks = (DailyTask*)realloc(manager->tasks,
                                         2*manager->taskCapacity*sizeof (DailyTask));

    if(newTasks == NULL)
        return false;

    manager->tasks = newTasks;
    return true;
}
bool
recordDailyTasks(DailyTaskManager* manager, const char* location)
{
    if(manager == NULL || location == NULL)
        return false;

    fclose(manager->diskStorage);
    FILE* disk = fopen(location, "w+");
    char* vineyard = manager->tasks[0].vineyard;

    for(unsigned i = 0; i<manager->taskNumber; ++i)
    {
        if(strcmp(manager->tasks[i].vineyard, vineyard) == 0)
        {
            fprintf(disk, "%s\n", manager->tasks[i].task);
        }
        else
        {
            vineyard = manager->tasks[i].vineyard;
            fprintf(disk, "%s\n", vineyard);
            fprintf(disk, "%s\n", manager->tasks[i].task);
        }
    }
    fclose(disk);

    return true;
}

DailyTask
selectTask(DailyTaskManager* manager, unsigned int index)
{
    DailyTask selected = manager->tasks[index];
    unsigned int taskNumber = manager->taskNumber;

    manager->tasks[index] = manager->tasks[taskNumber-1];
    manager->taskNumber -= 1;

    return selected;
}

DailyTask
selectFirstTask(DailyTaskManager* manager)
{
    return selectTask(manager, 0);
}

DailyTask
selectRandomTask(DailyTaskManager* manager)
{
    srand(time(NULL));
    unsigned int index = rand() % manager->taskNumber;
    return selectTask(manager, index);
}

void
cleanupDailyTaskManager(DailyTaskManager* manager)
{
    if(manager != NULL)
    {
        if(manager->tasks != NULL)
            free(manager->tasks);
        if(manager->diskStorage != NULL)
            fclose(manager->diskStorage);
    }
}
