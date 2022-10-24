cmake_minimum_required(VERSION 3.23)
project(ToyRedis)

set(CMAKE_CXX_STANDARD 11)

include_directories(laptop_factory_src)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(client
        laptop_factory_src/ClientMain.cpp
        laptop_factory_src/ClientSocket.cpp
        laptop_factory_src/ClientSocket.h
        laptop_factory_src/ClientStub.cpp
        laptop_factory_src/ClientStub.h
        laptop_factory_src/ClientThread.cpp
        laptop_factory_src/ClientThread.h
        laptop_factory_src/ClientTimer.cpp
        laptop_factory_src/ClientTimer.h
        laptop_factory_src/Messages.cpp
        laptop_factory_src/Messages.h
        laptop_factory_src/Socket.cpp
        laptop_factory_src/Socket.h)

add_executable(server
        laptop_factory_src/ServerMain.cpp
        laptop_factory_src/Messages.cpp
        laptop_factory_src/Messages.h
        laptop_factory_src/ServerSocket.cpp
        laptop_factory_src/ServerSocket.h
        laptop_factory_src/ServerStub.cpp
        laptop_factory_src/ServerStub.h
        laptop_factory_src/ServerThread.cpp
        laptop_factory_src/ServerThread.h
        laptop_factory_src/Socket.cpp
        laptop_factory_src/Socket.h)

target_link_libraries(client PRIVATE Threads::Threads)
target_link_libraries(server PRIVATE Threads::Threads)