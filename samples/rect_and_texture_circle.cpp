#include "samples_common.hpp"

int main()
{
    try {
        auto canvas = new_ref<canvas_t>(window_spec{.maximized_ = true});

        auto rect = new_ref<rect_t>("red quad", vec4{1.f, .2f, .2f, 1.f});
        canvas->draw_shape(rect);

        auto circle = new_ref<circle_t>(64);
        circle->translate({0.5f, 0.5f});
        circle->shader_ = new_ref<gl_shader_t>("skyline circle",
                                               std::string(RES_PATH) + "shaders/flat.vs.glsl",
                                               std::string(RES_PATH) + "shaders/tex.fs.glsl");
        circle->textures_.emplace("skyline", new_ref<gl_texture_t>(string(RES_PATH) + "img/skyline.jpg"));
        circle->shader_->set_uniform_int("uTex", 0);
        canvas->draw_shape(circle);

        auto trect = new_ref<rect_t>("orange rect");
        trect->translate({-0.5f, -0.5f});
        trect->shader_ = circle->shader_;
        trect->textures_.emplace("orange", new_ref<gl_texture_t>(string(RES_PATH) + "img/orange.png"));
        canvas->draw_shape(trect);

        canvas->run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }

    exit(EXIT_SUCCESS);
}
