/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef TELEMETRY_TREE_NODE_H
#define TELEMETRY_TREE_NODE_H

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <regex>
#include <deque>
#include <assert.h>

namespace telemetry {

/**
 * @brief A class to represent the buffer of telemetry::Record<T>.
 *
 */
template<class T>
class TreeNode
{
public:
    TreeNode () = default;

    TreeNode(std::shared_ptr<T> _value) {
        m_value = _value;
    }

    [[nodiscard]]
    bool childExists(const std::string& name) const {
        return m_children.find(name) != m_children.end();
    }

    bool addChild(const std::string& name, std::shared_ptr<TreeNode<T>> node) {

        if(this->childExists(name)) {
            std::cout << "The TreeNode named " << name << " already exist." << std::endl;
            return false;
        }

        m_children.insert({name, node});

        return true;
    }

    bool addChild(const std::string& name) {
        return this->addChild(name, std::make_shared<TreeNode<T>>());
    }

    std::weak_ptr<TreeNode<T>> getChild(const std::string& name) {
        if(this->childExists(name)) {
            return m_children.at(name);
        }

        return std::make_shared<TreeNode<T>>();
    }

    std::shared_ptr<T> getValue() {
        return m_value;
    }

    const std::unordered_map<std::string, std::shared_ptr<TreeNode>>& getChildren() const
    {
        return m_children;
    }

    [[nodiscard]]
    std::string toString(const std::string& name= ".", const unsigned int depth = 0) const
    {
        std::ostringstream oss;
        for (unsigned int i = 0 ; i < depth ; i++) {
            if (i != depth-1) {
                oss <<  "    ";
            } else {
                oss <<  "|-- ";
            }
        }
        oss  << name << std::endl;
        for (const auto & [key, child] : m_children) {
            oss << child->toString(key, depth + 1);
        }

        return oss.str();
    }

    bool addLeaf(const std::string& name) {
        auto nodes = this->split(name, m_stringSeparator);
        return this->addLeaf(nodes, nullptr);
    }

    bool addLeaf(const std::string& name, std::shared_ptr<T> element) {
        auto nodes = this->split(name, m_stringSeparator);
        return this->addLeaf(nodes, element);
    }

    bool addLeaf(const std::deque<std::string>& nodes, std::shared_ptr<T> element) {
        std::deque<std::string> tmp = nodes;
        return this->addLeaf(tmp, element);
    }

    std::weak_ptr<T> getLeaf(const std::string& name) {
        auto nodes = this->split(name, m_stringSeparator);

        auto leafPtr = this->getLeaf(nodes).lock();
        if (leafPtr == nullptr) {
            return std::make_shared<T>();
        }

        return leafPtr->getValue();
    }

    std::weak_ptr<T> getLeaf(const std::deque<std::string>& nodes) {
        std::deque<std::string> tmp = nodes;

        auto leafPtr = this->getLeaf(tmp).lock();
        if(leafPtr == nullptr) {
            return std::make_shared<T>();
        }

        return leafPtr->getValue();
    }

    [[nodiscard]]
    bool empty() const {
        return m_children.empty() || m_value == nullptr;
    }

private:

    static std::string m_stringSeparator;
    std::shared_ptr<T> m_value;
    std::unordered_map<std::string, std::shared_ptr<TreeNode>> m_children;

    // function to split a string into substring given a delimiter
    static std::deque<std::string> split(const std::string& input, const std::string& regex) {
        std::regex re(regex);
        std::sregex_token_iterator first{input.begin(), input.end(), re, -1}, last;
        return {first, last};
    };

    bool addLeaf(std::deque<std::string>& nodes, std::shared_ptr<T> element) {
        const auto& node = nodes.front();

        if(nodes.size() == 1) {
            if(!this->addChild(node, std::make_shared<TreeNode<T>>(element))) {
                return false;
            }
            return true;
        }
        if (!this->childExists(node)) {
            if (!this->addChild(node)) {
                return false;
            }
        }

        nodes.pop_front();
        return this->getChild(node).lock()->addLeaf(nodes, element);
    }

    std::weak_ptr<TreeNode<T>> getLeaf(std::deque<std::string>& nodes)
    {
        const auto& nodeName = nodes.front();
        auto ptr = this->getChild(nodeName).lock();

        if (ptr == nullptr) {
            return std::make_shared<TreeNode<T>>();
        }

        if (nodes.size() == 1) {
            return ptr;
        }

        nodes.pop_front();
        return ptr->getLeaf(nodes);
    }
};

template<class T>
std::string TreeNode<T>::m_stringSeparator = "::";

} // telemetry

#endif
