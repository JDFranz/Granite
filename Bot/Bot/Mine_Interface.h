#include "Unit_Mapping.h"

class Mine_Interface
{
        public:
                void Mine_Interface::new_depot(BWAPI::Unit nexus);
                void Mine_Interface::update_mine_interface(Unit_Mapping * map);
        private:

                struct mineral_timer
                {
                        BWAPI::Unit mineral; // mineral timer is assigned to
                        int num;         // timer number
                        int gather_time; // time it takes a worker to gather
                        int gatherers; // number of gatherers on mineral
                        int mining_time; // used for efficiency calculation
                };

                // collection of timer structures
                std::vector<mineral_timer> timers;

                // maps for worker status and assigned mineral
                std::map<BWAPI::Unit, bool> worker_gathering;
                std::map<BWAPI::Unit, bool> worker_queued;
                std::map<BWAPI::Unit, BWAPI::Unit> worker_mineral;

                void update_mines();
                void update_workers(BWAPI::Unit, Unit_Mapping *);
                void add_timer(BWAPI::Unit, BWAPI::Unit);
                mineral_timer * get_top_patch(BWAPI::Unit unit);
                mineral_timer * get_timer(BWAPI::Unit);
                void Mine_Interface::percent_mining_time();
                bool Mine_Interface::get_worker(BWAPI::Unitset workers, BWAPI::Unit, BWAPI::Unit mineral);
                void Mine_Interface::remove_timers(Unit_Mapping * map);
                void Mine_Interface::update_idles(BWAPI::Unitset, Unit_Mapping * map);
                void Mine_Interface::worker_returning(BWAPI::Unit u);
                void Mine_Interface::assign_worker(BWAPI::Unit u, BWAPI::Unitset);
};

