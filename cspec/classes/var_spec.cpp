#include <core.hpp>
#include <diagnostics.hpp>
#include <iostream>

#include <cspec.hpp>

void insert_data_into_var()
{
    var arr = empty_array;

    1'000'000_times([&]() {
       arr.push_back(1);
    });
}

void insert_data_into_vector()
{
    std::vector<double> arr;

    1'000'000_times([&]() {
       arr.push_back(1);
    });
}

using namespace uva::diagnostics;

cspec_describe("uva::core::var",
    describe("metrics",   
        it("var-array should not take much slower than std::vector", []() {
            expect(insert_data_into_var).to_not execute_slower_than(insert_data_into_vector).in(10_percent);
        })
    )
);