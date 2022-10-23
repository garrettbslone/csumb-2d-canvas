#include <glm/ext.hpp>
#include <mb2dc.hpp>
#include <iostream>

using namespace glm;
using namespace std;
using namespace mb2dc;

int main(int argc, char *argv[])
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
        circle->texture_ = new_ref<gl_texture_t>(string(RES_PATH) + "img/skyline.jpg");
        circle->shader_->set_uniform_int("uTex", 0);
        canvas->draw_shape(circle);

        canvas->on_update([&, canvas] (const vector<mb2dc::ref<drawable_t>> &nodes, const mat4 &view_proj)
        {
            for (const auto& n: nodes) {
                if (n->texture_) {
                    n->texture_->bind();
                    n->shader_->set_uniform_int("uTex", n->texture_->slot());
                }

                n->shader_->bind();
                n->shader_->set_uniform_mat4("MVP", n->get_model_mat() * view_proj);
                n->draw();
            }
        });

        canvas->run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }

    exit(EXIT_SUCCESS);
}
