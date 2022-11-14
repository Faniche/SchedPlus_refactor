#include <iostream>
#include "lib/spdlog/include/spdlog/spdlog.h"
#include "lib/spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "lib/CLI11/include/CLI/CLI.hpp"
#include "src/components/Node.h"
#include "src/components/DirectedLink.h"
#include "src/components/Stream.h"
#include "src/topology/Input.h"
#include "src/topology/Line_2sw_2es.h"
#include "src/solution/Solver.h"
#include "src/solution/GASolver.h"

namespace spd = spdlog;

int g_port_counter = 0;

int g_node_id = 0;

int g_link_id = 0;

int main(int argc, char **argv) {
    CLI::App app{"Schedplus: based on GA to schedule time sensitive flows."};
    int option_topology = 0;
    std::string topology_description = "Topology index:\n\t1: line_1sw_2es\n\t2: line_2sw_2es\n\t3: ring_4sw";
    app.add_option("-t, --topology", option_topology, topology_description);
    std::string option_ned_file;
    app.add_option("-n, --ned", option_ned_file, "Net description file name");
    int option_flow_number = 2;
    app.add_option("-f, --flows", option_flow_number, "The number of flow");
    int option_generation_number = 100;
    app.add_option("-g, --generation", option_generation_number, "The number of generation");
    bool flag_debug = {false};
    app.add_flag("-d, --debug", flag_debug, "debug mode");
    bool flag_random = {false};
    app.add_flag("-r, --random", flag_random, "use random flow for test");
    try {
        CLI11_PARSE(app, argc, argv);
    } catch (CLI::ParseError &error) {
        app.exit(error);
    }

    try {
        auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        auto console = std::make_shared<spdlog::logger>("console", sink);
        console->set_level(spdlog::level::debug);
        spd::set_default_logger(console);
        spd::set_pattern("[%H:%M:%S] [%^%l%$] %s:%# %v");
        auto input = std::make_shared<Line_2sw_2es>();
        input->setNodesAndLinks();
        input->setFlows(option_flow_number);
//        auto es0 = std::dynamic_pointer_cast<EndSystem>(input->nodes[0]);
//        auto sw0 = std::dynamic_pointer_cast<Switch>(input->nodes[2]);
        auto gaSolver = std::make_unique<GASolver>(input);
        gaSolver.get()->solve();




    } catch (const spdlog::spdlog_ex &ex) {

        std::cout << "Log init failed: " << ex.what() << std::endl;
        spdlog::shutdown();
        return 1;
    }
    spdlog::shutdown();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
