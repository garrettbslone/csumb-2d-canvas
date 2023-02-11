#include "samples_common.hpp"

int main(int argc, char *argv[])
{
    try {
        auto canvas = new_ref<canvas_t>(window_spec{.maximized_ = true});
        auto rect = new_ref<rect_t>();
        canvas->draw_shape(rect);
        canvas->play_audio(get_res("audio/explosion.wav"));
        canvas->run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }

    exit(EXIT_SUCCESS);
}
