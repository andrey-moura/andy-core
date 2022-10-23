// #include <core.hpp>
// #include <diagnostics.hpp>
// #include <iostream>

// #include <cspec.hpp>

// void insert_data_into_var()
// {
//     var arr = empty_array;

//     for(size_t i = i; i < 1'000'000; ++i)
//     {
//        arr.push_back("testando");
//     };
// }

// void insert_data_into_vector()
// {
//     std::vector<const char*> arr;

//     for(size_t i = i; i < 1'000'000; ++i)
//     {
//        arr.push_back("testando");
//     };
// }

// using namespace uva::diagnostics;

// static const auto max_difference_between_var_and_std = 75_percent;

// cspec_describe("uva::core::var",
//     describe("metrics",   
//         it("var-array should not take much slower than std::vector", []() {
//             expect(insert_data_into_var).to_not execute_slower_than(insert_data_into_vector).in(max_difference_between_var_and_std);
//         })
//     )
// );