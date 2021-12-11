/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

// This has to be probably removed when we will have multiple tests
#define CATCH_CONFIG_MAIN

#include <telemetry/TreeNode.h>

#include <catch2/catch.hpp>
#include <string>
#include <vector>
#include <regex>

TEST_CASE("Create Tree from vector of strings")
{
    std::vector<std::string> elements = {"A::B::C::D::element1",
                                         "A::B::C::D::element2",
                                         "A::B::C::element3",
                                         "A::B::F::element4"};


    auto tree = telemetry::TreeNode<double>::buildTreeFromVectorOfString(elements);
    REQUIRE(tree);
    std::cout << tree->toString() << std::endl;
}
