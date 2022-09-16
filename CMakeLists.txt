cmake_minimum_required(VERSION 3.16)

project(bipartite_matching VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(
    ${PROJECT_NAME}
    hyperparameter.h
    main.cpp
  )
  target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()
