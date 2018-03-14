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

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ngraph/log.hpp"

namespace ngraph
{
    class Function;
    class Node;

    namespace op
    {
        class Parameter;
        class Result;
    }

    enum class Placement
    {
        DEFAULT,
        INTERPRETER,
        CPU,
        GPU,
        ARGON,
    };

    class Cluster
    {
    public:
        Cluster();
        Cluster(const std::unordered_set<std::shared_ptr<Node>>& nodes);
        // Nodes
        void insert_node(const std::shared_ptr<Node>& node);
        const std::unordered_set<std::shared_ptr<Node>>& get_nodes() const { return m_nodes; }
        void clear_nodes() { m_nodes.clear(); }
        size_t size() const { return m_nodes.size(); }

    protected:
        size_t m_instance_id;
        static std::atomic<size_t> m_next_instance_id;
        std::unordered_set<std::shared_ptr<Node>> m_nodes;
    };

    namespace cluster_util
    {
        // Cluster graph utils
        std::vector<std::shared_ptr<Cluster>>
            split_function_to_clusters(const std::shared_ptr<Function>& f);
    }

    std::string placement_to_string(Placement placement);

    // Split function to function(s) with unique placement
    std::pair<std::vector<std::shared_ptr<Function>>,
              std::unordered_map<std::shared_ptr<op::Parameter>, std::shared_ptr<op::Result>>>
        split_function_by_placement(std::shared_ptr<Function> f);

}
