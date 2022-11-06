// #include <core.hpp>
// #include <diagnostics.hpp>
// #include <iostream>

// #include <cspec.hpp>

// #define TEST_REPETITIONS 1'000'000

// void insert_data_into_var()
// {
//     var arr = empty_array;

//     for(size_t i = i; i < TEST_REPETITIONS; ++i)
//     {
//        arr.push_back("test");
//     };
// }

// void insert_data_into_vector()
// {
//     std::vector<const char*> arr;

//     for(size_t i = i; i < TEST_REPETITIONS; ++i)
//     {
//        arr.push_back("test");
//     };
// }

// void assign_value_to_std_string()
// {
//     std::string s;

//     for(size_t i = i; i < TEST_REPETITIONS; ++i)
//     {
//        s = "test";
//     };
// }

// void assign_value_to_var_string()
// {
//     var s;

//     for(size_t i = i; i < TEST_REPETITIONS; ++i)
//     {
//        s = "test";
//     };
// }

// using namespace uva::diagnostics;

// static const auto max_difference_between_var_and_std = 25_percent;

// cspec_describe("uva::core::var",
//     describe("metrics",   
//         it("var-array should not take much slower than std::vector", []() {
//             expect(insert_data_into_var).to_not execute_slower_than(insert_data_into_vector).in(max_difference_between_var_and_std);
//         })

//         it("var-string should not take much slower than std::string to assign", []() {
//             expect(assign_value_to_var_string).to_not execute_slower_than(assign_value_to_std_string).in(max_difference_between_var_and_std);
//         })
//     )
// );