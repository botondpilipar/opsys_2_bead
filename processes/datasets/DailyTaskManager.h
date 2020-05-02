#pragma once

#include <pch.h>
#include <input_control.h>

#define VINEYARD_MAX_LENGTH 30
#define TASK_MAX_LENGTH 30
#define DEFAULT_TASK_SIZE 300

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

typedef struct DailyTask
{
    char task[TASK_MAX_LENGTH];
    char vineyard[VINEYARD_MAX_LENGTH];
} DailyTask;

typedef struct DailyTaskManager
{
    DailyTask* tasks;
    unsigned int taskNumber;
    unsigned int taskCapacity;
    FILE* diskStorage;
} DailyTaskManager;

/**
 * @brief Create DailyTask by handing its two parameters
 * @param task description of the task
 * @param vineyard name of the vineyard
 * @return Constructed DailyTask with parametes deepcopied
 */
DailyTask
createDailyTask(const char* task, const char* vineyard);

DailyTaskManager
createDailyTaskManager(const char* location);

bool
parseDailyTasks(DailyTaskManager* manager);

bool
recordDailyTasks(DailyTaskManager* manager, const char* location);

bool
expandDailyTaskManager(DailyTaskManager* manager);

bool
addDailyTask(DailyTaskManager* manager, DailyTask* task);

DailyTask
selectTask(DailyTaskManager* manager, unsigned int index);

DailyTask
selectFirstTask(DailyTaskManager* manager);

DailyTask
selectRandomTask(DailyTaskManager* manager);

void
cleanupDailyTaskManager(DailyTaskManager* manager);
