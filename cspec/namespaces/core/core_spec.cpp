#include <uva/core.hpp>
#include <faker.hpp>

// cspec must be included after ALL headers
#include <cspec.hpp>

using namespace uva;
using namespace faker;

cspec_describe("uva::core",
    describe("var", {
        context("initializated", {
            context("with default constructor", {
                it("should have null type", [](){
                    var v;
                    expect(v.type).to eq(var::var_type::null_type);
                })
                it("value_ptr should be nullptr", [](){
                    var v;
                    expect(v.m_value_ptr).to eq(nullptr);
                })
            })
            context("with bool copy constructor", {
                it("should have integer type", [](){
                    bool b = faker::boolean();
                    var v(b);
                    expect(v.type).to eq(var::var_type::integer);
                })
                it("should have value_ptr NOT nullptr", [](){
                    bool b = faker::boolean();
                    var v(b);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied boolean", [](){
                    bool b = faker::boolean();
                    var v(b);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(b);
                })
            })
            context("with int copy constructor", {
                it("should have integer type", [](){
                    int i = faker::number::integer();
                    var v(i);
                    expect(v.type).to eq(var::var_type::integer);
                })
                it("should have value_ptr NOT nullptr", [](){
                    int i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied integer", [](){
                    int i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(i);
                })
            })
            context("with uint64_t copy constructor", {
                it("should have integer type", [](){
                    uint64_t i = faker::number::integer();
                    var v(i);
                    expect(v.type).to eq(var::var_type::integer);
                })
                it("should have value_ptr NOT nullptr", [](){
                    uint64_t i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied uint64_t", [](){
                    uint64_t i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(i);
                })
            })
            context("with time_t copy constructor", {
                it("should have integer type", [](){
                    time_t i = faker::number::integer();
                    var v(i);
                    expect(v.type).to eq(var::var_type::integer);
                })
                it("should have value_ptr NOT nullptr", [](){
                    time_t i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied integer", [](){
                    time_t i = faker::number::integer();
                    var v(i);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(i);
                })
            })
            context("with double copy constructor", {
                it("should have real type", [](){
                    double d = faker::number::integer();
                    var v(d);
                    expect(v.type).to eq(var::var_type::real);
                })
                it("should have value_ptr NOT nullptr", [](){
                    double d = faker::number::integer();
                    var v(d);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied double", [](){
                    double d = faker::number::integer();
                    var v(d);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(d);
                })
            })
            context("with color copy constructor", {
                it("should have color type", [](){
                    color c = faker::drawing::color();
                    var v(c);
                    expect(v.type).to eq(var::var_type::color);
                })
                it("should have value_ptr NOT nullptr", [](){
                    color c = faker::drawing::color();
                    var v(c);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("value_ptr should have same memory as copied color", [](){
                    color c = faker::drawing::color();
                    var v(c);
                    expect(v.m_value_ptr).to point_to_mem_block_equal_of(c);
                })
            })
            context("with const char* copy constructor", {
                it("should have string type", [](){
                    const char* s = faker::lorem::cword();
                    var v(s);
                    expect(v.type).to eq(var::var_type::string);
                })
                it("should have value_ptr NOT nullptr", [](){
                    const char* s = faker::lorem::cword();
                    var v(s);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("as string should have same value as copied string", [](){
                    const char* s = faker::lorem::cword();
                    var v(s);
                    expect(v.as<var::string>()).to eq(s);
                })
            })
            context("with const char8_t* copy constructor", {
                it("should have string type", [](){
                    const char8_t* s = (char8_t*)faker::lorem::cword();
                    var v(s);
                    expect(v.type).to eq(var::var_type::string);
                })
                it("should have value_ptr NOT nullptr", [](){
                    const char8_t* s = (char8_t*)faker::lorem::cword();
                    var v(s);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("as string should have same value as copied string", [](){
                    const char8_t* s = (char8_t*)faker::lorem::cword();
                    var v(s);
                    expect(v).to eq(s);
                })
            })
            context("with const char*,size_t copy constructor", {
                it("should have string type", [](){
                    std::string_view s = faker::lorem::word();
                    var v(s.data(), s.size());
                    expect(v.type).to eq(var::var_type::string);
                })
                it("should have value_ptr NOT nullptr", [](){
                    std::string_view s = faker::lorem::word();
                    var v(s.data(), s.size());
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("as string should have same value as copied string", [](){
                    std::string_view s = faker::lorem::word();
                    var v(s.data(), s.size());
                    expect(v.as<var::string>()).to eq(s);
                })
            })
            context("with const std::string& copy constructor", {
                it("should have string type", [](){
                    std::string s = faker::lorem::word();
                    var v(s);
                    expect(v.type).to eq(var::var_type::string);
                })
                it("should have value_ptr NOT nullptr", [](){
                    std::string s = faker::lorem::word();
                    var v(s);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("as string should have same value as copied string", [](){
                    std::string s = faker::lorem::word();
                    var v(s);
                    expect(v.as<var::string>()).to eq(s);
                })
            })
            context("with const std::vector& copy constructor", {
                it("should have string type", [](){
                    std::vector<var> a = faker::array::random<var>(5);
                    var v(a);
                    expect(v.type).to eq(var::var_type::array);
                })
                it("should have value_ptr NOT nullptr", [](){
                    std::vector<var> a = faker::array::random<var>(5);
                    var v(a);
                    expect(v.m_value_ptr).to_not eq(nullptr);
                })
                it("as string should have same value as copied string", [](){
                    std::vector<var> a = faker::array::random<var>(5);
                    var v(a);
                    expect(v.as<var::array>()).to eq(a);
                })
            })
            // context("with const std::string& copy constructor", {
            //     it("should have string type", [](){
            //         std::string s = faker::lorem::word();
            //         var v(s);
            //         expect(v.type).to eq(var::var_type::string);
            //     })
            //     it("should have value_ptr NOT nullptr", [](){
            //         std::string s = faker::lorem::word();
            //         var v(s);
            //         expect(v.m_value_ptr).to_not eq(nullptr);
            //     })
            //     it("as string should have same value as copied string", [](){
            //         std::string s = faker::lorem::word();
            //         var v(s);
            //         expect(v.as<var::string>()).to eq(s);
            //     })
            // })
            // context("assigning var_type::", {
            //     describe("string", {
            //         context("std::string", {
            //             it("should have string type", [](){
            //                 var v = faker::lorem::word();
            //                 expect(v.type).to eq(var::var_type::string);
            //             })
            //             it("should have value_ptr NOT nullptr", [](){
            //                 var v = faker::lorem::word();
            //                 expect(v.m_value_ptr).to_not eq(nullptr);
            //             })
            //             it("should have correct value", [](){
            //                 std::string s = faker::lorem::word();
            //                 var v = s; 
            //                 expect(v).to eq(s);
            //             })
            //         })
            //     })
            //     describe("integer", {
            //         context("int", {
            //             it("should have integer type", [](){
            //                 var v = faker::number::integer();
            //                 expect(v.type).to eq(var::var_type::integer);
            //             })
            //             it("should have value_ptr NOT nullptr", [](){
            //                 var v = faker::number::integer();
            //                 expect(v.m_value_ptr).to_not eq(nullptr);
            //             })
            //             it("should have correct value", [](){
            //                 int i = faker::number::integer();
            //                 var v = i; 
            //                 expect(v).to eq(i);
            //             })
            //         })
            //     })
            //     describe("real", {
            //         context("double", {
            //             it("should have real type", [](){
            //                 var v = faker::number::real();
            //                 expect(v.type).to eq(var::var_type::real);
            //             })
            //             it("should have value_ptr NOT nullptr", [](){
            //                 var v = faker::number::real();
            //                 expect(v.m_value_ptr).to_not eq(nullptr);
            //             })
            //             it("should have correct value", [](){
            //                 double d = faker::number::real();
            //                 var v = d; 
            //                 expect(v).to eq(d);
            //             })
            //         })
            //     })
            //    describe("array", {
            //         context("std::vector<int>", {
            //             it("should have array type", [](){
            //                 var v = faker::array::random<int>(5);
            //                 expect(v.type).to eq(var::var_type::array);
            //             })
            //             it("should have value_ptr NOT nullptr", [](){
            //                 var v = faker::array::random<int>(5);
            //                 expect(v.m_value_ptr).to_not eq(nullptr);
            //             })
            //             it("should have correct value", [](){
            //                 std::vector<int> array = faker::array::random<int>(5);
            //                 var v = array; 
            //                 expect(v).to eq(array);
            //             })
            //         })
            //     })
            // })
        })
    })
);