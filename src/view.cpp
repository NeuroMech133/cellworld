#include<view.h>
#include<iostream>
#include <fstream>

using namespace ge211;
using namespace std;
using namespace cell_world;

View::View(World &world, Dimensions scene_dimensions):
    _world(world) ,
    _scene_dimensions(scene_dimensions)
{
    L("View::View(World &, Dimensions ) start");
    double sx,sy,lx,ly;
    lx = sx = _world[0].location.x;
    ly = sy = _world[0].location.y;
    for (unsigned int i=0;i<_world.size();i++){
        double x = _world[i].location.x;
        double y = _world[i].location.y;
        if (lx < x) lx = x;
        if (ly < y) ly = y;
        if (sx > x) sx = x;
        if (sy > y) sy = y;
    }
    double rx = (_scene_dimensions.width - 100) / (lx - sx);
    double ry = (_scene_dimensions.height - 100) / (ly - sy);
    _ratio = rx<ry?rx:ry;
    _cell_size = (int)_ratio/2-1;
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,0}); //0
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,255,255}); //1
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,0,0}); //2
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,255,0}); //3
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,255}); //4
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,255,0}); //5
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,255,255}); //6
    _cell_sprites.emplace_back(_cell_size, ge211::Color{255,0,255}); //7
    _cell_sprites.emplace_back(_cell_size, ge211::Color{192,192,192}); //8
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,128,128}); //9
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,0,0}); //10
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,128,0}); //11
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,128,0}); //12
    _cell_sprites.emplace_back(_cell_size, ge211::Color{128,0,128}); //13
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,128,128}); //14
    _cell_sprites.emplace_back(_cell_size, ge211::Color{0,0,128}); //15
    for (uint16_t i=0; i<256 ; i++ ) {
        _value_sprites.emplace_back(_cell_size, ge211::Color{255, (uint8_t) (255 - i), (uint8_t) (255 - i)});
    }
    int icon_counter = 0;
    string file_name;
    {
        std::stringstream fmt;
        fmt << "icon-" << icon_counter << ".png";
        file_name = resource_file(fmt.str());
    }
    while (file_exists (file_name)) {
        _icon_sprites.emplace_back(file_name);
        {
            std::stringstream fmt;
            icon_counter++;
            fmt << "icon-" << icon_counter << ".png";
            file_name = resource_file(fmt.str());
        }
    }
    icon_counter = 0;
    {
        std::stringstream fmt;
        fmt << "icons/custom-icon-" << icon_counter << ".png";
        file_name = fmt.str();
    }
    while (file_exists (file_name)) {
        _custom_icon_sprites.emplace_back(file_name);
        {
            std::stringstream fmt;
            icon_counter++;
            fmt << "icons/custom-icon-" << icon_counter << ".png";
            file_name = fmt.str();
        }
    }
    L("View::View(World &, Dimensions ) end");
}

Basic_position<int> View::_screen_location (const Location &location)
{
    L("View::_screen_location (const Location &) start");
    L("View::_screen_location (const Location &) end");
    return {
        (int)((double)_scene_dimensions.width / 2 + location.x * _ratio - _ratio  / 2),
        (int)((double)_scene_dimensions.height / 2 + location.y * _ratio - _ratio  / 2)
    };
}

void View::draw_scene(Sprite_set& sprites, vector<Agent_data> agents, const string& text)
{
    L("View::draw_scene(Sprite_set& , vector<Agent_data> , const string& ) start");
    fps.reconfigure(Text_sprite::Builder(sans) << text);
    sprites.add_sprite(fps, {10, 10});
    _draw_world(sprites);
    for (unsigned int i =0 ; i< agents.size(); i++) {
        sprites.add_sprite(_cell_sprites[agents[i].color], _screen_location(agents[i].cell.location),1);
    }
    L("View::draw_scene(Sprite_set& , vector<Agent_data> , const string& ) end");
}

void View::draw_editor(ge211::Sprite_set &sprites, int32_t index, std::vector<Cell_group_view> groups, const std::string& text) {
    L("View::draw_editor(ge211::Sprite_set &, int32_t , std::vector<Cell_group_view> , const std::string& ) start");
    if (!text.empty()) {
        fps.reconfigure(Text_sprite::Builder(sans) << text);
        sprites.add_sprite(fps, {10, 10});
    }
    _draw_world(sprites);
    for (uint32_t group_index = 0; group_index < groups.size() ; group_index ++) {
        Cell_group_view &gv = groups[group_index];
        if (gv.show) {
            Cell_group &cells = gv.cells;
            for (unsigned int i = 0; i < cells.size(); i++) {
                sprites.add_sprite(_cell_sprites[gv.color], _screen_location(cells[i].location), 2 + (int)group_index);
            }
        }
    }
    if (index>=0) {
        if (_world[index].occluded)
            sprites.add_sprite(_cell_sprites[12], _screen_location(_world[index].location),255);
        else
            sprites.add_sprite(_cell_sprites[3], _screen_location(_world[index].location),255);
    }
    L("View::draw_editor(ge211::Sprite_set &, int32_t , std::vector<Cell_group_view> , const std::string& ) end");
}

void View::_draw_world(ge211::Sprite_set &sprites) {
    L("View::_draw_world(ge211::Sprite_set &) start");
    for (unsigned int i = 0 ; i< _world.size(); i++) {
        const Cell &cell = _world[i];
        if (cell.occluded){
            sprites.add_sprite(_cell_sprites[9], _screen_location(cell.location),0);
        } else{
            sprites.add_sprite(_value_sprites[cell.value * 255], _screen_location(cell.location),0);
            if (cell.icon){
                sprites.add_sprite(cell.icon < 100 ?_icon_sprites[cell.icon]:_custom_icon_sprites[cell.icon], _screen_location(cell.location),100, Transform {}.scale((double)_cell_size/128.0).set_rotation(cell.direction.rotation()));
            }
        }
    }
    L("View::_draw_world(ge211::Sprite_set &) end");
}

int32_t View::get_cell(ge211::Position mouse_position) {
    L("View::get_cell(ge211::Position ) start");
    for (unsigned int i = 0 ; i< _world.size(); i++) {
        const Cell &cell = _world[i];
        ge211::Position cell_pos = _screen_location(cell.location);
        cell_pos.x += _cell_size;
        cell_pos.y += _cell_size;
        double distance = sqrt(pow(mouse_position.y-cell_pos.y,2) + pow(mouse_position.x-cell_pos.x,2));
        if (distance<=_cell_size){
            return i;
        }
    }
    L("View::get_cell(ge211::Position ) end");
    return Not_found;
}

std::string View::resource_file(std::string res) {
    L("View::resource_file(std::string ) start");
    string file_name(CELLWORLD_RESOURCES);
    L("View::resource_file(std::string ) end");
    return file_name + "/" + res;
}

bool View::file_exists (const std::string& name) {
    L("View::file_exists (const std::string& ) start");
    ifstream f(name.c_str());
    L("View::file_exists (const std::string& ) end");
    return f.good();
}