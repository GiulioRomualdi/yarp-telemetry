# Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

add_library(telemetry)
add_library(telemetry::telemetry ALIAS telemetry)

set(telemetry_HDRS
  telemetry/Buffer.h
  telemetry/BufferConfig.h
  telemetry/BufferManager.h
  telemetry/Record.h
  telemetry/TreeNode.h
)
set(telemetry_SRCS
  telemetry/BufferConfig.cpp
)
set(telemetry_IMPL_HDRS )
set(telemetry_IMPL_SRCS )

source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}"
  PREFIX "Source Files"
  FILES
    ${telemetry_SRCS}
    ${telemetry_IMPL_SRCS}
)
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}"
  PREFIX "Header Files"
  FILES
    ${telemetry_HDRS}
    ${telemetry_IMPL_HDRS}
)

target_sources(telemetry
  PRIVATE
    ${telemetry_SRCS}
    ${telemetry_HDRS}
    ${telemetry_IMPL_SRCS}
    ${telemetry_IMPL_HDRS}
)


target_include_directories(telemetry
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(telemetry PUBLIC cxx_std_17)

target_link_libraries(telemetry PUBLIC Boost::boost
                                       matioCpp::matioCpp
                                       Threads::Threads
                                PRIVATE nlohmann_json::nlohmann_json)
# Support using filesystem on GCC < 9.1,
# see https://en.cppreference.com/w/cpp/filesystem#Notes
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1))
  target_link_libraries(telemetry PUBLIC stdc++fs)
endif()
list(APPEND telemetry_PUBLIC_DEPS Boost
                                  matioCpp
                                  Threads)
list(APPEND telemetry_PRIVATE_DEPS nlohmann_json)

set_property(TARGET telemetry PROPERTY PUBLIC_HEADER ${telemetry_HDRS})
set_property(TARGET telemetry PROPERTY PRIVATE_HEADER ${telemetry_IMPL_HDRS})
set_property(TARGET telemetry PROPERTY VERSION ${VERSION_SHORT})
set_property(TARGET telemetry PROPERTY SOVERSION 0)
set_property(TARGET telemetry PROPERTY FOLDER "Libraries")

install(
  TARGETS telemetry
  EXPORT telemetry
  RUNTIME
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT telemetry
  LIBRARY
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT telemetry
    NAMELINK_COMPONENT telemetry-dev
  ARCHIVE
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT telemetry-dev
  PUBLIC_HEADER
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/telemetry"
    COMPONENT telemetry-dev
  PRIVATE_HEADER
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/telemetry/impl"
    COMPONENT telemetry-priv-dev
)

set(telemetry_PUBLIC_DEPS ${telemetry_PUBLIC_DEPS} PARENT_SCOPE)
set(telemetry_PRIVATE_DEPS ${telemetry_PRIVATE_DEPS} PARENT_SCOPE)
