# ******************************************************************************
# Copyright 2017-2018 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ******************************************************************************

# Enable ExternalProject CMake module
include(ExternalProject)

#----------------------------------------------------------------------------------------------------------
# Download and install GoogleTest ...
#----------------------------------------------------------------------------------------------------------

SET(XML_GIT_REPO_URL https://github.com/zeux/pugixml)
SET(XML_GIT_LABEL v1.8.1)

# The 'BUILD_BYPRODUCTS' argument was introduced in CMake 3.2.
if (${CMAKE_VERSION} VERSION_LESS 3.2)
    ExternalProject_Add(
        ext_xml
        GIT_REPOSITORY ${XML_GIT_REPO_URL}
        GIT_TAG ${XML_GIT_LABEL}
        # Disable install step
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        TMP_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/tmp"
        STAMP_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/stamp"
        DOWNLOAD_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/download"
        SOURCE_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/src"
        BINARY_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/build"
        INSTALL_DIR "${EXTERNAL_PROJECTS_ROOT}/xml"
        )
else()
    ExternalProject_Add(
        ext_xml
        GIT_REPOSITORY ${XML_GIT_REPO_URL}
        GIT_TAG ${XML_GIT_LABEL}
        # Disable install step
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        TMP_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/tmp"
        STAMP_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/stamp"
        DOWNLOAD_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/download"
        SOURCE_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/src"
        BINARY_DIR "${EXTERNAL_PROJECTS_ROOT}/xml/build"
        INSTALL_DIR "${EXTERNAL_PROJECTS_ROOT}/xml"
        # BUILD_BYPRODUCTS "${EXTERNAL_PROJECTS_ROOT}/xml/build/googlemock/xml/libpugixml.a"
        )
endif()

#----------------------------------------------------------------------------------------------------------

get_filename_component(
    XML_INCLUDE_DIR
    "${EXTERNAL_PROJECTS_ROOT}/xml/src/googletest/include"
    ABSOLUTE)
set(XML_INCLUDE_DIR "${XML_INCLUDE_DIR}" PARENT_SCOPE)

# Create a libpugixml target to be used as a dependency by test programs
add_library(libpugixml IMPORTED STATIC GLOBAL)
add_dependencies(libpugixml ext_xml)

# Set libpugixml properties
set_target_properties(libpugixml PROPERTIES
    "IMPORTED_LOCATION" "${EXTERNAL_PROJECTS_ROOT}/xml/build/googlemock/xml/libpugixml.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
