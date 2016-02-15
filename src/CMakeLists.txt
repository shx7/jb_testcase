cmake_minimum_required(VERSION 3.1)

project(duplicate_search)
set(MAIN_SRC main.cpp)
set(MAIN_EXE duplicate_search)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FILE_CLASSIFIER_SRC FileClassifier.cpp)
set(UTIL_SRC Util.cpp)

find_package(Boost 1.55 COMPONENTS
        filesystem program_options system
        REQUIRED)
if (!Boost_FOUND)
    message(FATAL_ERROR "Boost not found") 
endif()
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libraries path: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost libraries path: ${Boost_LIBRARIES}")

find_package(OpenSSL)
if (OPENSSL_FOUND)
    message(STATUS "OpenSSL found")
else()
    message(FATAL_ERROR "OpenSSL not found") 
endif()

message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries path: ${OPENSSL_LIBRARIES}")

add_executable(${MAIN_EXE} ${MAIN_SRC} ${FILE_CLASSIFIER_SRC} ${UTIL_SRC})
set_target_properties(${MAIN_EXE} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    ) 

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
LINK_DIRECTORIES(${OPENSSL_LIBRARY_DIRS})
target_link_libraries(${MAIN_EXE} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
