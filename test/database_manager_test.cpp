#include "database_manager_test.h"

TEST(Database, TestInitialize)
{
    FILE* test_entry = fopen(TEST_ENTRY, "w");
    FILE* empty_entry = fopen(EMPTY_ENTRY, "w");
    fclose(test_entry);
    fclose(empty_entry);
    
}
TEST(Entry, ValidEntry)
{
    WorkDay valid_days1 [WORK_DAYS] = {MON, TUE, WED, THU, FRI, SAT, SUN};
    WorkDay valid_days2 [WORK_DAYS] = {MON, WED, TUE, FRI, THU};
    WorkDay invalid_days1 [WORK_DAYS] = {MON, MON, MON, MON, MON, MON, MON};
    WorkDay invalid_days2 [WORK_DAYS] = {TUE, WED, THU, TUE, FRI, FRI};

    WorkerEntry valid1 = create_entry("Peter", "Street", valid_days1, 7);
    WorkerEntry valid2 = create_entry("Peter", "Street", valid_days2, 5);
    WorkerEntry invalid1 = create_entry("Peter", "Street", invalid_days1, 7);
    WorkerEntry invalid11 = create_entry("Peter", "Street", invalid_days1, 0);
    WorkerEntry invalid2 = create_entry("Peter", "Street", invalid_days2, 6);

    EXPECT_TRUE(is_valid_entry(&valid1));
    EXPECT_TRUE(is_valid_entry(&valid2));
    EXPECT_FALSE(is_valid_entry(&invalid1));
    EXPECT_FALSE(is_valid_entry(&invalid11));
    EXPECT_FALSE(is_valid_entry(&invalid2));
}


TEST(Database, CanOpenTextAndBinaryFile)
{
    int days_test1[WORK_DAYS];

    ASSERT_EQ(populate_jobs_required(TEST_RESOURCE1, days_test1), EXIT_SUCCESS);
    ASSERT_EQ(populate_jobs_required(TEST_RESOURCE2, days_test1), EXIT_SUCCESS);
    ASSERT_EQ(populate_jobs_required(INVALID_RESOURCE, days_test1), EXIT_FAILURE);
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

    populate_jobs_required(TEST_RESOURCE1, work_days1);
    populate_jobs_required(TEST_RESOURCE2, work_days2);
    
    EXPECT_TRUE(std::equal(expected1, expected1 + WORK_DAYS, work_days1));
    EXPECT_TRUE(std::equal(expected2, expected2 + WORK_DAYS, work_days2));
}

TEST(Database, DatabaseCreation)
{
    WorkerDatabase db1 = create_db(EMPTY_ENTRY);
    WorkerDatabase db2 = create_db(TEST_ENTRY);

    EXPECT_EQ(db1.entry_number, 0);
    EXPECT_EQ(db1.entry_capacity, INIT_DATABASE_LENGTH);
    EXPECT_EQ(db1.entry_mem_size, sizeof(WorkerEntry));
    EXPECT_NE(db1.disk_storage, nullptr);

    EXPECT_NE(db2.disk_storage, nullptr);

    cleanup_db(&db1);
    cleanup_db(&db2);
}

TEST(Database, DatabaseExpansion)
{
    WorkerDatabase db_test = create_db(EMPTY_ENTRY);
    EXPECT_FALSE(expand_database(&db_test));

    db_test.entry_number = db_test.entry_capacity;
    EXPECT_TRUE(expand_database(&db_test));

    db_test.entry_capacity = 0;
    db_test.entry_number = 0;
    expand_database(&db_test);
    EXPECT_GT(db_test.entry_capacity, 0);

    cleanup_db(&db_test);
}

TEST(Database, DatabaseEntryPopulationEmpty)
{
    WorkerDatabase db_test = create_db(TEST_ENTRY);

    ASSERT_EQ(populate_entries(&db_test), EXIT_SUCCESS);

    cleanup_db(&db_test);
}
 
TEST(DatabaseManipulation, AddEntry)
{
    WorkerDatabase db = initialize_db(TEST_ENTRY, TEST_RESOURCE1);
    
    WorkDay days1 [WORK_DAYS] = {MON, TUE};
    WorkDay days2 [WORK_DAYS] = {WED, THU};
    WorkDay days3 [WORK_DAYS] = {MON};

    WorkerEntry entry1 = create_entry("John", "Work Street 1.", days1, 2);
    WorkerEntry entry2 = create_entry("Peter", "Work Street 2.", days2, 2);
    WorkerEntry entry3 = create_entry("Sarah", "Work street 3.", days3, 1);

    EXPECT_TRUE(add_entry(&db, &entry1));
    EXPECT_TRUE(add_entry(&db, &entry2));
    EXPECT_FALSE(add_entry(&db, &entry3));

    WorkerDatabase db_copy = create_db(EMPTY_ENTRY);
    copy_database(&db_copy, &db);
    cleanup_db(&db);

    WorkerDatabase db_new = initialize_db(TEST_ENTRY, TEST_RESOURCE1);

    ASSERT_EQ(db_copy.entry_number, db_new.entry_number);
    EXPECT_EQ(memcmp(db_copy.entries, db_new.entries, db_new.entry_number*db_new.entry_mem_size), 0);
    EXPECT_EQ(memcmp(db_copy.jobs_avaliable, db_new.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanup_db(&db_new);
}

TEST(DatabaseManipulation, LookupEntry)
{
    WorkerDatabase db = initialize_db(TEST_ENTRY, TEST_RESOURCE1);
    bool john_found = false;
    bool peter_found = false;
    const char jname[NAME_MAX_LENGTH] = "John";
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char jaddress [ADDRESS_MAX_LENGTH] = "Work Street 1.";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    WorkDay jworking [WORK_DAYS] = {MON, TUE};
    WorkDay pworking [WORK_DAYS] = {WED, THU};

    ASSERT_EQ(db.entry_number, 2);

    WorkerEntry john = lookup(&db, jname, jaddress, &john_found);
    WorkerEntry peter = lookup(&db, pname, paddress, &peter_found);

    ASSERT_TRUE(peter_found); ASSERT_TRUE(john_found);
    EXPECT_STREQ(john.name, jname); EXPECT_STREQ(john.address, jaddress); 
    EXPECT_STREQ(peter.name, pname);EXPECT_STREQ(peter.address, paddress);
    ASSERT_EQ(peter.number_of_days, 2); ASSERT_EQ(peter.number_of_days, 2);
    ASSERT_EQ(memcmp(peter.days_working, pworking, 2), 0);
    ASSERT_EQ(memcmp(john.days_working, jworking, 2),  0);

    cleanup_db(&db);
}

TEST(DatabaseManipulation, RemoveLastEntry)
{
    WorkerDatabase db = initialize_db(TEST_ENTRY, TEST_RESOURCE1);

    bool peter_found = false;
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    WorkDay pworking [WORK_DAYS] = {WED, THU};
    int original_entries = db.entry_number;
    int original_jobs_avaliable [WORK_DAYS];

    memcpy(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS*sizeof(int));

    int peter_index = index_lookup(&db, pname, paddress, &peter_found);
    ASSERT_TRUE(peter_found);

    bool peter_removed = remove_entry(&db, peter_index);
    EXPECT_TRUE(peter_removed);
    EXPECT_EQ(original_entries - 1, db.entry_number);
    EXPECT_NE(memcmp(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    index_lookup(&db, pname, paddress, &peter_found);
    EXPECT_FALSE(peter_found);

    WorkerEntry peter_again = create_entry(pname, paddress, pworking, 2);
    add_entry(&db, &peter_again);

    WorkerDatabase db_copy = create_db(TEST_ENTRY);
    copy_database(&db_copy, &db);
    cleanup_db(&db);

    WorkerDatabase db_new = initialize_db(TEST_ENTRY, TEST_RESOURCE1);
    EXPECT_EQ(db_new.entry_number, db_copy.entry_number);
    EXPECT_EQ(db_new.entry_capacity, db_copy.entry_capacity);
    EXPECT_EQ(memcmp(db_new.entries, db_copy.entries, db_copy.entry_number*sizeof(WorkerEntry)), 0);
    EXPECT_EQ(memcmp(db_new.jobs_avaliable, db_copy.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanup_db(&db_new);
    free(db_copy.entries);
}

TEST(DatabaseManipulation, RemoveAnyEntry)
{
    WorkerDatabase db = initialize_db(TEST_ENTRY, TEST_RESOURCE1);

    bool john_found = false;
    bool peter_found = false;
    const char jname[NAME_MAX_LENGTH] = "John";
    const char pname[NAME_MAX_LENGTH] = "Peter";
    const char jaddress [ADDRESS_MAX_LENGTH] = "Work Street 1.";
    const char paddress [ADDRESS_MAX_LENGTH] = "Work Street 2.";
    int original_entries = db.entry_number;
    int original_jobs_avaliable [WORK_DAYS];
    memcpy(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS);

    int john_index = index_lookup(&db, jname, jaddress, &john_found);
    int peter_index = index_lookup(&db, pname, paddress, &peter_found);

    bool john_removed = remove_entry(&db, john_index);
    EXPECT_TRUE(john_removed);
    EXPECT_EQ(db.entry_number, original_entries - 1);
    EXPECT_NE(memcmp(original_jobs_avaliable, db.jobs_avaliable, WORK_DAYS), 0);

    int peter_new_index = index_lookup(&db, pname, paddress, &peter_found);
    EXPECT_TRUE(peter_found);
    EXPECT_NE(peter_new_index, peter_index);

    WorkerDatabase db_copy = create_db(TEST_ENTRY);
    copy_database(&db_copy, &db);
    cleanup_db(&db);

    WorkerDatabase db_new = initialize_db(TEST_ENTRY, TEST_RESOURCE1);

    EXPECT_EQ(db_new.entry_number, db_copy.entry_number);
    EXPECT_EQ(db_new.entry_capacity, db_copy.entry_capacity);
    EXPECT_EQ(memcmp(db_new.entries, db_copy.entries, db_copy.entry_number*sizeof(WorkerEntry)), 0);
    EXPECT_EQ(memcmp(db_new.jobs_avaliable, db_copy.jobs_avaliable, WORK_DAYS*sizeof(int)), 0);

    cleanup_db(&db_new);
    free(db_copy.entries);
}
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
