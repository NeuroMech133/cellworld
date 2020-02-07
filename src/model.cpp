#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;

void Model::_epoch(){
    L("Model::_epoch() start");
    iteration++;
    for (Agent * _agent : _agents) {
        auto action = _agent->get_action(); // read the action from the agent
        _agent->data.status = Action_pending; // set the state to pending again
        auto move = action.destinations[action.probabilities.pick()];
        int32_t destination_index = _map.find( _agent->data.cell.coordinates + move );
        if ( destination_index!= Not_found && !_world[destination_index].occluded )
            _agent->data.cell=_cell_group[ destination_index ];
    }
    for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++)
        _agents[agent_index]->update_state( get_state(agent_index) );
    L("Model::_epoch() end");
}

bool Model::update() // if all agents made their moves, it triggers an new epoch
{
    L("Model::update() start");
    bool finish;
    bool epoch_ready; // assumes no new actions
    do {
        finish = false;
        epoch_ready = true;
        for (auto &_agent : _agents) { // ask all agents to make their moves
            finish = finish || _agent->data.status == Finished; // check if all agents are done
            epoch_ready = epoch_ready && _agent->data.status == Action_ready;
        }
        if (finish) return false; //if any agent is not longer running, terminates the simulation
    } while (!epoch_ready);
    _epoch(); //if all agents are done with their actions, trigger the epoch
    L("Model::update() end");
    return true;
}

vector<Agent_data> Model::get_agents_data(){
    L("Model::get_agents_data() start");
    vector<Agent_data> r;
    for (auto & _agent : _agents) r.push_back(_agent->data);
    L("Model::get_agents_data() end");
    return r;
}

Model::Model( World &world, std::vector<Agent*> &agents ) : 
    _world (world),
    _agents (agents),
    _visibility(),
    _cell_group(world.create_cell_group()),
    _map(_cell_group)
    {
        L("Model::Model( World &, std::vector<Agent*> &) start");
        L("Model::Model( World &, std::vector<Agent*> &) end");
    }

void Model::end_episode() {
    L("Model::end_episode() start");
    State state = get_state();
    for(auto & _agent : _agents) _agent->end_episode(state);
    L("Model::end_episode() end");
}

void Model::start_episode() {
    L("Model::start_episode() start");
    iteration = 0;
    L("Model::start_episode() - State state  = get_state();");
    State state  = get_state();
    L("Model::start_episode() - for(auto & _agent : _agents)");
    for(auto & _agent : _agents) {
        L("Model::start_episode() - _agent->data.status = Started;");
        _agent->data.status = Started;
        L("Model::start_episode() - _agent->data.cell = _agent->start_episode(state);");
        _agent->data.cell = _agent->start_episode(state);
    }
    L("Model::start_episode() end");
}

State Model::get_state() {
    L("Model::get_state() start");
    State state;
    state.iteration = iteration;
    for(auto & _agent : _agents) state.agents_data.push_back(_agent->data);
    L("Model::get_state() end");
    return state;
}

void Model::set_state(State state) {
    L("Model::set_state(State) start");
    iteration = state.iteration;
    for(auto & _agent : _agents) {
        int32_t index = state.find(_agent->data.type.name);
        if (index!=Not_found)
            _agent->data = state.agents_data[index];
        else
            _agent->start_episode(state);
    }
    L("Model::set_state(State) end");
}

State Model::get_state(uint32_t agent_index) {
    L("Model::get_state(uint32_t) start");
    auto cell = _agents[agent_index]->data.cell;
    State state;
    state.iteration = iteration;
    for (uint32_t index = 0; index < _agents.size() ; index++)
        if (index != agent_index && _visibility[cell].contains(_agents[index]->data.cell)) state.agents_data.push_back(_agents[index]->data);
    L("Model::get_state(uint32_t) end");
    return state;
}
