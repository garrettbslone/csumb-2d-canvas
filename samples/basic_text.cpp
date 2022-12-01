#include <mb2dc.hpp>
#include <iostream>

using namespace glm;
using namespace std;
using namespace mb2dc;

int main()
{
    try {
        canvas_t canvas(window_spec{.maximized_ = true});

        auto x = (-1.0 * canvas.window_->width() / 2.0);
        auto y = canvas.window_->height() / 2.0;
        int secs = 0, fc = 0;

        auto time = canvas.overlay_->draw_text("Time: " + to_string(secs), font_t::inkfree(), {x - (x / 2.95), y - (y / 2.75)}, 0.25);
        auto fps = canvas.overlay_->draw_text("fps: ", font_t::inkfree(), {x - (x / 2.25), y - (y / 2.75)}, 0.25);
        auto changing = canvas.overlay_->draw_text("Start Typing...", font_t::inkfree(), {0.f, 0.f}, 1.f);
        bool changed = false;

        canvas.window_->data_.input_->key_down_ = [&] (int k)
        {
            if (!changed) {
                changing->change(reinterpret_cast<char *>(&k));
                changed = true;
            }

            auto _k = static_cast<key>(k);
            if (_k == KEY_ESCAPE) {
                canvas.close();
            } else if (_k == KEY_DELETE || _k == KEY_BACKSPACE) {
                changing->backspace();
                return;
            } else if (_k == KEY_ENTER) {
                changing->clear();
                return;
            }

            changing->append(reinterpret_cast<char *>(&k));
        };

        canvas.on_window_resize([&] (int w, int h)
        {
            x = (-1.0 * w / 2.0);
            y = h / 2.0;

            time->reposition({x - (x / 2.95), y - (y / 2.75)});
            fps->reposition({x - (x / 2.25), y - (y / 2.75)});
            changing->reposition({0.0, 0.0});
        });

        canvas.overlay_->on_update([&] (const vector<ui_element_t *> &elements, const mat4 &view_proj)
        {
            if (floor(glfwGetTime()) >= secs) {
                secs++;
                time->change(to_string(secs), 6);
                fps->change(to_string(fc), 5);
                fc = 0;
            }

            for (auto &e: elements) {
                e->draw(view_proj);
            }

            fc++;
        });

        canvas.run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }
}