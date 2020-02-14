#pragma once
#include <core.h>
#include <cell_group.h>
#include <agent.h>

namespace cell_world{
    struct Cell_group_view{
        Cell_group cells;
        Color color;
        bool show;
    };
    struct View
    {
        View(Cell_group &, ge211::Dimensions);
        void draw_scene (ge211::Sprite_set& , std::vector<Agent_data>, const std::string&);
        void draw_editor (ge211::Sprite_set&, int32_t , std::vector<Cell_group_view>, const std::string&);
        int32_t get_cell(ge211::Position);
        static std::string resource_file(std::string res);
        static bool file_exists (const std::string&);
        private:
        void _draw_world(ge211::Sprite_set& sprites);
        ge211::Basic_position<int> _screen_location (const Location & );
        Cell_group &_cell_group;
        std::vector<ge211::Image_sprite> _icon_sprites;
        std::vector<ge211::Image_sprite> _custom_icon_sprites;
        std::vector<ge211::Circle_sprite> _circle_sprites;
        std::vector<ge211::Rectangle_sprite> _square_sprites;
        std::vector<ge211::Circle_sprite> _circle_value_sprites;
        std::vector<ge211::Rectangle_sprite> _square_value_sprites;
        ge211::Font sans{"sans.ttf", 30};
        ge211::Text_sprite fps;
        const ge211::Dimensions _scene_dimensions;
        double _ratio;
        int _cell_size;
    };
}