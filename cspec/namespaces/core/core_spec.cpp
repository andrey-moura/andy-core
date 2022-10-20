#include <core.hpp>
#include <cspec.hpp>

cspec_describe("uva::core",

    describe("operator\"\"_times",
        it("should run correct amount of times",  []() {
            int i = 0;
            
            5_times([&]() {
                i++;
            });

            expect(i).to eq(5);
        })
    )

);