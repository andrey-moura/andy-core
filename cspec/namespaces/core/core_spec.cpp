#include <core.hpp>
#include <cspec.hpp>

cspec_describe("uva::core",

    describe("user-defined-literals",

        describe("operator\"\"_times",
            it("should run correct amount of times",  []() {
                int i = 0;
                
                5_times([&]() {
                    i++;
                });

                expect(i).to eq(5);
            })
        )

        describe("operator\"\"_var",
            context("operating with string",
                it("should return correct string", []() {
                    expect("some string that will be converted to var"_var).to eq("some string that will be converted to var");
                })

                it("should have correct var_type", [](){
                    expect("foo"_var.type).to eq(var::var_type::string);
                })
            )
        )

        describe("operator\"\"_percent",
            context("operating with double",
                it("should return correct per cent", []() {
                    expect(1_percent).to eq(0);
                    expect(1_percent).to eq(0.01);
                    expect(10_percent).to eq(0.1);
                    expect(25_percent).to eq(0.25);
                    expect(50_percent).to eq(0.50);
                    expect(75_percent).to eq(0.75);
                    expect(100_percent).to eq(1);
                    expect(200_percent).to eq(2);
                    expect(300_percent).to eq(3);
                })

                it("should have correct var_type", [](){
                    expect((4_percent).type).to eq(var::var_type::real);
                })
            )
        )
    )
);