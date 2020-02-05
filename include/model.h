#pragma once
#include <ge211.h>
#include <core.h>
#include <world.h>
#include <agent.h>
#include <visibility.h>

namespace cell_world{
    struct Model
    {
        Model( World &world, std::vector<Agent*>&);
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void end_episode();
        State get_state(uint32_t);
        State get_state();
        void set_state(State);
        uint32_t iteration;
        protected:
        World &_world;
        std::vector<Agent*> &_agents;
        Visibility _visibility;
        Cell_group _cell_group;
        Map _map;
        void _epoch();
    };
} 