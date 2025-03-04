#include <cell_world/connection.h>
#include <cell_world/chance.h>
#include <cell_world/map.h>

using namespace std;

namespace cell_world {

    Coordinates_list Connection_pattern::get_candidates(Coordinates coordinate) const {
        Move_list c;
        for (auto &p:(*this)) c.push_back(coordinate + p);
        return c;
    }

    Connection_pattern::Connection_pattern() = default;

    Connection_pattern &Connection_pattern::operator=(const Connection_pattern &cp) {
        if (&cp != this) {
            clear();
            for (auto p:cp) push_back(p);
        }
        return *this;
    }

    Graph Connection_pattern::get_graph(const Cell_group &cells) const {
        Graph graph(cells);
        Map map(cells);
        int destination_index;
        for (const Cell &cell:cells) {
            if (cell.occluded) continue;
            for (Coordinates &coord : get_candidates(cell.coordinates)) {
                if ((destination_index = map.find(coord)) != Not_found &&
                        !cells[destination_index].occluded) {
                    graph[cell].add(cells[destination_index]);
                }
            }
        }
        return graph;
    }

    Connection_pattern Connection_pattern::get_pattern(Cell c, Cell_group cg) {
        Connection_pattern cp;
        for (unsigned int i = 0; i < cg.size(); i++) cp.push_back(cg[i].coordinates - c.coordinates);
        return cp;
    }

    Move Connection_pattern::random_move() const {
        return (*this)[Chance::dice(size())];
    }
}