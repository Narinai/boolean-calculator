cmake_minimum_required(VERSION 3.23)
project(MyFantasticProject VERSION 0.1)


set(CMAKE_CXX_STANDARD 20)

option(ItIsALib         "Is it a library?"              ON)
option(IsHeadersOnly    "Is it a header-only lib?"      OFF)
option(IsShared         "If it is a lib, is it SHARED?" ON)
option(UseClangTidy     "Use ClangTidy"                 ON)
option(UseCPPCheck      "Use CPPCheck"                  ON)
option(UseCCache        "Use CCache"                    ON)
option(UseIPO           "Use IPO"                       ON)



# Files
file(GLOB_RECURSE sources src/*.cc)
file(GLOB_RECURSE headers include/*.h)

include(cmake/signs.cmake)
include(cmake/tools.cmake)

message("\n${Block} Files found:")
message("${BigArrow} Headers:")
foreach(header IN LISTS headers)
    message("${SmallArrow} ${header}")
endforeach()
message("${BigArrow} Sources:")
foreach(source IN LISTS sources)
    message("${SmallArrow} ${source}")
endforeach()

message("\n${Block} Checking, is source dir equal to binary dir")
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "${ErrorSmallArrow} You are building project in source directory! EXIT!")
else()
    message("${SmallArrow} It's ok")
endif()

message("\n${Block} Compiling as:")
if (ItIsALib)
    if (IsHeadersOnly)
        message("${SmallArrow} Headers only library")
        add_library(${PROJECT_NAME} INTERFACE)
        target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
        target_include_directories(
                ${PROJECT_NAME}
                INTERFACE
                $<INSTALL_INTERFACE:include>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )
    elseif(IsShared)
        message("${SmallArrow} Shared library")
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
        set(CMAKE_CXX_VISIBILITY_PRESET hidden)
        set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
        add_library(
                ${PROJECT_NAME}
                SHARED
                ${headers}
                ${sources}
        )
        target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
        target_include_directories(
                ${PROJECT_NAME}
                PUBLIC
                $<INSTALL_INTERFACE:include>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
    else()
        message("${SmallArrow} Static library")
        add_library(
                ${PROJECT_NAME}
                STATIC
                ${headers}
                ${sources}
        )
        target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
        target_include_directories(
                ${PROJECT_NAME}
                PUBLIC
                $<INSTALL_INTERFACE:include>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
    endif()
else()
    message("${SmallArrow} Executable")
    add_executable(
            ${PROJECT_NAME}
            ${headers}
            ${sources})
endif()

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
        PUBLIC_HEADER            "${PUBLIC_HEADERS}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
)

#add_subdirectory("libs/otherlib" build)
#target_link_libraries(${PROJECT_NAME} otherlib)
