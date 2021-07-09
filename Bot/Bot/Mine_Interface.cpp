#include "Mine_Interface.h"

/*
 * Mine Interface
 *
 * Public Procedures
 *
 * ----------------------------------------------------------------------
 * Update Mine Interface
 *      Called by on frame, this ensures IDLE workers in range of resource
 *      Depots are mining.
 *
 * New Depot
 *      Called by onComplete, this adds all minerals in range of a new
 *      resource depot to the timers which enables mining
 * ----------------------------------------------------------------------
 *
 * Idea -
 *      Queue based mining procedure with worker to distance taken into
 *      account
 *
 * - Data Structures -
 *
 * Mineral Timers
 *
 *       These structures are assigned to each mineral and act as a timer
 *         also contains the number of workers currently gathering
 *         s well as typical gather time and a variable mining time which
 *         is used for efficiency calcutation
 *
 *
 * Worker Maps
 *      
 *       Maps are used to get information about what the worker is doing
 *         and which mineral it is assigned to
 *
 * Form -
 *
 *      Main loop in update workers. Update timer counters, check for minerals
 *      that have been mined out then go through each resource depot and
 *      update the workers in range.
 *
 * TODO 
 *
 * Fix errors that will occur with more than one resource depot
 *
 * error in get_top_patch
 *
 *      The list of timers is not delimited by each resource depot
 *      therefore the function get_top_patch will search through
 *      timers for all resource depots.
 *
 *              TOFIX - modify loop or change data structure
 *
 * error in new_depot
 *
 *      The minerals are added in radius "as the crow flies"
 *      there may be a situation where minerals are in range but
 *      at a different height or behind a wall.
 *
 *              TOFIX - Add tool ground_distance that uses pathfinding to
 *                      approximate walking distance.
 *                      Or fine tune the radius so it will only grab
 *                      correct minerals.
 *
 *  ------------------ 
 *  - Author Kevin Kobler
 *  ------------------ 
 *
 */

// Update Mine Interface../
void Mine_Interface::update_mine_interface(Unit_Mapping * map)
        /* Called by onFrame()
         *
         * idea - update mining for all resource depots
         */
{

        BWAPI::Unitset resource_depots;

        // Remove timer for destroyed minerals
        remove_timers(map);

        // Increment the timer for minerals being mined
        update_mines();

        // Uncomment to check mining efficiency
        //percent_mining_time(map);

        // Get all our resource depots
        resource_depots = BWAPI::Broodwar->getUnitsInRectangle(0, 0, 99999, 99999,
                        BWAPI::Filter::IsResourceDepot && BWAPI::Filter::IsOwned);

        for ( auto &u : resource_depots )
        {
                if(!u)
                        continue;
                // Main function for the mines
                update_workers(u, map);
        }
}//..

// New Depot../
void Mine_Interface::new_depot(BWAPI::Unit nexus)
        /*
         * Called on the completion of a nexus
         * 
         * input - nexus -> the nexus to set up mining
         *
         * idea -> add timer for all minerals
         */ 
{
        BWAPI::Unitset minerals = nexus->getUnitsInRadius(300, BWAPI::Filter::IsMineralField);

        for (auto &u : minerals)
        {
                if (!u)
                        continue;
                add_timer(u, nexus);
        }
}//..

// Add Timer../
void Mine_Interface::add_timer(BWAPI::Unit mineral, BWAPI::Unit nexus)
        /*
         * input - mineral -> the mineral to create a timer for
         *         nexus   -> used to calculate travel distance to mineral
         *
         * idea - initialize structure variables for mineral timer
         *        add timer to the list (timers)
         */
{

        // Declare structure
        mineral_timer new_mineral;

        // Set structure values
        new_mineral.mineral = mineral; // Mineral associated with timer
        new_mineral.num = 0; // The timer value
        new_mineral.gather_time = -77; // Time it takes worker to gather
        new_mineral.gatherers = 0; // # of workers gathering
        new_mineral.mining_time = 0; // used for percentage calculation

        // Add to vector
        timers.push_back(new_mineral);
}//..

// Remove Timers../
void Mine_Interface::remove_timers(Unit_Mapping * map)
        /*
         * input - unit map
         *
         * idea - erases timer from vector if the mineral it was associated
         *        with is gone. Set its current worker as IDLE
         */
{
        std::vector<mineral_timer>::iterator it;
        for ( it = timers.begin(); it != timers.end(); it++)
        {
                if (!it->mineral)
                {
                        timers.erase(it);
                }
        }
}//..

// Update Mines../
void Mine_Interface::update_mines()
        /*
         * idea - use the mining timer to update mining processes
         */
{

        for ( auto &timer : timers)
        {
                if (!timer.mineral)
                        continue;
                if(timer.mineral->isBeingGathered())
                        timer.num+=2;
        }
}//..

// Percent Mining Time../
void Mine_Interface::percent_mining_time()
        /*
         * input - unit map
         *
         * idea - print out the percent of time a mineral is being gathered
         */
{
        static int frames = 0;
        int count = 0;
        double percent;
        double totalpercent = 0;

        frames++; //increment frame counter
        for (auto& u : timers)
        {
                if (!u.mineral)
                        continue;

                // Count number of timers for totalpercent
                count++;

                // Tally mineral gathering
                if (u.mineral->isBeingGathered())
                        u.mining_time++;

                // Calculate percent and add it to total percent
                percent = double(u.mining_time) / frames * 100;
                totalpercent += percent;

                // Print and reset variables every 200 frames
                if (!(frames % 200))
                {
                        std::cout << "%" << percent << " mining time" << std::endl;
                        u.mining_time = 0;
                }

        }
        // Print and reset variables every 200 frames
        if (!(frames % 200))
        {
                std::cout << std::endl << "Total Percentage = " << totalpercent / count << std::endl << std::endl;
                totalpercent = 0;
                count = 0;
                frames = 0;
        }
}//..

// Update Workers../
void Mine_Interface::update_workers(BWAPI::Unit nexus, Unit_Mapping * map)
        /*
         * input - nexus ->Then Resource depot to order workers from
         *         map   -> Unit map, for getting and updating worker status
         *
         * idea - get all workers in range of resource depot whose
         *        status is set to IDLE update their status to MINE
         *        for all workers in range whose status is MINE
         *        check their current order and update accordingly.
         */
{
        BWAPI::Unitset workers;
        mineral_timer * patch;
        static int spamtimer = 0;


        workers = nexus->getUnitsInRadius(300);
        //set IDLE workers to MINE
        update_idles(workers, map);

        // Spamtimer spaces out the right click on mineral spam
        ++spamtimer;
        for (auto &u : workers)
        {
                if (!u)
                        continue;

                // skip if not assigned to the mines
                if (map->get_task(u) != MINE)
                        continue;

                // assign a new patch to worker if not gathering or returning
                if ((worker_gathering[u] == false && !u->isCarryingMinerals()))
                        assign_worker(u, workers);

                // if the worker is idle and carrying minerals return cargo
                if (u->isIdle() && u->isCarryingMinerals())
                        u->returnCargo();

                // Deassign worker if the mineral is gone
                if (!worker_mineral[u])
                {
                        worker_queued[u] = false;
                        worker_gathering[u] = false;
                        worker_mineral[u] = nullptr;
                        continue;
                }

                // Make sure worker isn't idle
                if (u->isIdle() && !worker_queued[u])
                        if (worker_mineral[u])
                                u->gather(worker_mineral[u]);

                // Have queued workers spam click minerals to hold position
                if ( worker_queued[u] == true && !( spamtimer % 3 ))
                {
                        if (worker_mineral[u])
                        {
                                u->rightClick(worker_mineral[u]);
                                spamtimer = 0;
                        }
                }


                // if the mineral is no longer being mined
                // have queued worker harvest
                if (worker_queued[u] == true && !worker_mineral[u]->isBeingGathered())
                {
                        if (worker_mineral[u])
                        {
                                u->gather(worker_mineral[u]);
                                worker_queued[u] = false;
                        }
                }

                // If worker finsined harvesting minerals
                if (u->isCarryingMinerals() && worker_gathering[u] == true)
                {
                        worker_returning(u);
                }
        }
}//..

// Worker Returning../
void Mine_Interface::worker_returning(BWAPI::Unit u)
        /*
         * input - u -> The worker who is returning with cargo
         *
         * idea - update timer and worker maps
         */
{
        mineral_timer* patch;

        if (!worker_mineral[u])
                return;

        // Get the timer for the worker's mineral
        patch = get_timer(worker_mineral[u]);

        if (!patch)
                return;

        // Decrease gatherers and update the timer
        patch->gatherers--;
        patch->num = patch->gather_time * patch->gatherers;

        // set variables
        worker_gathering[u] = false;
        worker_mineral[u] = nullptr;

}//..

// Assign Worker../
void Mine_Interface::assign_worker(BWAPI::Unit u, BWAPI::Unitset workers)
        /*
         * input - u -> worker to be assigned
         *
         * idea - get the top patch in queue and assign worker to it
         */
{
        mineral_timer * patch;

        // get the next patch in queue
        patch = get_top_patch(u);
        patch->num += patch->gather_time;

        if (!patch->mineral)
                return;

        // check if there is a worker already on the mineral
        if (get_worker(workers, u, patch->mineral))
        { // if so delegate this worker as queued
                worker_queued[u] = true;
        }

        // click the mineral and set values
        u->rightClick(patch->mineral);

        patch->gatherers++;

        worker_mineral[u] = patch->mineral;
        worker_gathering[u] = true; // set worker as mining
}//..

// Update Idles../
void Mine_Interface::update_idles(BWAPI::Unitset workers, Unit_Mapping * map)
        /*
         * input - nexus -> resource depot next to minerals
         *         map -> unit map
         *
         * idea - set the task of all idle areas within radius of the nexis
         *        to mine
         */
{
        for (auto &u : workers)
        {
                if (!u)
                        continue;

                if (map->get_task(u) == IDLE)
                {
                        map->set_task(u, MINE);
                        worker_queued[u] = false;
                        worker_mineral[u] = nullptr;
                        worker_gathering[u] = false;
                }
        }
}//..

// Get Worker../
bool Mine_Interface::get_worker(BWAPI::Unitset workers, BWAPI::Unit unit, BWAPI::Unit mineral)
        /*
         * input - workers -> workset to search
         *         unit -> ignore this unit
         *         mineral -> return true if a worker is mining this
         *
         * idea - unit calls this function to see if there is already a worker
         *        mining the selected mineral
         */
{
        BWAPI::Unit worker = nullptr;

        if (!mineral)
                return false;

        for (auto& u : workers)
        {
                // continue if it doesn't exist
                if (!u)
                        continue;
                // continue if u is unit
                if (u->getID() == unit->getID())
                        continue;
                //check to see if the worker is assigned to mineral
                if (worker_mineral[u])
                        if (worker_mineral[u]->getID() == mineral->getID())
                                return true;
        }
        return false;
}//..

// Get Timer../
Mine_Interface::mineral_timer * Mine_Interface::get_timer(BWAPI::Unit unit)
        /*
         * input - unit -> the mineral that we are getting the timer for
         *
         * idea - go through all timers and check if they match unit
         */
{
        mineral_timer * match = new mineral_timer;
        if (!unit)
                return nullptr;
        match = nullptr;
        for (auto& u : timers)
        {
                if (!u.mineral)
                        continue;

                if (u.mineral->getID() == unit->getID())
                {
                        match = &u;
                        return match;
                }
        }
        return match;

}//..

// Get Top Patch../
Mine_Interface::mineral_timer * Mine_Interface::get_top_patch(BWAPI::Unit unit)
        /*
         * input - unit -> Worker the mineral is for
         *
         * idea - find the max of all timers with the distance of unit to
         *        mineral taken into account
         */
{
        int highest = -99999;
        mineral_timer * top = new mineral_timer;
        int base_num;

        top = nullptr;
        for (auto &u : timers)
        {
                if (!u.mineral)
                        continue;
                base_num = int(unit->getDistance(u.mineral) / 5 * -.5);
                if (u.num > highest)
                {
                        highest = u.num + base_num;
                        top = &u;
                }
        }

        return top;
}//..
