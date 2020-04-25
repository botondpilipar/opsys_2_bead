extern "C"
{
    #include <database_macros.h>
    #include <worker_database_managers.h>
}
#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <filesystem>

#define TEST_RESOURCE1 "resources/test_resource/employed_per_day1.txt"
#define TEST_RESOURCE2 "resources/test_resource/employed_per_day2.txt"
#define INVALID_RESOURCE "resources/test_resource/non_existing.txt"
#define TEST_ENTRY "resources/test_resource/test_entries.bin"
#define EMPTY_ENTRY "resources/test_resource/test_empty_entries.bin"
#define FIXTURE_TEST_ENTRY "resources/test_resource/fixture_entry.bin"
