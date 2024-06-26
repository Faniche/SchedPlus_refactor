//
// Created by faniche on 22-11-14.
//

#ifndef SCHEDPLUS_REFACTOR_MOGASOLVER_H
#define SCHEDPLUS_REFACTOR_MOGASOLVER_H

#include <set>
#include <queue>
#include "../Solver.h"
#include "../../../lib/openGA/openGA.hpp"
#include "GaSolution.h"
#include "GaMiddleCost.h"

typedef EA::Genetic<TtStreams, MyMiddleCost> GA_Type;
typedef EA::GenerationType<TtStreams, MyMiddleCost> Generation_Type;

class MoGaSolver : public Solver {
private:
    GA_Type ga_obj;

    EA::Chronometer timer;

    double accTime;

    std::shared_ptr<Input> input;

    int optionInitExecuteTimeOut;

    int optionEvalExecuteTimeOut;

    bool debug;

    int generations;

    bool flagUseNoWait;

    void vSolve(const std::string &path, int32_t runId) override;

    void setEachHopStartTime(const TtStreams &p, MyMiddleCost &c);

    bool checkDDLorE2E(MyMiddleCost &c,
                       bool useNoWait,
                       std::pair<stream_id_t, sched_time_t> &maxDdl,
                       std::pair<stream_id_t, sched_time_t> &maxE2E);

    void groupStreamsInit(const std::vector<route_t> &routes,
                          map<group_id_t, vector<stream_id_t>> &groupStream);

    void groupStreamsEval(const std::vector<route_t> &routes,
                          MyMiddleCost &c);

    void setLinkHyperperiod(const TtStreams &p, MyMiddleCost &c);

    void setLinkStreamInterval(const TtStreams &p, MyMiddleCost &c);

    bool checkCollisionWithInterval(const TtStreams &p, MyMiddleCost &c);

    static bool checkCollisionWithStreamHelp(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                                             sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen);

    bool checkCollisionWithStream(const TtStreams &p, MyMiddleCost &c);

    bool scheduleP5(const TtStreams &p, MyMiddleCost &c);

    group_id_t getGrpIdOfStream(stream_id_t streamId, MyMiddleCost &c);

public:
    explicit MoGaSolver(std::shared_ptr<Input> _input, int _optionInitExecuteTimeOut, int _optionEvalExecuteTimeOut, bool _debug, int _generations, bool _flagUseNoWait);

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

    void save_results(GA_Type &ga_obj, const std::string &path, int32_t runId, double costTime);

};


#endif //SCHEDPLUS_REFACTOR_MOGASOLVER_H
