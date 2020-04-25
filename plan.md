# Operating Systems 2nd assignment

## The task

A "Jó bor, jó egészség" szőlészetbe (szülő) is megérkezett a tavasz. A szőlészet gazdatisztje(gyerek) naponta reggel ellenőrzi, hogy az egyes "dűlők" milyen állapotban vannak. A napi ellenőrzéssel megállapítja, hogy melyik területen (Jenő telek, Lovas dűlő, Hosszú, Selyem telek, Malom telek és Szula),  milyen munka elvégzését (metszés, rügyfakasztó permetezés, tavaszi nyitás, horolás) kell elvégezni. Például: Lovas dűlő, metszés. Egy nap csak egy helyen folyik valamilyen munka. Az ellenőrzés  eredményét csővezetéken továbbítja a szőlészeti irodába. 

A "Jó bor, jó egészség" szőlészet vezetése miután megkapta az aznapi munkafeladatot (pl: Malom telek, tavaszi nyitás), ezután elindítja a munkásjáratot(másik gyerek) a jelentkezett dolgozókért. A vezetés továbbítja a járat vezetőnek (szintén csővezetéken) az aznapra jelentkezettek névsorát, és azt, hogy aznap reggel, hova és milyen munkára kell vinni a csapatot. Ezeket az információkat a járat vezetője a képernyőre is kiírja.

Ha a napi jelentkezettek száma kisebb mint a szükséges keret, akkor a munkásjárat vezetője a "reggeli azonnali jelentkezők" közül kiválasztja(véletlenszerűen) a még hiányzó munkásokat. Miután kiszállította a napi csapatot a megadott helyszínre, egy jelzéssel tudatja a vezetéssel, hogy aznap hány munkást sikerült kiszállítani. A kapott eredményt a szőlészet vezetése képernyőre írja.


## Tools

- Cmake Makefile project
- Doxygen documentation generator
- googletest test framework
- Qt Creator Integrated Development Environment
- Clang Tidy
- Clang Format

## Project components

### Office (szőlészet iroda)

1. Process in itself
    a, Starts `VineyardOwner` process
       - Receives `DailyTask` from the child process
    b, Starts TaskMaster process
       - Daily workers as parameter
       - DailyTask as parameter

### VineyardOwner (gazdatiszt)
    a, Checks the state of vineyards
        - identifies the tasks needed
        - selects one task and sends it to the office

### TaskMaster (munkásjárat)
    a, Writes parameter to output
    b, Checks, whether there is enough registered worker
       - selects the rest from unregistered workers
    c, Writes the number of selected workers to output

### Vineyard (dűlő)

### DailyTask

## Dependencies

1. `DatabaseManager` from `opsys_1_bead`
2. `GoogleTest` from repository


# Tasks

1. [ ] Extend WorkerDatabase
   1. [X] `UnregisteredWorkerEntry` struct
   2. [X] Database to store both worker types
   3. [ ] Refactor and expand methods to unregistered workers
      1. [ ] Read entries
      2. [ ] Add entries
      3. [ ] Modify entries
      4. [ ] Delete entries
   4. [ ] Rename existing methods, fields
   6. [ ] Registered and unregistered entry infterface
   7. [ ] Registered and unregistered entry implementation
   8. [ ] Previous tests pass?
   9.  [ ] Send registered/unregistered workers through parametered signals
2. [ ] Resource tree planning
   1. [ ] resource tasks per vineyards - *how to store?*
   2. [ ] What process will read and modify which files?
3. [ ] Extend console interface
   1. [ ] Main dialog for registered, unregistered entries
   2. [ ] Extra dialog `startWorkingDay` to handle interactions
   3. [ ] Database management - database is same process
   4. [ ] Workday interaction - database is seperate process
4. [ ] Implement subprocesses
   1. [ ] `Office`
   2. [ ] `VineyardOwner`
   3. [ ] `TaskMaster`
5. [ ] Mocking
6. [ ] Testing
7. [ ] Documentation
8. [ ] Memory analysis
