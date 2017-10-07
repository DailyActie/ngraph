// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#pragma once

#include <functional>

#include "ngraph/pass/pass.hpp"
#include "ngraph/pattern/matcher.hpp"
namespace ngraph
{
    namespace pass
    {
        class GraphRewrite;
    }
    using gr_callback_fn = std::function<void(std::shared_ptr<pattern::Matcher> m, std::shared_ptr<Node> match_root)>;
}

class ngraph::pass::GraphRewrite : public CallGraphPass
{
public:
    GraphRewrite() : CallGraphPass() {};
    virtual void process_match() = 0;

    void add_matcher_callback_pair(std::shared_ptr<pattern::Matcher> m, gr_callback_fn callback)
    {
        m_matcher_callback_pairs.push_back(std::make_pair(m, callback));
    };
    virtual bool run_on_call_graph(std::list<Node*>&) override;


private:
    //enable cascading rewrites
    std::vector<std::pair<std::shared_ptr<pattern::Matcher>, gr_callback_fn>> m_matcher_callback_pairs;

private:
};