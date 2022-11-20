cmake_minimum_required(VERSION 3.23)
project(SchedPlus_refactor)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/CLI11
        lib/spdlog)

add_executable(SchedPlus main.cpp
        src/type.h
        src/components/Stream.cpp               src/components/Stream.h
        src/components/DeliveryGuarantee.cpp    src/components/DeliveryGuarantee.h
        src/components/Node.cpp                 src/components/Node.h
        src/components/Switch.cpp               src/components/Switch.h
        src/components/EndSystem.cpp            src/components/EndSystem.h
        src/components/Port.cpp                 src/components/Port.h
        src/components/Frame.cpp                src/components/Frame.h
        src/components/Route.cpp                src/components/Route.h
        src/components/DirectedLink.cpp         src/components/DirectedLink.h
        src/components/GateControlEntry.cpp     src/components/GateControlEntry.h
        src/components/FullDuplexLink.cpp       src/components/FullDuplexLink.h
        src/topology/Input.cpp                  src/topology/Input.h
        src/topology/Line_2sw_2es.cpp           src/topology/Line_2sw_2es.h
        src/solution/SmtSolver.cpp              src/solution/SmtSolver.h
        src/solution/Solver.cpp                 src/solution/Solver.h
        src/solution/GASolver.cpp               src/solution/GASolver.h)

target_link_libraries(SchedPlus PUBLIC
        "${PROJECT_SOURCE_DIR}/lib/spdlog/build/libspdlog.a"
        pthread
        CLI11::CLI11
        z3)

target_include_directories(SchedPlus PUBLIC lib/spdlog/include)

add_executable(getmt getMt.cpp)