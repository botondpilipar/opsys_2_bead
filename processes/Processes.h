#pragma once

#include <pch.h>
#include <DailyTaskManager.h>
#include <worker_database_managers.h>
#include <database_macros.h>
#include <working_days.h>

#define MAX_WORKER_PER_DAY INIT_DATABASE_LENGTH

bool
isChildProcess(pid_t forkPid);

int
vineyardOwnerProcess(const char* taskFileLocation, int pipe[], pid_t forkPid);

int
taskMasterProcess(WorkerDatabase* database,
                  WorkDay day,
                  int workerListPipe[],
                  int dailyTaskPipe[]);

int
officeProcess(WorkerDatabase* database,
              const char* dailtyTaskFileLocation,
              WorkDay day);
