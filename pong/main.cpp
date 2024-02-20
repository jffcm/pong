#include <iostream>
#include <raylib.h>

const int SCREEN_WIDTH{1280};
const int SCREEN_HEIGHT{720};
const int BALL_SPEED_X{7};
const int BALL_SPEED_Y{7};
const int BALL_RADIUS{20};
const int PADDLE_SPEED{6};
const float PADDLE_WIDTH{25};
const float PADDLE_HEIGHT{120};

int player_score{0};
int cpu_score{0};

class Ball {
public:
    float x{}, y{};
    int speed_x{}, speed_y{};
    int radius{};

    Ball(float x, float y, int speed_x, int speed_y, int radius)
        : x(x), y(y), speed_x(speed_x), speed_y(speed_y), radius(radius) {}

    void Draw() const {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        if(x + radius >= GetScreenWidth()) {
            ++cpu_score;
            ResetBall();
        }
        
        if(x - radius <= 0) {
            ++player_score;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if(y <= 0) {
            y = 0;
        }

        if(y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public: 
    float x{}, y{};
    float width{}, height{};
    int speed{};

    Paddle(float x, float y, float width, float height, int speed) 
        : x(x), y(y), width(width), height(height), speed(speed) {}

    void Draw() const {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y -= speed;
        }

        if(IsKeyDown(KEY_DOWN)) {
            y += speed;
        }

        LimitMovement();
    }
};

class CpuPaddle: public Paddle {
public:
    CpuPaddle(float x, float y, float width, float height, int speed) 
        : Paddle(x, y, width, height, speed) {}

    void Update(int ball_y) { 
        if(y + height / 2 > ball_y) {
            y -= speed;
        }

        if(y + height / 2 <= ball_y) {
            y += speed;
        }

        LimitMovement();
    }
};

int main() {    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong game!");

    int posX{GetScreenWidth() / 2 - SCREEN_WIDTH / 2};
    int posY{GetScreenHeight() / 2 - SCREEN_HEIGHT / 2};

    SetWindowPosition(posX, posY);
    SetTargetFPS(60); 

    Ball ball((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), BALL_SPEED_X, BALL_SPEED_Y, BALL_RADIUS);
    Paddle player((SCREEN_WIDTH - 35), (SCREEN_HEIGHT / 2 - 60), PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED);
    CpuPaddle cpu(10, (SCREEN_HEIGHT / 2 - cpu.height / 2), PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED);

    while(!WindowShouldClose()) {
        BeginDrawing(); 

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        ClearBackground(BLACK);
        DrawLine((SCREEN_WIDTH / 2), 0, (SCREEN_WIDTH / 2), SCREEN_HEIGHT, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), (SCREEN_WIDTH / 4 - 20), 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), (3 * SCREEN_WIDTH / 4 - 20), 20, 80, WHITE);

        EndDrawing(); 
    }

    CloseWindow();
    return 0;
}