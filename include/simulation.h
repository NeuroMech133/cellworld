#pragma once
#include <ge211.h>
#include <model.h>
#include <view.h>

namespace cell_world{
    class Simulation : public ge211::Abstract_game
    {
        public:
        Simulation (World &, std::vector<Agent*> &, ge211::Dimensions);
        Simulation (World &, std::vector<Agent*> &, ge211::Dimensions, uint16_t, uint32_t);
        ge211::Dimensions initial_window_dimensions() const override;
        void draw(ge211::Sprite_set&) override;
        bool is_paused = false;
        void on_key(ge211::Key) override;
        void on_frame(double) override;
        void run_silent();
        void run_silent(bool);
        uint32_t episode;

        private:
        Model _model;
        ge211::Dimensions _scene_dimensions;
        View _view;
        uint16_t _iterations;
        uint32_t _episodes;
    };
}