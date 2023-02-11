//
// Created by Garrett on 11/28/2022.
//

#include "samples_common.hpp"

int main()
{
    try {
        canvas_t canvas(window_spec{.maximized_ = true});

        auto x = (-1.0 * canvas.width() / 2.0);
        auto y = canvas.height() / 2.0;
        int secs = 0, fc = 0;

        auto time = canvas.draw_text("Time: " + to_string(secs), font_t::inkfree(), {x - (x / 2.95), y - (y / 2.75)}, 0.25);
        auto fps = canvas.draw_text("fps: ", font_t::inkfree(), {x - (x / 2.25), y - (y / 2.75)}, 0.25);
        auto changing = canvas.draw_text("Start Typing...", font_t::inkfree(), {0.f, 0.f}, 1.f);
        bool changed = false;

        auto btn = new_ref<button_t>();
        btn->translate({-0.85f, 0.85f});
        btn->set_z_index(-1.f);
        btn->on_click([&] (int btn, clickable_t *thiz)
        {
            auto _p = canvas.get_mouse_pos_rel();
            cout << "btn clicked id: " << thiz->id() << " - " << dynamic_cast<button_t *>(thiz)->name_ << endl;
            changing->clear();
        });
        canvas.draw_ui_element(btn);

        auto p = btn->convert_to_spos(btn->center());
        auto btn_msg = canvas.draw_text("Clear Text", font_t::arial(), p / glm::vec2(1.415f, 1.525f), 0.2f);
        btn_msg->set_z_index(100.f);

        canvas.on_key_down([&] (int k)
        {
            if (!changed) {
                changing->clear();
                changed = true;
            }

            auto _k = static_cast<key_code>(k);

            if (_k == KEY_ESCAPE) {
                canvas.close();
            } else if (_k == KEY_DELETE || _k == KEY_BACKSPACE) {
                changing->backspace();
                return;
            } else if (_k == KEY_ENTER) {
                changing->clear();
                return;
//            } else if (!std::isprint(k)) {
//                return;
            } else if ((_k >= KEY_A && _k <= KEY_Z) && canvas.get_key(KEY_LEFT_SHIFT)) {
                _k = static_cast<key_code>(k - 'a' - 'A');
            }

            changing->append(reinterpret_cast<char *>(&k));
        });

        canvas.on_window_resize([&] (int w, int h)
        {
            x = (-1.0 * w / 2.0);
            y = h / 2.0;

            time->reposition({x - (x / 2.95), y - (y / 2.75)});
            fps->reposition({x - (x / 2.25), y - (y / 2.75)});
            changing->reposition({0.0, 0.0});

            btn->translate({-0.85f, 0.85f});
            p = btn->convert_to_spos(btn->center());
            btn_msg->reposition(p / glm::vec2(1.415f, 1.525f));
        });

        canvas.on_ui_update([&] (const vector<ui_element_t *> &elements, const mat4 &view_proj)
        {
            auto _time = glfwGetTime();
            if (floor(_time) >= secs) {
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

    return 0;
}