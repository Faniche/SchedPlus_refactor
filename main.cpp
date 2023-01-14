#include <iostream>
#include "lib/spdlog/include/spdlog/spdlog.h"
#include "lib/spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "lib/CLI11/include/CLI/CLI.hpp"
#include "src/components/node/Node.h"
#include "src/components/link/DirectedLink.h"
#include "src/components/stream/Stream.h"
#include "src/topology/Line_2sw_2es.h"
#include "src/solution/Solver.h"

#define OPENGA_EXTERN_LOCAL_VARS
#include "src/solution/GeneticAlgorithm/MoGaSolver.h"

namespace spd = spdlog;

int g_port_counter = 0;

int g_node_id = 0;

void run(int optionTopology, const std::string &streamFilePath, bool flagDebug, int generationNumber, int runId){
    if (runId != 0)  g_node_id = 0;
    std::shared_ptr<Input> input;
    std::string savePath = "SolutionReport/";
    switch (optionTopology) {
        case 1:
            input = std::make_shared<Line_2sw_2es>();
            savePath.append(LINE_2SW_2ES);
            break;
        case 2:
            // TODO
//                input = std::make_shared<Line_2sw_2es>();
//                savePath.append(LINE_2SW_2ES);
            break;
        case 3:
            // TODO
//                input = std::make_shared<Line_2sw_2es>();
//                savePath.append(LINE_2SW_2ES);
            break;
        default:
            exit(EXIT_FAILURE);
    }
    input->setNodesAndLinks();
    input->setStreams(streamFilePath);
    /* append stream number to result path. */
    savePath.append("/" + std::to_string(input->streams.size()) + "/");
    if (runId == 0) {
        bool ret = std::filesystem::remove_all(savePath);
        std::filesystem::create_directories(savePath);
    }
    auto gaSolver = std::make_unique<MoGaSolver>(input, flagDebug, generationNumber);
    gaSolver->solve(savePath, runId);
}

int main(int argc, char **argv) {
    CLI::App app{"Schedplus: based on GA to schedule time sensitive streams."};
    int option_topology = 0;
    std::string topology_description = "Topology index:\n\t1: line_1sw_2es\n\t2: line_2sw_2es\n\t3: ring_4sw";
    app.add_option("-t, --topology", option_topology, topology_description);
//    std::string option_ned_file;
//    app.add_option("-n, --ned", option_ned_file, "Net description file name");
    std::string streamFilePath;
    app.add_option("-s, --stream", streamFilePath, "Streams json file");
    int option_flow_number = 2;
    app.add_option("-f, --streams", option_flow_number, "The number of flow");
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

        for (int i = 0; i < 10; ++i) {
            run(option_topology, streamFilePath, flag_debug, option_generation_number, i);
        }


    } catch (const spdlog::spdlog_ex &ex) {

        std::cout << "Log init failed: " << ex.what() << std::endl;
        spdlog::shutdown();
        return 1;
    }
    spdlog::shutdown();
    return 0;
}
