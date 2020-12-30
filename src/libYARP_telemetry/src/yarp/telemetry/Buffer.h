/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_TELEMETRY_BUFFER_H
#define YARP_TELEMETRY_BUFFER_H

#include <yarp/telemetry/Record.h>
#include <boost/circular_buffer.hpp>
#include <cstring>
#include <vector>
#include <memory>

namespace yarp::telemetry {

template<class T>
class Buffer {
public:

    using iterator       =  typename boost::circular_buffer<Record<T>>::iterator;
    using const_iterator =  typename boost::circular_buffer<Record<T>>::const_iterator;

    Buffer() = delete;

    Buffer(const Buffer&) = default;

    Buffer(Buffer&&) noexcept = default;

    Buffer<T>& operator=(const Buffer<T>&) = default;

    Buffer<T>& operator=(Buffer<T>&&) noexcept = default;

    virtual ~Buffer() = default;

    Buffer(size_t num_elements,  const std::vector<size_t>& dimensions,
        const std::string& name): m_buffer_ptr(std::make_shared<boost::circular_buffer<Record<T>>>(num_elements)),
                                  m_dimensions(dimensions), m_variable_name(name) {

    }

    Buffer(size_t num_elements, const std::string& name): m_buffer_ptr(std::make_shared<boost::circular_buffer<Record<T>>>(num_elements)),
                                                          m_variable_name(name) {

    }

    // TODO: enforced Record<T> ??
    // TODO: check if I am pushing a vector with the right dimensions
    inline void push_back(const Record<T> &elem)
    {
        m_buffer_ptr->push_back(elem);
    }

    // TODO: enforced Record<T> ??
    // TODO: check if I am pushing a vector with the right dimensions
    inline void push_back(Record<T>&& elem)
    {
        m_buffer_ptr->push_back(std::move(elem));
    }

    size_t getBufferFreeSpace() const {
        return m_buffer_ptr->capacity() - m_buffer_ptr->size();
    }

    size_t size() const {
        return m_buffer_ptr->size();
    }

    bool empty() const {
        return m_buffer_ptr->empty();
    }

    bool full() const {
        return m_buffer_ptr->full();
    }

    iterator begin() noexcept {
        return m_buffer_ptr->begin();
    }

    iterator end() noexcept {
        return m_buffer_ptr->end();
    }

    const_iterator begin() const noexcept {
        return m_buffer_ptr->begin();
    }

    const_iterator end() const noexcept {
        return m_buffer_ptr->end();
    }

    // TODO maybe this class has to be a struct??
    std::vector<size_t> getDimensions() const {
        return m_dimensions;
    }

    std::shared_ptr<boost::circular_buffer<Record<T>>> getBufferSharedPtr() const {
        return m_buffer_ptr;
    }

    std::string getVariableName() const {
        return m_variable_name;
    }

private:
    std::shared_ptr<boost::circular_buffer<Record<T>>> m_buffer_ptr;
    std::vector<size_t> m_dimensions{1,1};
    std::string m_variable_name;

};


} // yarp::telemetry

#endif
