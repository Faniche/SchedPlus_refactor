cmake_minimum_required(VERSION 3.23)
project(SchedPlus_refactor)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/CLI11
        lib/spdlog)

add_executable(SchedPlus main.cpp
        src/type.h
        src/components/stream/Stream.cpp src/components/stream/Stream.h
        src/components/stream/DeliveryGuarantee.cpp src/components/stream/DeliveryGuarantee.h
        src/components/node/Node.cpp src/components/node/Node.h
        src/components/node/Switch.cpp src/components/node/Switch.h
        src/components/node/EndSystem.cpp src/components/node/EndSystem.h
        src/components/node/Port.cpp src/components/node/Port.h
        src/components/stream/Frame.cpp src/components/stream/Frame.h
        src/components/link/Route.cpp src/components/link/Route.h
        src/components/link/DirectedLink.cpp src/components/link/DirectedLink.h
        src/components/node/GateControlEntry.cpp src/components/node/GateControlEntry.h
        src/components/link/FullDuplexLink.cpp src/components/link/FullDuplexLink.h
        src/topology/Input.cpp src/topology/Input.h
        src/topology/Line_2sw_2es.cpp src/topology/Line_2sw_2es.h
        src/solution/SmtSolver.cpp src/solution/SmtSolver.h
        src/solution/Solver.cpp src/solution/Solver.h
        src/solution/GASolver.cpp src/solution/GASolver.h
        src/components/link/Graph.cpp src/components/link/Graph.h)

target_link_libraries(SchedPlus PUBLIC
        "${PROJECT_SOURCE_DIR}/lib/spdlog/build/libspdlog.a"
        pthread
        CLI11::CLI11
        z3)

target_include_directories(SchedPlus PUBLIC lib/spdlog/include)

add_executable(getmt test/getMt.cpp)

add_executable(DpWithNote test/DpWithNote.cpp)