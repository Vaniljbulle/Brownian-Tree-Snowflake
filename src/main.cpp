#include <emscripten.h>
#include <SDL2/SDL.h>
#include <array>
#include <string>
#include <cmath>
#include "../include/Snowflake.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define PARTICLES_PER_FRAME 1
#define PARTICLE_RADIUS 2
#define SEGMENTS 3

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Snowflake snowflakeLogic = Snowflake(WINDOW_WIDTH, WINDOW_HEIGHT, SEGMENTS);

void drawFilledCircle(int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void rotatePoint(int cx, int cy, int angle, int &x, int &y) {
    float rad = angle * (M_PI / 180.0f);
    float s = sin(rad);
    float c = cos(rad);

    x -= cx;
    y -= cy;

    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    x = xnew + cx;
    y = ynew + cy;
}

void draw() {
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800,
                                                   800);
    SDL_SetRenderTarget(renderer, screenTexture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < SEGMENTS; i++) {
        int angle = (360 / SEGMENTS) * i;
        for (const auto &particle : snowflakeLogic.getParticles()) {
            int x = particle.x, y = particle.y;
            rotatePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, angle, x, y);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            drawFilledCircle(x, y, particle.radius);
            drawFilledCircle(x, WINDOW_HEIGHT - y, particle.radius);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(screenTexture);
}

void loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
            return;
        }
    }


    snowflakeLogic.update(PARTICLE_RADIUS);
    draw();
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 800, 0, &window, &renderer);

    emscripten_set_main_loop(loop, 0, 1);
}