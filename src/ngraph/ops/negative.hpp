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

#include "ngraph/ops/util/unary_elementwise_arithmetic.hpp"

namespace ngraph
{
    namespace op
    {
        /// \brief Elementwise negative operation.
        class Negative : public util::UnaryElementwiseArithmetic
        {
        public:
            /// \brief Constructs a negative operation.
            ///
            /// \param arg Node that produces the input tensor.
            Negative(const std::shared_ptr<Node>& arg)
                : UnaryElementwiseArithmetic("Negative", arg)
            {
            }

            virtual std::shared_ptr<Node>
                copy_with_new_args(const NodeVector& new_args) const override
            {
                if (new_args.size() != 1)
                {
                    throw ngraph_error("Incorrect number of new arguments");
                }
                return std::make_shared<Negative>(new_args.at(0));
            }

            virtual void generate_adjoints(autodiff::Adjoints& adjoints,
                                           const std::shared_ptr<Node>& delta) override;
        };
    }
    inline std::shared_ptr<ngraph::Node> operator-(const std::shared_ptr<ngraph::Node> arg0)
    {
        return std::make_shared<ngraph::op::Negative>(arg0);
    }
}
