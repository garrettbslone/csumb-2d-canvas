//
// Created by Garrett on 10/23/2022.
//

#include <mb2dc.hpp>

#include <iostream>
#include <unordered_map>

#include <glad/gl.h>

using namespace mb2dc;
using namespace glm;

using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::unordered_map;
using std::pair;
using std::swap;
using std::tolower;
using std::toupper;

// by default rects are 200 x 200
const double BACKGROUND_SCALE = 1.0 / 200.0;

// This checker board is 10x10, and I realized after making it that it should be
// 8x8 but the math is easier for 10x10.
const int BOARD_DIM = 10, EDGE = BOARD_DIM - 1;

char board[BOARD_DIM][BOARD_DIM] = {
        ' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r',
        'r', ' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r', ' ',
        ' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        'b', ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b', ' ',
        ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b',
        'b', ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b', ' '
};

struct game_state {
    enum _state {
        RED_TURN,
        BLACK_TURN,
        RED_WIN,
        BLACK_WIN
    };

    // since there is no text rendering/ui, use the window title to display the
    // game state for now
    static const unordered_map<_state, string> title_map;

    static unsigned short red_pieces, black_pieces;

    static _state state;

    static inline bool over()
    {
        return state == _state::RED_WIN || state == _state::BLACK_WIN;
    }

    static inline string title()
    {
        auto it = title_map.find(state);

        if (it != title_map.end()) {
            return it->second;
        }

        return "";
    }

    static inline bool valid_piece(int r, int c)
    {
        return (state == _state::RED_TURN && tolower(board[r][c]) == 'r') ||
               (state == _state::BLACK_TURN && tolower(board[r][c]) == 'b');
    }

    static inline void next_move()
    {
        if (state == _state::RED_TURN) {
            state = _state::BLACK_TURN;
        } else if (state == _state::BLACK_TURN) {
            state = _state::RED_TURN;
        }
    }
};

const unordered_map<game_state::_state, string> game_state::title_map = {
        {_state::RED_TURN, "Red Turn"},
        {_state::BLACK_TURN, "Black Turn"},
        {_state::RED_WIN, "Red Won!"},
        {_state::BLACK_WIN, "Black Won!"}
};

unsigned short game_state::red_pieces = 15, game_state::black_pieces = 15;
game_state::_state game_state::state = game_state::_state::RED_TURN;

/*
 * Need to keep track of 2 square selections at a time. The first_ is the starting
 * piece and the second_ is the potential ending position.
 */
struct selection_t {
    vec2 first_{-1}, second_{1};
    bool selected_{false};

    inline void reset()
    {
        this->first_ = vec2{-1};
        this->second_ = vec2{-1};
        this->selected_ = false;
    }
};

/*
 * Class representing the checker piece drawable object with a base of a circle.
 */
class checker_piece_t : public circle_t {
public:
    checker_piece_t()
            : circle_t(64, 1)
    {

    }

    void draw(const mat4 &view_proj) const override
    {
        this->va_->bind();
        this->shader_->bind();
        this->shader_->set_uniform_float("zIndex", this->z_index_);

        auto it = this->textures_.find(this->active_texture_);

        if (it != this->textures_.end()) {
            this->shader_->enable_texture("uTex", it->second);
        }

        this->shader_->set_uniform_mat4("MVP", this->model_ * view_proj);

        auto ib = this->va_->get_index_buffer();
        if (ib && ib->get_count() > 0) {
            glDrawElements(
                    get_topology(this->topo_),
                    (GLint) ib->get_count(),
                    GL_UNSIGNED_INT,
                    nullptr);
        } else {
            glDrawArrays(
                    get_topology(this->topo_),
                    0,
                    (GLint) this->va_->get_vertex_buffers()->get_data_size());
        }
    }

    /*
     * Since all of the checkers have the most of the same properties except board
     * position and the current color/type, we can draw all of the checkers on the
     * board using a single drawable by changing the color and position for each piece
     * in the board.
     */
    static void draw_all(checker_piece_t *checker,
                         const mat4 &view_proj,
                         const float checker_scale)
    {
        for (auto r = 0; r < BOARD_DIM; r++) {
            for (auto c = 0; c < BOARD_DIM; c++) {
                switch (board[r][c]) {
                case 'r':
                    checker->active_texture_ = "red";
                    break;
                case 'R':
                    checker->active_texture_ = "red_king";
                    break;
                case 'b':
                    checker->active_texture_ = "black";
                    break;
                case 'B':
                    checker->active_texture_ = "black_king";
                    break;
                }

                if (board[r][c] != ' ') {
                    // It would probably be more efficient to undo the translation after each
                    // iteration, but for simplicity the checker will be reset, rescaled, and
                    // translated for each individual piece
                    checker->reset_transformations();
                    checker->scale(vec2(checker_scale));
                    checker->translate({
                            checker_piece_t::translation_offset_ * (-EDGE + 2 * c),
                            checker_piece_t::translation_offset_ * (EDGE - 2 * r),
                    });
                    checker->draw(view_proj);
                }
            }
        }
    }

    string active_texture_;

    // This is really a scale factor that will be combined with the current window
    // dimension (or really the dimension of the square board in the window).
    static constexpr float CHECKER_SCALE = 1.0 / 22.5;
    // The offset to move a checker_piece_t to the center of one of the grid squares
    // on the board.
    static float translation_offset_;
};

float checker_piece_t::translation_offset_ = 0.0;

/*
 * Remove the pieces n from board_addrs by setting each board_addr to ' ' and
 * update the number of pieces remaining on the board.
 */
static void remove_pieces(char **board_addrs, int n)
{
    if (n < 1 || !board_addrs || !board_addrs[0]) {
        return;
    }

    char color = *(board_addrs[0]);

    for (auto i = 0; i < n; i++) {
        *board_addrs[i] = ' ';
    }

    if (tolower(color) == 'b') {
        game_state::black_pieces -= n;
    } else if (tolower(color) == 'r') {
        game_state::red_pieces -= n;
    }
}

/*
 * | |r| | | | |     | | | | | | |
 * | | |b| | | |     |r| |b| | | |
 * | | | | | | | ==> | | | |r| | |
 * | | |b| |b| |     | | |b| |b| |
 * | | | | | | |     | |r| | | |r|
 *
 * Checks if a regular piece (not king) can move from [r, c] -> [move_r, move_c]
 * allowing a maximum of 2 jumps in one turn. If remove is set, any pieces jumped
 * will be removed from the board.
 */
static bool is_valid_move_reg(int r, int c, int move_r, int move_c, bool remove)
{
    char other = tolower(board[r][c]) == 'b' ? 'r' : 'b';

    // regular red pieces should be moving down the board so move_r > r
    // and regular black pieces should be moving up the board so r > move_r
    if ((other == 'b' && r > move_r) || (other == 'r' && r < move_r)) {
        return false;
    }

    int dist_r = move_r - r, dist_c = move_c - c, y_dir = other == 'b' ? 1 : -1;

    // standard move to an empty adjacent, diagonal square
    if (dist_r == y_dir && abs(dist_c) == 1) {
        return true;
    }

    // only a max of 2 to remove at a time
    // capping at double jumps for now
    char *to_remove[2];

    // single jump down/up 2 over 2 (left or right)
    if (dist_r == y_dir * 2 && abs(dist_c) == 2 && tolower(board[r + y_dir][c + (dist_c / 2)]) == other) {
        // take the other piece off
        to_remove[0] = &board[r + y_dir][c + (dist_c / 2)];

        if (remove) {
            remove_pieces(to_remove, 1);
        }

        return true;
    }

    // double jump down/up 4 over 0
    if (dist_r == y_dir * 4 && dist_c == 0) {
        // the middle between the other pieces being jumped must be empty (only on one side)
        // |r| | | | |
        // | |b| | | |
        // |x| |x| | |
        // | |b| | | |
        // | | | | | |
        // from this picture either of the x spots needs to be empty for r to double jump black
        bool valid_jump = tolower(board[r + y_dir * 2][c + 2]) == ' ' || tolower(board[r + y_dir * 2][c]) == ' ';

        if (tolower(board[r + y_dir][c + 1]) == other &&
                tolower(board[r + y_dir * 3][c + 1]) == other && valid_jump) {
            to_remove[0] = &board[r + y_dir][c + 1];
            to_remove[1] = &board[r + y_dir * 3][c + 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        valid_jump = tolower(board[r + y_dir * 2][c - 2]) == ' ' || tolower(board[r + y_dir * 2][c]) == ' ';

        if (tolower(board[r + y_dir][c - 1]) == other &&
                tolower(board[r + y_dir * 3][c - 1]) == other && valid_jump) {
            to_remove[0] = &board[r + y_dir][c - 1];
            to_remove[0] = &board[r + y_dir * 3][c - 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        return false;
    }

    int x_dir = dist_c / 4;

    // double jump diagonal down 4 over 4 (left or right)
    if (dist_r == y_dir * 4 && abs(dist_c) == 4 && tolower(board[r + y_dir][c + x_dir]) == other &&
            tolower(board[r + y_dir * 3][c + x_dir * 3]) == other) {
        to_remove[0] = &board[r + y_dir][c + x_dir];
        to_remove[1] = &board[r + y_dir * 3][c + x_dir * 3];

        if (remove) {
            remove_pieces(to_remove, 2);
        }

        return true;
    }

    return false;
}

/*
 * Checks if a king can move from [r, c] -> [move_r, move_c] allowing a maximum of 2
 * jumps in one turn. If remove is set, any pieces jumped will be removed from the board.
 */
static bool is_valid_move_king(int r, int c, int move_r, int move_c, bool remove)
{
    char other = tolower(board[r][c]) == 'b' ? 'r' : 'b';
    int dist_r = move_r - r, dist_c = move_c - c;

    // standard move to an empty adjacent, diagonal square
    if (abs(dist_r) == 1 && abs(dist_c) == 1) {
        return true;
    }

    char *to_remove[2];

    // single jump down/up 2 over 2 (left or right)
    if (abs(dist_r) ==  2 && abs(dist_c) == 2) {
        // down 2
        if (tolower(board[r + 1][c + (dist_c / 2)]) == other) {
            to_remove[0] = &board[r + 1][c + (dist_c / 2)];

            if (remove) {
                remove_pieces(to_remove, 1);
            }

            return true;
        }

        // up 2
        if (tolower(board[r - 1][c + (dist_c / 2)]) == other) {
            to_remove[0] = &board[r - 1][c + (dist_c / 2)];

            if (remove) {
                remove_pieces(to_remove, 1);
            }

            return true;
        }
    }

    // double jump down/up 4 over 0
    if (abs(dist_r) == 4 && dist_c == 0) {
        bool valid_jump = tolower(board[r + 2][c + 2]) == ' ' || tolower(board[r + 2][c]) == ' ';

        // down 4, jumps on right
        if (tolower(board[r + 1][c + 1]) == other &&
                tolower(board[r + 3][c + 1]) == other && valid_jump) {
            to_remove[0] = &board[r + 1][c + 1];
            to_remove[1] = &board[r + 3][c + 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        valid_jump = tolower(board[r - 2][c + 2]) == ' ' || tolower(board[r - 2][c]) == ' ';

        // up 4, jumps on right
        if (tolower(board[r - 1][c + 1]) == other &&
                tolower(board[r - 3][c + 1]) == other && valid_jump) {
            to_remove[0] = &board[r + 1][c + 1];
            to_remove[1] = &board[r + 3][c + 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        valid_jump = tolower(board[r + 2][c - 2]) == ' ' || tolower(board[r + 2][c]) == ' ';

        // down 4, jumps on left
        if (tolower(board[r + 1][c - 1]) == other &&
                tolower(board[r + 3][c - 1]) == other && valid_jump) {
            to_remove[0] = &board[r + 1][c - 1];
            to_remove[1] = &board[r + 3][c - 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        valid_jump = tolower(board[r - 2][c - 2]) == ' ' || tolower(board[r + 2][c]) == ' ';

        // up 4, jumps on left
        if (tolower(board[r - 1][c - 1]) == other &&
                tolower(board[r - 3][c - 1]) == other && valid_jump) {
            to_remove[0] = &board[r - 1][c - 1];
            to_remove[1] = &board[r - 3][c - 1];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        return false;
    }

    int x_dir = dist_c / 4;

    // double jump diagonal down 4 over 4 (left or right)
    if (abs(dist_r) == 4 && abs(dist_c) == 4) {
        // down 4
        if (tolower(board[r + 1][c + x_dir]) == other && tolower(board[r + 3][c + x_dir * 3]) == other) {
            to_remove[0] = &board[r + 1][c + x_dir];
            to_remove[1] = &board[r + 3][c + x_dir * 3];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }

        // up 4
        if (tolower(board[r - 1][c + x_dir]) == other && tolower(board[r - 3][c + x_dir * 3]) == other) {
            to_remove[0] = &board[r - 1][c + x_dir];
            to_remove[1] = &board[r - 3][c + x_dir * 3];

            if (remove) {
                remove_pieces(to_remove, 2);
            }

            return true;
        }
    }

    return false;
}

/*
 * Check whether or not [r, c] is a black square.
 * All black squares are on either an even row and odd column or
 * and odd row and even column.
 */
bool on_black_square(int r, int c)
{
    return (r & 1 && !(c & 1)) || (c & 1 && !(r & 1));
}

/*
 * Returns whether a move from [r, c] -> [move_r, move_c] is possible. If remove
 * is set, any pieces jumped will be removed from the board.
 */
static bool move_is_valid(int r, int c, int move_r, int move_c, bool remove)
{
    // r can't be the same as move_r and c can't be the same as move_c
    // but we don't need to explicitly check for it since if they are trying to
    // move to the same space teh board[move_r][move_c] won't be empty and will
    // fail here anyways
    if (r < 0 || r >= BOARD_DIM ||
        c < 0 || c >= BOARD_DIM ||
        move_r < 0 || move_r >= BOARD_DIM ||
        move_c < 0 || move_c >= BOARD_DIM ||
        board[r][c] == ' ' ||
        board[move_r][move_c] != ' ' ||
        !on_black_square(r, c) ||
        !on_black_square(move_r, move_c)) {
        return false;
    }

    switch (board[r][c]) {
    case 'b':
    case 'r':
        return is_valid_move_reg(r, c, move_r, move_c, remove);
    case 'B':
    case 'R':
        return is_valid_move_king(r, c, move_r, move_c, remove);
    default:
        return false;
    }
}

/*
 * Did a regular red piece make it to the bottom or a regular black piece make
 * it to the top of the board?
 */
static bool to_king(int r, char piece)
{
    return (piece == 'b' && r == 0) || (piece == 'r' && r == EDGE);
}

/*
 * If a piece has been selected at [r, c] it will either:
 *  1) Show all possible moves that p can make by highlighting them blue
 *  2) Check if the move is valid and remove jumped pieces if any
 */
static void show_possible_moves(rect_t *move_rect,
                                const mat4 &view_proj,
                                float scale,
                                selection_t &selection,
                                uint32_t window_dim)
{
    int r = selection.first_.x, c = selection.first_.y;
    int move_r = selection.second_.x, move_c = selection.second_.y;

    const double RECT_BOUND = (1000./window_dim) * 1.8, RECT_STEP = 0.5;

    // Try to move p from [r, c] -> [move_r, move_c]
    if (selection.selected_ && move_r >= 0 && move_r < BOARD_DIM && move_c >= 0 && move_c < BOARD_DIM) {
        if (move_is_valid(r, c, move_r, move_c, true)) {
            if (to_king(move_r, board[r][c])) {
                board[r][c] = static_cast<char>(toupper(board[r][c]));
            }

            swap(board[r][c], board[move_r][move_c]);
            game_state::next_move();
        }

        selection.reset();

    // Highlight the possible moves from [r, c]
    } else if (r >= 0 && r < BOARD_DIM && c >= 0 && c < BOARD_DIM && board[r][c] != ' ') {
        // highlight selected piece
        move_rect->reset_transformations();
        move_rect->scale({scale, scale});
        move_rect->translate({
                -RECT_BOUND + c * RECT_STEP, RECT_BOUND - r * RECT_STEP
        });
        move_rect->draw(view_proj);

        // show possible moves
        // for now just check every other square which is checking 99 possible moves every time
        // there has to be a better way to do this
        for (auto i = 0; i < BOARD_DIM; i++) {
            for (auto j = 0; j < BOARD_DIM; j++) {
                if (move_is_valid(r, c, i, j, false)) {
                    move_rect->reset_transformations();
                    move_rect->scale({scale, scale});
                    move_rect->translate({
                            -RECT_BOUND + j * RECT_STEP, RECT_BOUND - i * RECT_STEP
                    });
                    move_rect->draw(view_proj);
                }
            }
        }
    } else {
        selection.reset();
    }
}

int main(int argc, char *argv[])
{
    // declare these in main so they can be captured into the window resize callback
    // we want the board to always be a square, so only keep 'window_dim' for window
    // width and height
    // I would suggest keeping this between 500-1000
    unsigned int window_dim = 800;

    try {
        auto canvas = new_ref<canvas_t>(window_spec{"checkers", window_dim, window_dim});
        canvas->window_->clear_clr_ = {0.28125f, 0.0546875f, 0.0078125f, 1.f};
        // don't allow the window to be resized because the translations get messed up on
        // resize for some reason
        canvas->fixed_size();

        auto background = new_ref<rect_t>();
        background->shader_ = new_ref<gl_shader_t>("board background",
                                                   std::string(RES_PATH) + "shaders/flat.vs.glsl",
                                                   std::string(RES_PATH) + "shaders/tex.fs.glsl");
        background->textures_.emplace("checker board",
                                      new_ref<gl_texture_t>(string(RES_PATH) + "img/checkerboard.png"));
        background->textures_.begin()->second->alpha_ = 1.f;
        background->shader_->set_uniform_int("uTex", 0);
        background->scale({window_dim * BACKGROUND_SCALE, window_dim * BACKGROUND_SCALE});
        canvas->draw_shape(background);

        // the blue highlight square
        auto move_rect = new_ref<rect_t>("move rect", vec4{0.09375, 0.9375, 0.91015625, 0.3});
        move_rect->set_z_index(50.f);
        canvas->draw_shape(move_rect);

        auto checker = new_ref<checker_piece_t>();
        auto checker_scale = window_dim * checker_piece_t::CHECKER_SCALE;
        checker_piece_t::translation_offset_ = 1.0 / (10.0 * checker_scale);
        checker->set_z_index(10.f);
        checker->textures_["red"] = new_ref<gl_texture_t>(string(RES_PATH) + "img/red_checker.png");
        checker->textures_["black"] = new_ref<gl_texture_t>(string(RES_PATH) + "img/black_checker.png");
        checker->textures_["red_king"] = new_ref<gl_texture_t>(string(RES_PATH) + "img/red_checker_king.png");
        checker->textures_["black_king"] = new_ref<gl_texture_t>(string(RES_PATH) + "img/black_checker_king.png");
        checker->shader_ = background->shader_;
        canvas->draw_shape(checker);

        background->set_z_index(-10);

        canvas->on_window_resize([&background, &window_dim, &move_rect, &checker_scale](int width, int height)
        {
            background->reset_transformations();

            // keep the board a square
            window_dim = width < height ? width : height;

            background->scale(vec2{static_cast<float>(window_dim * BACKGROUND_SCALE)});
            move_rect->scale(vec2{static_cast<float>(window_dim / BOARD_DIM * BACKGROUND_SCALE)});
            checker_scale = window_dim * checker_piece_t::CHECKER_SCALE;
            cout << checker_piece_t::translation_offset_ << endl;
            checker_piece_t::translation_offset_ = 1.0 / (10.0 * checker_scale);
            cout << checker_piece_t::translation_offset_ << endl;
        });

        selection_t selected;
        canvas->window_->data_.input_->mouse_btn_down_ = [&selected, &canvas, &window_dim] (int btn)
        {
            if (static_cast<mouse_btn>(btn) == MOUSE_BTN_LEFT) {
                auto pos = canvas->window_->data_.input_->get_mouse_pos_real();

                // convert to a square on the board
                int square_width = window_dim / 10;
                int r = floor(pos.y / square_width);
                int c = floor(pos.x / square_width);

                if (!selected.selected_ && game_state::valid_piece(r, c)) {
                    selected.first_ = {r, c};
                    selected.selected_ = true;
                } else {
                    selected.second_ = {r, c};
                }
            }
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        canvas->on_update([&checker_scale, &selected, &window_dim, &canvas]
            (const vector<drawable_t *> &nodes, const mat4 &view_proj)
        {
            if (game_state::over()) {
                return;
            }

            canvas->window_->set_title(game_state::title());

            for (auto &n: nodes) {
                if (dynamic_cast<checker_piece_t *>(n)) {
                    checker_piece_t::draw_all(dynamic_cast<checker_piece_t *>(n), view_proj, checker_scale);
                } else if (n->name_ == "move rect") {
                    show_possible_moves(
                            dynamic_cast<rect_t *>(n),
                            view_proj,
                            window_dim / (BOARD_DIM) * BACKGROUND_SCALE,
                            selected,
                            window_dim);
                } else {
                    n->draw(view_proj);
                }
            }

            if (game_state::red_pieces == 0) {
                game_state::state = game_state::_state::BLACK_WIN;
            } else if (game_state::black_pieces == 0) {
                game_state::state = game_state::_state::RED_WIN;
            }
        });

        canvas->run(true);
    } catch (mb2dc_runtime_ex &ex) {
        cout << ex.msg_ << endl;
    }

    exit(EXIT_SUCCESS);
}
