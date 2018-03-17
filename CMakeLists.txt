cmake_minimum_required(VERSION 3.5)
project(Judger)

set(CMAKE_CXX_STANDARD 11)

add_executable(Judger   src/main.cpp
                        src/Option.cpp
                        src/Option.h
                        src/Clock.cpp
                        src/Clock.h
                        src/Compiler.cpp
                        src/Compiler.h
                        src/Executor.cpp
                        src/Executor.h
                        src/Comparer.cpp
                        src/Comparer.h
        src/WhiteList.cpp
        src/WhiteList.h
                        src/Result.cpp
                        src/Result.h
                        src/Logger.cpp
                        src/Logger.h src/utility.h src/utility.cpp)