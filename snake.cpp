#include "raylib.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

static const string GFX = "graphics/";

const int cell_size = 40;
const int cell_number = 20;

// Simple integer grid vector (replaces pygame Vector2 for grid coords).
struct Vec2 {
    int x = 0;    int y = 0;
    bool operator==(const Vec2 &o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vec2 &o) const { return !(*this == o); }
    Vec2 operator+(const Vec2 &o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2 &o) const { return {x - o.x, y - o.y}; }
};

class Snake {
public:
    vector<Vec2> body;
    Vec2 direction;

    Texture2D head_up, head_down, head_left, head_right;
    Texture2D tail_up, tail_down, tail_left, tail_right;
    Texture2D body_horizontal, body_vertical;
    Texture2D body_br, body_bl, body_tl, body_tr;

    Texture2D head; // current head
    Texture2D tail; // current tail

    Snake() {
        body = {{6, 10}, {5, 10}, {4, 10}};
        direction = {1, 0};

        head_up    = LoadTexture((GFX + "head_up.png").c_str());
        head_down  = LoadTexture((GFX + "head_down.png").c_str());
        head_left  = LoadTexture((GFX + "head_left.png").c_str());
        head_right = LoadTexture((GFX + "head_right.png").c_str());

        tail_up    = LoadTexture((GFX + "tail_up.png").c_str());
        tail_down  = LoadTexture((GFX + "tail_down.png").c_str());
        tail_left  = LoadTexture((GFX + "tail_left.png").c_str());
        tail_right = LoadTexture((GFX + "tail_right.png").c_str());

        body_horizontal = LoadTexture((GFX + "body_horizontal.png").c_str());
        body_vertical   = LoadTexture((GFX + "body_vertical.png").c_str());

        body_br = LoadTexture((GFX + "body_br.png").c_str());
        body_bl = LoadTexture((GFX + "body_bl.png").c_str());
        body_tl = LoadTexture((GFX + "body_tl.png").c_str());
        body_tr = LoadTexture((GFX + "body_tr.png").c_str());

        head = head_right;
        tail = tail_left;
    }

    void unload() {
        UnloadTexture(head_up); UnloadTexture(head_down);
        UnloadTexture(head_left); UnloadTexture(head_right);
        UnloadTexture(tail_up); UnloadTexture(tail_down);
        UnloadTexture(tail_left); UnloadTexture(tail_right);
        UnloadTexture(body_horizontal); UnloadTexture(body_vertical);
        UnloadTexture(body_br); UnloadTexture(body_bl);
        UnloadTexture(body_tl); UnloadTexture(body_tr);
    }

    void create_snake() {
        update_graphics();

        for (size_t index = 0; index < body.size(); ++index) {
            Vec2 block = body[index];
            int pos_x = block.x * cell_size;
            int pos_y = block.y * cell_size;

            if (index == 0) {
                DrawTexture(head, pos_x, pos_y, WHITE);
            } else if (index == body.size() - 1) {
                DrawTexture(tail, pos_x, pos_y, WHITE);
            } else if ((body[index] - body[index - 1]) == (body[index + 1] - body[index])) {
                Vec2 d = body[index] - body[index - 1];
                if (d == Vec2{1, 0} || d == Vec2{-1, 0})
                    DrawTexture(body_horizontal, pos_x, pos_y, WHITE);
                else
                    DrawTexture(body_vertical, pos_x, pos_y, WHITE);
            } else {
                Vec2 f = body[index] - body[index - 1];
                Vec2 s = body[index + 1] - body[index];
                Vec2 diff = f - s;
                if (diff == Vec2{1, -1})       DrawTexture(body_bl, pos_x, pos_y, WHITE);
                else if (diff == Vec2{1, 1})   DrawTexture(body_tl, pos_x, pos_y, WHITE);
                else if (diff == Vec2{-1, -1}) DrawTexture(body_br, pos_x, pos_y, WHITE);
                else                           DrawTexture(body_tr, pos_x, pos_y, WHITE);
            }
        }
    }

    void add_body() {
        body.insert(body.begin(), body[0] + direction);
    }

    void move_snake() {
        vector<Vec2> copy(body.begin(), body.end() - 1);
        copy.insert(copy.begin(), copy[0] + direction);
        body = copy;
    }

    void update_graphics() {
        Vec2 hd = body[0] - body[1];
        if (hd == Vec2{0, -1})      head = head_up;
        else if (hd == Vec2{0, 1})  head = head_down;
        else if (hd == Vec2{-1, 0}) head = head_left;
        else                        head = head_right;

        size_t n = body.size();
        Vec2 td = body[n - 1] - body[n - 2];
        if (td == Vec2{0, -1})      tail = tail_up;
        else if (td == Vec2{0, 1})  tail = tail_down;
        else if (td == Vec2{-1, 0}) tail = tail_left;
        else                        tail = tail_right;
    }
};

class Fruit {
public:
    Vec2 pos;

    Fruit() { randomize(); }

    void create_fruit(Texture2D apple) {
        DrawTexture(apple, pos.x * cell_size, pos.y * cell_size, WHITE);
    }

    void randomize() {
        pos.x = GetRandomValue(0, cell_number - 1);
        pos.y = GetRandomValue(0, cell_number - 1);
    }
};

class Main {
public:
    Fruit fruit;
    Snake snake;
    Texture2D apple;
    Font game_font;

    Main() {
        apple = LoadTexture((GFX + "apple.png").c_str());
        game_font = GetFontDefault();
    }

    void unload() {
        UnloadTexture(apple);
        snake.unload();
    }

    void update() {
        snake.move_snake();
        check_collide();
        check_hit();
    }

    void draw_elements() {
        draw_grass();
        fruit.create_fruit(apple);
        snake.create_snake();
        display_score();
    }

    void check_collide() {
        if (fruit.pos == snake.body[0]) {
            fruit.randomize();
            snake.add_body();
        }
    }

    void check_hit() {
        if (snake.body[0].x == cell_number || snake.body[0].y == cell_number)
            game_over();
        if (snake.body[0].x < 0 || snake.body[0].y < 0)
            game_over();

        for (size_t i = 1; i < snake.body.size(); ++i) {
            if (snake.body[0] == snake.body[i])
                game_over();
            if (snake.body[i] == fruit.pos)
                fruit.randomize();
        }
    }

    void game_over() {
        snake.body = {{6, 10}, {5, 10}, {4, 10}};
    }

    void draw_grass() {
        ClearBackground(Color{170, 240, 170, 255}); // lightgreen
    }

    void display_score() {
        string score_text = to_string((int)snake.body.size() - 3);
        int font_size = 40;
        Vector2 ts = MeasureTextEx(game_font, score_text.c_str(), (float)font_size, 2);

        int score_x = cell_number * cell_size - 60;
        int score_y = cell_number * cell_size - 40;

        // draw score centered around (score_x, score_y)
        DrawTextEx(game_font, score_text.c_str(),
                   {score_x - ts.x / 2, score_y - ts.y / 2},
                   (float)font_size, 2, WHITE);

        // apple placed to the left of the score text
        int apple_x = (int)(score_x - ts.x / 2) - apple.width;
        int apple_y = score_y - apple.height / 2;
        DrawTexture(apple, apple_x, apple_y, WHITE);
    }
};

int main() {
    SetRandomSeed((unsigned int)time(nullptr));
    InitWindow(cell_size * cell_number, cell_size * cell_number, "Snake");
    SetTargetFPS(60);

    Main main_game;

    bool x_axis = true;
    bool y_axis = false;

    double last_update = GetTime();
    const double update_interval = 0.15; // 150 ms, matches SCREEN_UPDATE timer

    while (!WindowShouldClose()) {
        // timer-based logic update
        double now = GetTime();
        if (now - last_update >= update_interval) {
            main_game.update();
            last_update = now;
        }
        
        if (!y_axis) {
            if (IsKeyPressed(KEY_UP)) {
                main_game.snake.direction = {0, -1};
                y_axis = true; x_axis = false;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                main_game.snake.direction = {0, 1};
                y_axis = true; x_axis = false;
            }
        }
        if (!x_axis) {
            if (IsKeyPressed(KEY_LEFT)) {
                main_game.snake.direction = {-1, 0};
                y_axis = false; x_axis = true;
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                main_game.snake.direction = {1, 0};
                y_axis = false; x_axis = true;
            }
        }

        BeginDrawing();
        main_game.draw_elements();
        EndDrawing();
    }

    main_game.unload();
    CloseWindow();
    return 0;
}
