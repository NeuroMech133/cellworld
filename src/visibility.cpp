#include<visibility.h>
#include<iostream>
using namespace std;

namespace cellworld {

     enum Visible{
        Not_checked,
        Visible,
        Not_visible
    };
     
    vector<Cell> Visibility::line (const Cell &source, const Cell &destination) const{
        vector<Cell> l;
        double dist = _world.distance(source,destination);
        for (unsigned int i = 0; i < _world.size() ; i++){
            if (_world.distance(_world[i],source,destination) <= .5001 ){
                if ( (_world.distance(_world[i],source) < dist) &&
                     (_world.distance(_world[i],destination) < dist) ) {
                    l.push_back(_world[i]);
                 }
            }
        }
        for (unsigned int i = 1; i < l.size() ; i++){
            unsigned int ii = i;
            while ( ii > 0 && _world.distance(source,l[ii]) < _world.distance(source,l[ii])){
                std::swap(l[i],l[ii--]);
            }
        }
        return l;
    }
    bool Visibility::is_visible(const Cell& c0, const Cell& c1) const{
        return is_visible(c0.id,c1.id);
    }

    bool Visibility::is_visible(const uint32_t s, const uint32_t d) const{
        return _visibility[_visibility_index(s,d)] == Visible::Visible;
    }

    uint32_t Visibility::_visibility_index(const uint32_t s, const uint32_t d) const{
        return s * _world.size() + d;
    }

    void Visibility::_set_visibility(const uint32_t s, const uint32_t  d, const uint8_t is_visible){
        _visibility[_visibility_index(s,d)] = is_visible;
        _visibility[_visibility_index(d,s)] = is_visible;
    }

    bool Visibility::get_visible_cells (Cell_group & r, uint32_t cell_id) const{
        cout << "Visibility::get_visible_cells start" << endl;
        r.clear();
        cout << "Visibility::get_visible_cells 1" << endl;
        cout << "Visibility::get_visible_cells id:" << cell_id << endl;
        if (_world[cell_id].occluded) return false;
        cout << "Visibility::get_visible_cells 2" << endl;
        for (unsigned int i=0;i<_world.size();i++)
            if ((cell_id!=i) && (_visibility[_visibility_index(cell_id,i)] == Visible::Visible)) r.add(i);
        cout << "Visibility::get_visible_cells end" << endl;
        return true;
    }

    bool Visibility::_get_visibility(const uint32_t s, const uint32_t d){
        unsigned int i = _visibility_index(s,d);
        if (_visibility[i] != Visible::Not_checked) return _visibility[i] == Visible::Visible;
        if (_world[s].occluded || _world[d].occluded){
            _set_visibility(s,d,Visible::Not_visible);
            return false;
        }
        vector<Cell> l = line(_world[s],_world[d]);
        for (unsigned int ii = 0; ii < l.size() ; ii++){
            if (!_get_visibility(s,l[ii].id)){ 
                _set_visibility(s,d,Visible::Not_visible);
                return false;
            };
        }
        _set_visibility(s,d,Visible::Visible);
        return true;        
    }
     
    Visibility::Visibility( World &world) : _world(world){
        _visibility = (uint8_t*) malloc(world.size()*world.size());
        reset();
    }

    void Visibility::reset() {
        for(unsigned int i=0;i<_world.size()*_world.size();i++) _visibility[i]=Visible::Not_checked;
        for (unsigned int s = 0; s < _world.size() ; s++){
            for (unsigned int d = 0; d < _world.size() ; d++){
                if (s!=d) _get_visibility(s,d);
            }
        }
    }

    Visibility::~Visibility() {
        free(_visibility);
    }
}