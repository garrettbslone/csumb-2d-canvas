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
        canvas->draw_shape(circle);

        mat4 view_proj(1.f);
        float w = canvas->window_->get_width(), h = canvas->window_->get_height();

        view_proj = ortho(-w / 2, w / 2, -h / 2, h / 2, -100.f, 100.f);

        canvas->window_->on_resize([&, view_proj, w, h] (uint32_t width, uint32_t height) mutable
        {
            w = static_cast<float>(width);
            h = static_cast<float>(height);

            view_proj = ortho(-w / 2, w / 2, -h / 2, h / 2, -100.f, 100.f);
        });

        canvas->on_update([&, view_proj] (const vector<mb2dc::ref<drawable_t>> &nodes)
        {
            for (const auto& n: nodes) {
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
