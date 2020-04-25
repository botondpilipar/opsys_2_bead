#include "database_manager_test.h"
extern "C"
{
    #include "../console_interface/input_control.h"
}

TEST(InputControl, NameValidation)
{
    EXPECT_TRUE(is_name("Petofi Sandor"));
    EXPECT_TRUE(is_name("John Lenon"));
    EXPECT_TRUE(is_name("Peter"));
    EXPECT_TRUE(is_name("Name\tTabs"));

    EXPECT_FALSE(is_name(" "));
    EXPECT_FALSE(is_name("   "));
    EXPECT_FALSE(is_name(""));
    EXPECT_FALSE(is_name("Peter 1234"));
    EXPECT_FALSE(is_name("\n\r"));
    EXPECT_FALSE(is_name(nullptr));
}

TEST(InputControl, AddressValidation)
{
    EXPECT_TRUE(is_address("Munka utca 1."));
    EXPECT_TRUE(is_address("Work Street 3."));
    EXPECT_TRUE(is_address("Budapest XVII. Jozsef utca 27."));
    EXPECT_TRUE(is_address("Jozsef \n utca \n 3."));

    EXPECT_FALSE(is_address(""));
    EXPECT_FALSE(is_address("    "));
    EXPECT_FALSE(is_address(nullptr));
    EXPECT_FALSE(is_address("3. Maria 4."));
    EXPECT_FALSE(is_address("Jozsef utca"));
    EXPECT_FALSE(is_address("utca"));
}
TEST(InputControl, WorkingDayParsing)
{
    WorkDay days[WORK_DAYS];
    WorkDay all_days [7] = {MON, TUE, WED, THU, FRI, SAT, SUN};
    int input_max_length = WORK_DAYS*MAX_DAY_STR_LENGTH + 7;
    int number_of_days;

    char every_day [input_max_length];
    strcpy(every_day, "hétfő kedd szerda csütörtök péntek szombat vasárnap");
    char only_mon [input_max_length];
    strcpy(only_mon, "hétfő hétfő hétfő hétfő hétfő hétfő");
    char only_ascii [input_max_length];
    strcpy(only_ascii, "hetfo kedd szerda");
    char wrong_input [input_max_length];
    strcpy(wrong_input, "abcdefghijklmn");

    EXPECT_TRUE(to_working_days(every_day, days, &number_of_days));

    for(int i = 0; i<WORK_DAYS; ++i)
    {
        EXPECT_EQ(days[i], all_days[i]);
    }

    EXPECT_TRUE(to_working_days(only_mon, days, &number_of_days));

    EXPECT_FALSE(to_working_days(only_ascii, days, &number_of_days));
    EXPECT_FALSE(to_working_days(wrong_input, days, &number_of_days));
}
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}