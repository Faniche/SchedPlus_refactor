//
// Created by faniche on 22-11-14.
//

#ifndef SCHEDPLUS_REFACTOR_MOGASOLVER_H
#define SCHEDPLUS_REFACTOR_MOGASOLVER_H

#include "../Solver.h"
#include "../../../lib/openGA/openGA.hpp"
#include "GaSolution.h"
#include "GaMiddleCost.h"

typedef EA::Genetic<TtStreams, MyMiddleCost> GA_Type;
typedef EA::GenerationType<TtStreams, MyMiddleCost> Generation_Type;

class MoGaSolver : public Solver {
private:
    std::shared_ptr<Input> input;

    bool debug;

    int generations;

    void vSolve() override;

    void setEachHopStartTime(const TtStreams &p, MyMiddleCost &c);

    bool checkDDLorE2E(MyMiddleCost &c, std::pair<stream_id_t, sched_time_t> &max_ddl,
                       std::pair<stream_id_t, sched_time_t> &max_e2e);

    void setLinkHyperperiod(const std::vector<route_t> &routes,
                            map<link_id_t, sched_time_t> &linkHyperperiod);

    void groupStreams(const std::vector<route_t> &routes,
                      MyMiddleCost &c);

    static bool checkCollisionHelp(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                                   sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen);

    bool checkCollision(MyMiddleCost &c, const std::vector<route_t> &routes);

    static std::pair<sched_time_t, sched_time_t>
    compressP6Help(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                   sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen,
                   bool &couldMoveRight,
                   bool &couldMoveLeft
    );

    void compressP6(const TtStreams &p, MyMiddleCost &c);

    bool checkQueueCache(const TtStreams &p, MyMiddleCost &c);

public:
    explicit MoGaSolver(std::shared_ptr<Input> _input, bool _debug, int _generations);

    void init_genes(TtStreams &p, const std::function<double(void)> &rnd01);

    bool eval_solution(const TtStreams &p, MyMiddleCost &c);

    double get_shrink_scale(int n_generation, const std::function<double(void)> &rnd01);

    TtStreams mutate(
            const TtStreams &X_base,
            const std::function<double(void)> &rnd01,
            double shrink_scale);

    TtStreams crossover(
            const TtStreams &X1,
            const TtStreams &X2,
            const std::function<double(void)> &rnd01);

    vector<double> calculate_MO_objectives(const GA_Type::thisChromosomeType &X);

    void MO_report_generation(
            int generation_number,
            const EA::GenerationType<TtStreams, MyMiddleCost> &last_generation,
            const vector<unsigned int> &pareto_fron);

    void save_results(GA_Type &ga_obj, const std::string &ned_file);


};


#endif //SCHEDPLUS_REFACTOR_MOGASOLVER_H
