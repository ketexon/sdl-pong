#pragma once

#include <SDL.h>
#include <cmath>
#include <random>

struct Vector2 {
    float x, y;
    Vector2() : x{0}, y{0} {}
    Vector2(float x, float y) : x{x}, y{y} {}

    Vector2 operator+(const Vector2& other){
        return Vector2 {
            x + other.x, y + other.y
        };
    }

    Vector2 operator-(const Vector2& other){
        return Vector2 {
            x - other.x, y - other.y
        };
    }

    Vector2 operator*(float scalar){
        return Vector2 {
            x * scalar, y * scalar
        };
    }

    Vector2 operator/(float scalar){
        return operator*(1/scalar);
    }

    float Magnitude(){
        return std::sqrtf(x * x + y * y);
    }

    Vector2 Normalized(){
        return operator*(1/Magnitude());
    }

    float Dot(Vector2 other){
        return x * other.x + y * other.y;
    }

    static Vector2 Polar(float r, float theta){
        return Vector2 {
            r * std::cos(theta),
            r * std::sin(theta)
        };
    }
};


class App {
public:
    App();

    void Run();
    void UpdateTime();
    void Update();
    void Draw();

    ~App();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_FRect leftPaddle, rightPaddle, ball;
    Vector2 ballVelocity;

    std::random_device rd;
    std::default_random_engine eng;


    bool upPressed = false, downPressed = false;

    float time = 0, deltaTime = 0;

    void ResetBall();
};