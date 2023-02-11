//
// Created by Garrett on 11/12/2022.
//

#ifndef MB2DC_PLAYER_H
#define MB2DC_PLAYER_H

#include <miniaudio/miniaudio.h>

#include <string>

namespace mb2dc {

class audio_player_t {
public:
    audio_player_t();
    ~audio_player_t();

    void play(const std::string &sound);

private:
    ma_engine engine_;
};

}

#endif //MB2DC_PLAYER_H
