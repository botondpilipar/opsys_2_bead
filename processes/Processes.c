#include <DailyTaskManager.h>
#include "Processes.h"

bool
isSubProcess()
{
    pid_t parent = getppid();
    if(parent == NULL)
        return false;

    return true;
}

void
vineyardOwnerProcess(const char* taskFileLocation, int pipe[])
{
    // Close reading end
    close(pipe[0]);
    DailyTaskManager manager = createDailyTaskManager(taskFileLocation);

    DailyTask selected = selectRandomTask(&manager);
    write(pipe[1], &selected, sizeof(DailyTask));
}

void
taskMasterProcess(WorkerDatabase database,
                  WorkDay day,
                  int workerListPipe[],
                  int dailyTaskPipe[])
{
    close(workerListPipe[1]);
    close(dailyTaskPipe[1]);

    WorkerEntry workerList[MAX_WORKER_PER_DAY];
    DailyTask task;

    ssize_t workerByte = read(workerListPipe[0], workerList, sizeof (workerList));
    read(dailyTaskPipe[0], &task, sizeof(task));

    if(database.jobs_avaliable[day] != 0)
    {
        unsigned int workersReceived = workerByte / sizeof (WorkerEntry);
        unsigned int workersRequired = MAX_WORKER_PER_DAY - workersReceived;

        WorkerEntry unregisteredWorkers[workersRequired];

        requestWorkers(&database,
                        unregisteredWorkers,
                        workersRequired,
                        day,
                        false);
    }
}
