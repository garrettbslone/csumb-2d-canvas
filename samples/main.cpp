#include <glm/ext.hpp>
#include <mb2dc.hpp>
#include <iostream>

using namespace glm;
using namespace std;
using namespace mb2dc;

int main(int argc, char *argv[])
{
    try {
        auto can = new_ref<canvas>(window_spec{.maximized_ = true});
        auto q = new_ref<quad>("red quad", vec4{1.f, .2f, .2f, 1.f});
        can->draw_shape(q);
        auto cir = new_ref<circle>(64);
        cir->translate({0.5f, 0.5f});
        can->draw_shape(cir);

        mat4 view_proj(1.f);
        float w = can->window_->get_width(), h = can->window_->get_height();

        view_proj = ortho(-w / 2, w / 2, -h / 2, h / 2, -100.f, 100.f);

        can->on_update([&, view_proj](const vector<mb2dc::ref<drawable>> &nodes) {
            for (const auto& n: nodes) {
                n->shader_->bind();
                n->shader_->set_uniform_mat4("MVP", n->get_model_mat() * view_proj);
                cout << n->name_ << endl;
                n->draw();
            }
        });

        can->run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }

    exit(EXIT_SUCCESS);
}
