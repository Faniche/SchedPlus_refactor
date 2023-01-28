#include <iostream>
#include "lib/spdlog/include/spdlog/spdlog.h"
#include "lib/spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "lib/CLI11/include/CLI/CLI.hpp"
#include "src/components/node/Node.h"
#include "src/components/link/DirectedLink.h"
#include "src/components/stream/Stream.h"
#include "src/solution/Solver.h"
#include "src/topology/Line_2sw_2es.h"
#include "src/topology/Ring_4sw_8es.h"
#include "src/topology/Snow_7sw_18es.h"
#include "src/topology/Tree_7sw_21es.h"

#define OPENGA_EXTERN_LOCAL_VARS

#include "src/solution/GeneticAlgorithm/MoGaSolver.h"


namespace spd = spdlog;

int g_port_counter = 0;

int g_node_id = 0;

void run(int optionTopology,
         const std::string &streamFilePath,
         int optionStreamNumber,
         bool flagDebug,
         int generationNumber,
         int optionExecuteTimes) {
    std::shared_ptr<Input> input;
    std::string savePath = "SolutionReport/";
    switch (optionTopology) {
        case 1:
            input = std::make_shared<Line_2sw_2es>();
            savePath.append(LINE_2SW_2ES);
            break;
        case 2:
            input = std::make_shared<Ring_4sw_8es>();
            savePath.append(RING_4SW_8ES);
            break;
        case 3:
            input = std::make_shared<Tree_7sw_21es>();
            savePath.append(TREE_7SW_21ES);
            break;
        case 4:
            input = std::make_shared<Snow_7sw_18es>();
            savePath.append(SNOW_7SW_18ES);
            break;
        default:
            exit(EXIT_FAILURE);
    }
    input->setNodesAndLinks();
    if (!streamFilePath.empty() && optionStreamNumber != 0) {
        spdlog::get("console")->error("{}:{}: input error", __FILE__, __LINE__);
    } else if (!streamFilePath.empty()) {
        input->setStreams(streamFilePath);
    } else if (optionStreamNumber != 0) {
        input->setStreams(optionStreamNumber);
    } else {
        spdlog::get("console")->error("{}:{}: input error", __FILE__, __LINE__);
    }
    /* append stream number to result path. */
    savePath.append("/" + std::to_string(input->streams.size()) + "/");
    std::filesystem::remove_all(savePath);
    std::filesystem::create_directories(savePath);
    if(streamFilePath.empty())
        input->saveStreams(savePath);
    for (int i = 0; i < optionExecuteTimes; ++i) {
        auto gaSolver = std::make_unique<MoGaSolver>(input, flagDebug, generationNumber);
        gaSolver->solve(savePath, i);
    }
}

int main(int argc, char **argv) {
    CLI::App app{"Schedplus: based on GA to schedule time sensitive streams."};
    int optionTopology = 0;
    std::string topology_description = "Topology index:\n\t1: line_1sw_2es\n\t2: line_2sw_2es\n\t3: ring_4sw";
    app.add_option("-t, --topology", optionTopology, topology_description);
//    std::string option_ned_file;
//    app.add_option("-n, --ned", option_ned_file, "Net description file name");
    std::string streamFilePath = "";
    app.add_option("-s, --stream", streamFilePath, "Streams json file");
    int optionStreamNumber = 0;
    app.add_option("-f, --streams", optionStreamNumber, "The number of flow");
    int optionGenerationNumber = 100;
    app.add_option("-g, --generation", optionGenerationNumber, "The number of generation");
    int optionExecuteTimes = 10;
    app.add_option("-e, --execute", optionExecuteTimes, "The execute times");
    bool flagDebug = {false};
    app.add_flag("-d, --debug", flagDebug, "debug mode");
    bool flagRandom = {false};
    app.add_flag("-r, --random", flagRandom, "use random flow for test");
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
        run(optionTopology, streamFilePath, optionStreamNumber, flagDebug, optionGenerationNumber, optionExecuteTimes);
    } catch (const spdlog::spdlog_ex &ex) {

        std::cout << "Log init failed: " << ex.what() << std::endl;
        spdlog::shutdown();
        return 1;
    }
    spdlog::shutdown();
    return 0;
}
