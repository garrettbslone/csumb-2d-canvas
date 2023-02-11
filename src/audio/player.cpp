//
// Created by Garrett on 11/12/2022.
//

#define MINIAUDIO_IMPLEMENTATION
#include <audio/player.h>

#include <core/exception.hpp>

namespace mb2dc {

audio_player_t::audio_player_t()
{
    ma_result result = ma_engine_init(nullptr, &this->engine_);

    if (result != MA_SUCCESS) {
        throw audio_ex("Failed to initialize audio engine.");
    }
}

audio_player_t::~audio_player_t()
{
    ma_engine_uninit(&this->engine_);
}

void audio_player_t::play(const std::string &sound)
{
    ma_engine_play_sound(&this->engine_, sound.c_str(), nullptr);
}

}