cmake_minimum_required(VERSION 3.20)

# set the project name
project(BoostSpiritExamples VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.75.0)
if(Boost_FOUND)
    message(STATUS "Boost was found!")
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(spirit_lex_word_count1 spirit_lex_word_count1.cpp)
    #target_link_libraries(spirit_lex_word_count1 ${Boost_LIBRARIES}) 
endif()
