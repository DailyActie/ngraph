/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "ngraph/codegen/compiler.hpp"
#include "ngraph/codegen/execution_engine.hpp"
#include "ngraph/util.hpp"

using namespace std;
using namespace ngraph;

void compile(const string& source)
{
}

TEST(benchmark, compile_time)
{
    cout << "All times in milliseconds\n";
    stopwatch timer;
    size_t compile_count = 10;
    {
        string source = R"()";
        timer.start();
        for (size_t i = 0; i < compile_count; i++)
        {
            codegen::Compiler compiler;
            auto module = compiler.compile(source);
            ASSERT_NE(nullptr, module);
        }
        timer.stop();
        cout << "empty source\n";
        cout << setw(10) << fixed << showpoint << setprecision(1)
             << static_cast<double>(timer.get_milliseconds()) / static_cast<double>(compile_count)
             << "\n";
    }

    vector<string> includes = {
        R"(#include <cmath>)",
        R"(#include <Eigen/Dense>)",
        R"(#include <tbb/flow_graph.h>)",
        R"(#include "ngraph/runtime/aligned_buffer.hpp")",
        R"(#include "ngraph/runtime/cpu/cpu_eigen_utils.hpp")",
        R"(#include "ngraph/runtime/cpu/cpu_kernels.hpp")",
        R"(#include "ngraph/runtime/kernel/avg_pool.hpp")",
        R"(#include "ngraph/runtime/kernel/broadcast.hpp")",
        R"(#include "ngraph/runtime/kernel/concat.hpp")",
        R"(#include "ngraph/runtime/kernel/convolution.hpp")",
        R"(#include "ngraph/runtime/kernel/dot.hpp")",
        R"(#include "ngraph/runtime/kernel/max_pool.hpp")",
        R"(#include "ngraph/runtime/kernel/not.hpp")",
        R"(#include "ngraph/runtime/kernel/one_hot.hpp")",
        R"(#include "ngraph/runtime/kernel/pad.hpp")",
        R"(#include "ngraph/runtime/kernel/reduce.hpp")",
        R"(#include "ngraph/runtime/kernel/reduce_window.hpp")",
        R"(#include "ngraph/runtime/kernel/replace_slice.hpp")",
        R"(#include "ngraph/runtime/kernel/reshape.hpp")",
        R"(#include "ngraph/runtime/kernel/reverse.hpp")",
        R"(#include "ngraph/runtime/kernel/select_and_scatter.hpp")",
        R"(#include "ngraph/runtime/kernel/slice.hpp")",
        R"(#include "ngraph/runtime/kernel/sum.hpp")",
        R"(#include "ngraph/util.hpp")"};

    cout << "\nno precompiled headers\n";
    for (const string& include : includes)
    {
        string source = include;
        timer.start();
        for (size_t i = 0; i < compile_count; i++)
        {
            codegen::Compiler compiler;
            auto module = compiler.compile(source);
            ASSERT_NE(nullptr, module);
        }
        timer.stop();
        cout << setw(10) << fixed << showpoint << setprecision(1)
             << static_cast<double>(timer.get_milliseconds()) / static_cast<double>(compile_count);
        cout << "  " << source << "\n";
    }
}

TEST(DISABLED_codegen, simple_return)
{
    constexpr auto source = R"(extern "C" int test() { return 2+5; })";

    codegen::Compiler compiler;
    codegen::ExecutionEngine execution_engine;

    auto module = compiler.compile(source);
    ASSERT_NE(nullptr, module);

    execution_engine.add_module(module);

    execution_engine.finalize();

    auto func = execution_engine.find_function<int()>("test");
    ASSERT_NE(nullptr, func);

    int result = func();
    EXPECT_EQ(7, result);
}

TEST(DISABLED_codegen, pass_args)
{
    constexpr auto source = R"(extern "C" int test(int a, int b) { return a+b; })";

    codegen::Compiler compiler;
    codegen::ExecutionEngine execution_engine;

    auto module = compiler.compile(source);
    ASSERT_NE(nullptr, module);

    execution_engine.add_module(module);

    execution_engine.finalize();

    auto func = execution_engine.find_function<int(int, int)>("test");
    ASSERT_NE(nullptr, func);

    int result = func(20, 22);
    EXPECT_EQ(42, result);
}

TEST(DISABLED_codegen, include)
{
    constexpr auto source =
        R"(
        #include <cmath>
        extern "C" int test(int a, int b)
        {
            return (int)pow((double)a,(double)b);
        }
    )";

    codegen::Compiler compiler;
    codegen::ExecutionEngine execution_engine;

    auto module = compiler.compile(source);
    ASSERT_NE(nullptr, module);

    execution_engine.add_module(module);

    execution_engine.finalize();

    auto func = execution_engine.find_function<int(int, int)>("test");
    ASSERT_NE(nullptr, func);

    int result = func(20, 2);
    EXPECT_EQ(400, result);
}
