#include <DailyTaskManager.h>
#include "Processes.h"
#define WORKER_SENT_SIGNAL SIGUSR1

bool
isChildProcess(pid_t forkPid)
{
    return forkPid == 0;
}

int
vineyardOwnerProcess(const char* taskFileLocation, int pipe[], pid_t forkPid)
{
    puts("Gazdatiszt:");

    close(pipe[0]);
    DailyTaskManager manager = createDailyTaskManager(taskFileLocation);

    if(manager.diskStorage == NULL || manager.taskNumber == 0)
    {
        perror("Could not open input file to read");
        return EXIT_FAILURE;
    }
    else
    {
        DailyTask selected = selectRandomTask(&manager);

        printf("Kiválasztott feladat: %s %s\n", selected.task, selected.vineyard);
        puts("-----------------------------------------------------------------");

        write(pipe[1], &selected, sizeof(DailyTask));

        recordDailyTasks(&manager, taskFileLocation);
        return EXIT_SUCCESS;
    }
}

int
taskMasterProcess(WorkerDatabase* database,
                  WorkDay day,
                  int workerListPipe[],
                  int dailyTaskPipe[])
{
    puts("Munkásjárat: ");

    close(workerListPipe[1]);
    close(dailyTaskPipe[1]);

    WorkerEntry workerList[MAX_WORKER_PER_DAY];
    DailyTask task;

    ssize_t workerByte = read(workerListPipe[0], workerList, sizeof (workerList));
    ssize_t dailyTaskByte = read(dailyTaskPipe[0], &task, sizeof(task));

    if(workerByte == 0 || dailyTaskByte == 0)
    {
        puts("Munkasjarat nem kapta meg a dologozok"
             "listajat vagy a napi feladatatot."
             "Sikertelen munkanap. Visszateres");
            
        return EXIT_FAILURE;
    }

    unsigned int workersReceived = workerByte / sizeof (WorkerEntry);
    unsigned int workersRequired = database->jobs_avaliable[day] - workersReceived;

    printf("A munkásjárat a napi feladatot megkapta: \n %s %s \n", task.task, task.vineyard);
    printf("A munkahoz %d munkás jelentkezett\n", workersReceived);

    for(unsigned int i = 0; i<workersReceived; ++i)
        puts(workerList[i].name);

    printf("Szükséges még %d munkás a kezdéshez\n", workersRequired);

    if(database->jobs_avaliable[day] != 0)
    {
        WorkerEntry unregisteredWorkers[workersRequired];
        unsigned int received =
                requestWorkers(database, unregisteredWorkers, workersRequired, day, false);

        puts("Aznapi jelentkezők névsora: \n");
        for(unsigned i = 0; i<received; ++i)
        {
            puts(workerList[i].name);
        }
        puts("-----------------------------------------------------------------");

        workersReceived += received;
    }

    union sigval totalWorkersReceived;
    totalWorkersReceived.sival_int = workersReceived;
    sigqueue(getppid(), WORKER_SENT_SIGNAL, totalWorkersReceived);

    return EXIT_SUCCESS;
}

void
taskMasterSignalHandler(int sigNumber, siginfo_t* info, void* unused)
{
    printf("Sikeresen kiszállított munkások: %d\n", info->si_value.sival_int);
}

int
officeProcess(WorkerDatabase* database,
              const char* dailyTaskFileLocation,
              WorkDay day)
{
    struct sigaction action;
    action.sa_sigaction = taskMasterSignalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(WORKER_SENT_SIGNAL, &action, NULL);

    puts("Szőlészeti iroda: ");

    int dailyTaskInputPipe[2];
    int dailyTaskOutputPipe[2];
    int workerListOutputPipe[2];
    if (pipe(dailyTaskInputPipe) < 0 ||
        pipe(dailyTaskOutputPipe)< 0 ||
        pipe(workerListOutputPipe)<0)
        {
            perror("Unsuccessfull pipe opening");
            return EXIT_FAILURE;
        }


    pid_t vineyardOwnerPid = fork();
    if(isChildProcess(vineyardOwnerPid))
    {
        vineyardOwnerProcess(dailyTaskFileLocation, dailyTaskInputPipe, vineyardOwnerPid);
        exit(0);
    }
    else
    {
        close(dailyTaskInputPipe[1]);

        DailyTask selectedTask;
        read(dailyTaskInputPipe[0], &selectedTask, sizeof(DailyTask));

        pid_t vineyardOwnerPid = fork();

        if(isChildProcess(vineyardOwnerPid))
        {
            taskMasterProcess(database, day, workerListOutputPipe, dailyTaskOutputPipe);
            exit(0);
        }
        else
        {
            close(dailyTaskOutputPipe[0]);

            // TODO jobs_available is the complete opposite of what I need. I need the one required`
            unsigned int registeredWorkersRequired = database->jobs_required[day];
            WorkerEntry registeredWorkers[registeredWorkersRequired];

            unsigned int workersReceived =
                    requestWorkers(database, registeredWorkers, registeredWorkersRequired, day, true);

            write(workerListOutputPipe[1], registeredWorkers, workersReceived*sizeof(WorkerEntry));
            write(dailyTaskOutputPipe[1], &selectedTask, sizeof (DailyTask));

            pause();
            puts("Nap vége");
        }
        
    }

    
}
