#include "App.hpp"

#include <iostream>

#include <SDL.h>
#include <stdexcept>
#include <random>

#include <algorithm>

const char* Title = "Pong";
constexpr int WindowWidth = 500, WindowHeight = 400;
constexpr int PaddleWidth = 10, PaddleHeight = 75;
constexpr float PaddleSpeed = 150.f;
constexpr float BallRadius = 10.f;
constexpr float SlowBallSpeed = 200.f;
constexpr float BallSpeed = 300.f;
constexpr float ReflectAngle = M_PI/3;

App::App()
    : eng{rd()}
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        throw std::runtime_error{"SDL_Init failed"};
    }

    window = SDL_CreateWindow(
        Title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WindowWidth, WindowHeight,
        SDL_WINDOW_SHOWN
    );

    if(window == nullptr){
        throw std::runtime_error{"SDL_CreateWindow failed"};
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if(renderer == nullptr){
        throw std::runtime_error{"SDL_CreateRenderer failed"};
    }



    leftPaddle = SDL_FRect {
        .x = 0, .y = (WindowHeight - PaddleHeight)/2,
        .w = 10, .h = PaddleHeight,
    };
    rightPaddle = SDL_FRect {
        .x = WindowWidth - PaddleWidth, .y = (WindowHeight - PaddleHeight)/2,
        .w = PaddleWidth, .h = PaddleHeight,
    };
    ResetBall();

    time = 0;
    SDL_Delay(1000/60);
}

void App::UpdateTime(){
    deltaTime = SDL_GetTicks()/1000.f - time;
    time = SDL_GetTicks()/1000.f;
}

void App::Run(){
    SDL_Event event;
    while(true){
        UpdateTime();

        while(SDL_PollEvent(&event) == 1){
            if(event.type == SDL_QUIT){
                return;
            }
            if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                if(event.key.keysym.scancode == SDL_SCANCODE_UP){
                    upPressed = event.key.state == SDL_PRESSED;
                }

                if(event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                    downPressed = event.key.state == SDL_PRESSED;
                }
            }
        }
        
        Update();
        Draw();
    }
}

void App::Update(){


    leftPaddle.y += (upPressed && !downPressed ? -1 : downPressed ? 1 : 0) * PaddleSpeed * deltaTime;
    leftPaddle.y = std::clamp(leftPaddle.y, 0.f, static_cast<float>(WindowHeight) - PaddleHeight);

    auto prevBall = ball;

    ball.x += ballVelocity.x * deltaTime;
    ball.y += ballVelocity.y * deltaTime;

    if(ball.y > WindowHeight - BallRadius && ballVelocity.y > 0){
        ballVelocity.y *= -1;
    }

    if (ball.y < 0 && ballVelocity.y < 0){
        ballVelocity.y *= -1;
    }

    if(prevBall.x + BallRadius < rightPaddle.x && ball.x + BallRadius > rightPaddle.x){
        if(std::abs((ball.y+BallRadius/2)-(rightPaddle.y + PaddleHeight/2)) < PaddleHeight/2){
            ballVelocity = Vector2::Polar(
                BallSpeed,
                std::lerp(M_PI-ReflectAngle, M_PI+ReflectAngle, 1 - (ball.y + BallRadius - rightPaddle.y)/PaddleHeight)
            );
        }
    }

    if(prevBall.x > PaddleWidth && ball.x < PaddleWidth){
        if(std::abs((ball.y+BallRadius/2)-(leftPaddle.y + PaddleHeight/2)) < PaddleHeight/2){
            ballVelocity = Vector2::Polar(
                BallSpeed,
                std::lerp(-ReflectAngle, ReflectAngle, (ball.y + BallRadius - leftPaddle.y)/PaddleHeight)
            );
        }
    }

    if(ball.x < 0){
        std::cout << "You Lose" << std::endl;
        ResetBall();
    }
    else if(ball.x + BallRadius > WindowWidth){
        std::cout << "You Win" << std::endl;
        ResetBall();
    }

    if(ball.y > (rightPaddle.y + PaddleHeight/2)){
        rightPaddle.y += PaddleSpeed*deltaTime;
    }

    if(ball.y < (rightPaddle.y + PaddleHeight/2)){
        rightPaddle.y -= PaddleSpeed*deltaTime;
    }

    rightPaddle.y = std::clamp(rightPaddle.y, 0.f, static_cast<float>(WindowHeight) - PaddleHeight);
}



void App::Draw(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(renderer, &leftPaddle);
    SDL_RenderFillRectF(renderer, &rightPaddle);
    SDL_RenderFillRectF(renderer, &ball);

    SDL_RenderPresent(renderer);
}


App::~App(){
    if(window != nullptr){
        SDL_DestroyWindow(window);
    }
}


void App::ResetBall(){
    static std::uniform_real_distribution<float> dist{(3*M_PI)/4, (5*M_PI)/4};
    ball = SDL_FRect {
        .x = (WindowWidth-BallRadius)/2, .y = (WindowHeight-BallRadius)/2, .w = BallRadius, .h = BallRadius,
    };

    ballVelocity = Vector2::Polar(1, dist(eng)) * SlowBallSpeed;
}