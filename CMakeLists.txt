cmake_minimum_required(VERSION 2.8.4)
project(SmallRpc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-prototypes")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.56.0 COMPONENTS system serialization thread REQUIRED)
if (Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

set(COMMON_FILES common/RpcConnection.cpp common/SimpleNetworking.cpp)
set(CLIENT_FILES client/RpcConnection.cpp client/main.cpp client/Procedures.cpp)
set(SERVER_FILES server/RpcConnection.cpp server/main.cpp server/Procedures.cpp)

add_executable(SmallRpcClient ${CLIENT_FILES} ${COMMON_FILES})
add_executable(SmallRpcServer ${SERVER_FILES} ${COMMON_FILES})

target_link_libraries(SmallRpcClient ${Boost_LIBRARIES} dl)
target_link_libraries(SmallRpcServer ${Boost_LIBRARIES} dl)

