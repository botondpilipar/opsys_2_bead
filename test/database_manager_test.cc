#include "database_manager_test.hh"

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

TEST_F(WorkerEntryTest, EntryCreationAndValidation)
{
    WorkerEntry valid1 = createEntry(entryName1, entryAddress1, entryWorkDays1, 7, true);
    WorkerEntry valid2 = createEntry(entryName1, entryAddress1, entryWorkDays2, 5, true);
    WorkerEntry invalid1 = createEntry(entryName1, entryAddress1, invalidDays1, 7, true);
    WorkerEntry invalid11 = createEntry(entryName1, entryAddress1, invalidDays1, 0, true);
    WorkerEntry invalid2 = createEntry(entryName2, entryAddress2, invalidDays2, 0, true);

    WorkerEntry unregisteredValid1 = createEntry(entryName1, entryAddress1, entryWorkDays1, 7, false);
    WorkerEntry unregisteredValid2 = createEntry(entryName2, entryAddress2, nullptr, 0, false);

    EXPECT_TRUE(isValidEntry(&valid1));
    EXPECT_TRUE(isValidEntry(&valid2));
    EXPECT_TRUE(isValidEntry(&unregisteredValid1));
    EXPECT_TRUE(isValidEntry(&unregisteredValid2));
    EXPECT_FALSE(isValidEntry(&invalid1));
    EXPECT_FALSE(isValidEntry(&invalid11));
    EXPECT_FALSE(isValidEntry(&invalid2));
}

TEST_F(WorkerEntryTest, EntryToWork)
{
    WorkerEntry valid1 = createEntry(entryName1, entryAddress1, entryWorkDays1, 7, true);
    WorkerEntry valid2 = createEntry(entryName1, entryAddress1, entryWorkDays2, 5, true);
    WorkerEntry unregisteredValid1 = createEntry(entryName1, entryAddress1, entryWorkDays1, 7, false);

    size_t validDays1 = valid1.numberOfDays;
    size_t validDays2 = valid2.numberOfDays;

    EXPECT_TRUE(canWorkOnDay(&valid1, MON));
    EXPECT_TRUE(canWorkOnDay(&valid1, FRI));
    EXPECT_TRUE(canWorkOnDay(&unregisteredValid1, FRI));
    EXPECT_FALSE(canWorkOnDay(&valid2, SUN));

    sendToWork(&valid1, MON);
    sendToWork(&valid2, FRI);

    EXPECT_FALSE(canWorkOnDay(&valid1, MON));
    EXPECT_FALSE(canWorkOnDay(&valid2, FRI));
    EXPECT_NE(valid1.numberOfDays, validDays1);
    EXPECT_NE(valid2.numberOfDays, validDays2);
}

TEST(Database, TestInitialize)
{
    FILE* test_entry = fopen(TEST_ENTRY, "w");
    FILE* empty_entry = fopen(EMPTY_ENTRY, "w");
    fclose(test_entry);
    fclose(empty_entry);
    
}

TEST(Database, CanOpenTextAndBinaryFile)
{
    int days_test1[WORK_DAYS];

    ASSERT_EQ(populateJobsRequired(TEST_RESOURCE1, days_test1), EXIT_SUCCESS);
    ASSERT_EQ(populateJobsRequired(TEST_RESOURCE2, days_test1), EXIT_SUCCESS);
    ASSERT_EQ(populateJobsRequired(INVALID_RESOURCE, days_test1), EXIT_FAILURE);
}

TEST(Database, DatabaseJobRequiredPopulation)
{
    int expected1[WORK_DAYS];
    int expected2[WORK_DAYS];
    
    std::iota(expected1, expected1 + WORK_DAYS, 1);
    auto r_from = std::reverse_iterator<int*>(expected2 + WORK_DAYS);
    auto r_to = std::reverse_iterator<int*>(expected2);
    std::iota(r_from, r_to, 1);

    int work_days1[WORK_DAYS];
    int work_days2[WORK_DAYS];

    populateJobsRequired(TEST_RESOURCE1, work_days1);
    populateJobsRequired(TEST_RESOURCE2, work_days2);
    
    EXPECT_TRUE(std::equal(expected1, expected1 + WORK_DAYS, work_days1));
    EXPECT_TRUE(std::equal(expected2, expected2 + WORK_DAYS, work_days2));
}

TEST(Database, DatabaseCreation)
{
    WorkerDatabase db1 = createDb(EMPTY_ENTRY);
    WorkerDatabase db2 = createDb(TEST_ENTRY);

    EXPECT_EQ(db1.entryNumber, 0);
    EXPECT_EQ(db1.entryCapacity, INIT_DATABASE_LENGTH);
    EXPECT_EQ(db1.entryMemorySize, sizeof(WorkerEntry));
    EXPECT_NE(db1.diskStorage, nullptr);

    EXPECT_NE(db2.diskStorage, nullptr);

    cleanupDb(&db1);
    cleanupDb(&db2);
}

TEST(Database, DatabaseExpansion)
{
    WorkerDatabase db_test = createDb(EMPTY_ENTRY);
    EXPECT_FALSE(expandDatabase(&db_test));

    db_test.entryNumber = db_test.entryCapacity;
    EXPECT_TRUE(expandDatabase(&db_test));

    db_test.entryCapacity = 0;
    db_test.entryNumber = 0;
    expandDatabase(&db_test);
    EXPECT_GT(db_test.entryCapacity, 0);

    cleanupDb(&db_test);
}

TEST(Database, DatabaseEntryPopulationEmpty)
{
    WorkerDatabase db_test = createDb(TEST_ENTRY);

    ASSERT_EQ(populateEntries(&db_test), EXIT_SUCCESS);

    cleanupDb(&db_test);
}
 
TEST(DatabaseManipulation, AddEntry)
{
    WorkerDatabase db = initializeDb(TEST_ENTRY, TEST_RESOURCE1);
    
    WorkDay days1 [WORK_DAYS] = {MON, TUE};
    WorkDay days2 [WORK_DAYS] = {WED, THU};
    WorkDay days3 [WORK_DAYS] = {MON};

    WorkerEntry entry1 = createEntry("John", "Work Street 1.", days1, 2, true);
    WorkerEntry entry2 = createEntry("Peter", "Work Street 2.", days2, 2, true);
    WorkerEntry entry3 = createEntry("Sarah", "Work street 3.", days3, 1, true);

    EXPECT_TRUE(addEntry(&db, &entry1));
    EXPECT_TRUE(addEntry(&db, &entry2));
    EXPECT_FALSE(addEntry(&db, &entry3));

    WorkerDatabase db_copy = createDb(EMPTY_ENTRY);
    copyDatabase(&db_copy, &db);
    cleanupDb(&db);

    WorkerDatabase db_new = initializeDb(TEST_ENTRY, TEST_RESOURCE1);

    ASSERT_EQ(db_copy.entryNumber, db_new.entryNumber);
    EXPECT_EQ(memcmp(db_copy.entries, db_new.entries, db_new.entryNumber*db_new.entryMemorySize), 0);
    EXPECT_EQ(memcmp(db_copy.jobs_avaliable, db_new.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanupDb(&db_new);
}

TEST(DatabaseManipulation, LookupEntry)
{
    WorkerDatabase db = initializeDb(TEST_ENTRY, TEST_RESOURCE1);
    bool john_found = false;
    bool peter_found = false;
    const char jname[NAME_MAX_LENGTH] = "John";
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char jaddress [ADDRESS_MAX_LENGTH] = "Work Street 1.";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    WorkDay jworking [WORK_DAYS] = {MON, TUE};
    WorkDay pworking [WORK_DAYS] = {WED, THU};

    ASSERT_EQ(db.entryNumber, 2);

    WorkerEntry john = lookup(&db, jname, jaddress, &john_found);
    WorkerEntry peter = lookup(&db, pname, paddress, &peter_found);

    ASSERT_TRUE(peter_found); ASSERT_TRUE(john_found);
    EXPECT_STREQ(john.name, jname); EXPECT_STREQ(john.address, jaddress); 
    EXPECT_STREQ(peter.name, pname);EXPECT_STREQ(peter.address, paddress);
    ASSERT_EQ(peter.numberOfDays, 2); ASSERT_EQ(peter.numberOfDays, 2);
    ASSERT_EQ(memcmp(peter.daysWorking, pworking, 2), 0);
    ASSERT_EQ(memcmp(john.daysWorking, jworking, 2),  0);

    cleanupDb(&db);
}

TEST(DatabaseManipulation, RemoveLastEntry)
{
    WorkerDatabase db = initializeDb(TEST_ENTRY, TEST_RESOURCE1);

    bool peter_found = false;
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    WorkDay pworking [WORK_DAYS] = {WED, THU};
    size_t original_entries = db.entryNumber;
    int original_jobs_avaliable [WORK_DAYS];

    memcpy(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS*sizeof(int));

    int peter_index = indexLookup(&db, pname, paddress, &peter_found);
    ASSERT_TRUE(peter_found);

    bool peter_removed = removeEntry(&db, peter_index);
    EXPECT_TRUE(peter_removed);
    EXPECT_EQ(original_entries - 1, db.entryNumber);
    EXPECT_NE(memcmp(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    indexLookup(&db, pname, paddress, &peter_found);
    EXPECT_FALSE(peter_found);

    WorkerEntry peter_again = createEntry(pname, paddress, pworking, 2, true);
    addEntry(&db, &peter_again);

    WorkerDatabase db_copy = createDb(TEST_ENTRY);
    copyDatabase(&db_copy, &db);
    cleanupDb(&db);

    WorkerDatabase db_new = initializeDb(TEST_ENTRY, TEST_RESOURCE1);
    EXPECT_EQ(db_new.entryNumber, db_copy.entryNumber);
    EXPECT_EQ(db_new.entryCapacity, db_copy.entryCapacity);
    EXPECT_EQ(memcmp(db_new.entries, db_copy.entries, db_copy.entryNumber*sizeof(WorkerEntry)), 0);
    EXPECT_EQ(memcmp(db_new.jobs_avaliable, db_copy.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanupDb(&db_new);
    free(db_copy.entries);
}

TEST(DatabaseManipulation, RemoveAnyEntry)
{
    WorkerDatabase db = initializeDb(TEST_ENTRY, TEST_RESOURCE1);

    bool john_found = false;
    bool peter_found = false;
    const char jname[NAME_MAX_LENGTH] = "John";
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char jaddress [ADDRESS_MAX_LENGTH] = "Work Street 1.";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    size_t original_entries = db.entryNumber;
    int original_jobs_avaliable [WORK_DAYS];
    memcpy(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS);

    int john_index = indexLookup(&db, jname, jaddress, &john_found);
    int peter_index = indexLookup(&db, pname, paddress, &peter_found);

    bool john_removed = removeEntry(&db, john_index);
    EXPECT_TRUE(john_removed);
    EXPECT_EQ(db.entryNumber, original_entries - 1);
    EXPECT_NE(memcmp(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS), 0);

    int peter_new_index = indexLookup(&db, pname, paddress, &peter_found);
    EXPECT_TRUE(peter_found);
    EXPECT_NE(peter_new_index, peter_index);

    WorkerDatabase db_copy = createDb(TEST_ENTRY);
    copyDatabase(&db_copy, &db);
    cleanupDb(&db);

    WorkerDatabase db_new = initializeDb(TEST_ENTRY, TEST_RESOURCE1);

    EXPECT_EQ(db_new.entryNumber, db_copy.entryNumber);
    EXPECT_EQ(db_new.entryCapacity, db_copy.entryCapacity);
    EXPECT_EQ(memcmp(db_new.entries, db_copy.entries, db_copy.entryNumber*sizeof(WorkerEntry)), 0);
    EXPECT_EQ(memcmp(db_new.jobs_avaliable, db_copy.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanupDb(&db_new);
    free(db_copy.entries);
}
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
