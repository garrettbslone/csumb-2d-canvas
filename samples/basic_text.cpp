//
// Created by Garrett on 11/28/2022.
//

#include "samples_common.hpp"

#define CURSOR_TRACKING_POS {\
    changing->width() * CURSOR_SCALE_W / canvas.width(),\
    CURSOR_SCALE_H/ (changing->height() * canvas.height())\
}

constexpr float CURSOR_SCALE_W = 100.f, CURSOR_SCALE_H = 200.f;

int main()
{
    try {
        canvas_t canvas(window_spec{.maximized_ = true});

        int secs = 0, fc = 0;

        auto time = canvas.draw_ui_text(
            "Time: " + to_string(secs),
            font_t::inkfree(),
            align::HZ_LEFT | align::VT_TOP,
            0.25
        );
        auto fps = canvas.draw_ui_text(
            "fps: ",
            font_t::inkfree(),
            align::HZ_RIGHT | align::VT_TOP,
            0.25
        );
        auto changing = canvas.draw_ui_text(
            "Start Typing...",
            font_t::inkfree(),
            align::VT_CENTER | align::HZ_CENTER,
            1.f
        );

        auto cursor = new_ref<rect_t>("cursor", vec4(1.f));
        cursor->scale({0.01f, changing->height() / 200.f});
        cursor->translate(CURSOR_TRACKING_POS);
        cursor->shader_ = new_ref<gl_shader_t>(
            "blink",
            get_res("shaders/flat.vs.glsl"),
            get_res("shaders/blink.fs.glsl")
        );
        canvas.draw_shape(cursor);

        bool changed = false;

        auto btn = new_ref<button_t>();
        btn->translate(
            ui_overlay_t::calculate_alignment(
                align::VT_TOP | align::HZ_CENTER | align::ANCHOR_ORIGIN_HZ | align::ANCHOR_EDGE_VT,
                100,
                20,
                0.015,
                true
            )
        );
        btn->set_z_index(-1.f);
        btn->on_click([&] (int btn, const ref_t<clickable_t> &thiz)
        {
            auto _p = canvas.get_mouse_pos_rel();
            cout << "btn clicked: " << btn << " id: " << thiz->id() << " - ";
            cout << dynamic_pointer_cast<button_t>(thiz)->name_ << " @ [" << _p.x << ", " << _p.y << "]\n";
            changing->clear();
            cursor->translate(CURSOR_TRACKING_POS);
        });
        canvas.draw_ui_element(btn);

        auto btn_msg = btn->make_child<text_t>(
                "Clear Text",
                font_t::arial(),
                vec2{0, 0},
                0.2f
        );
        btn_msg->align(align::ANCHOR_EDGE | align::VT_TOP | align::HZ_CENTER);
        btn_msg->set_z_index(100.f);
        canvas.draw_ui_text(btn_msg);

        canvas.on_key_down([&] (int k, int mods)
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
                cursor->translate(CURSOR_TRACKING_POS);
                return;
            } else if (_k == KEY_ENTER) {
                changing->clear();
                cursor->translate(CURSOR_TRACKING_POS);
                return;
            }

            char key_str[2] = {key_code_to_ascii(_k, mods & KEY_MOD_SHIFT, mods & KEY_MOD_CAPS), '\0'};

            if (std::isprint(key_str[0])) {
                changing->append(key_str);
                cursor->translate(CURSOR_TRACKING_POS);
            }
        });

        canvas.on_ui_update([&] (const vector<ref_t<ui_element_t>> &elements, const mat4 &view_proj)
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

        canvas.on_update([&] (const vector<ref_t<drawable_t>> &elements, const mat4 &view_proj)
        {
            for (auto &e: elements) {
                e->shader_->bind();
                e->shader_->set_uniform_float("uTime", glfwGetTime());
                e->shader_->set_uniform_float("uRate", 5.f);
                e->draw(view_proj);
            }
        });

        canvas.run();
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    } catch (bad_weak_ptr &e) {
        cout << e.what();
    }

    return 0;
}