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
#define SEGMENTS 12

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Snowflake snowflakeLogic = Snowflake(WINDOW_WIDTH, WINDOW_HEIGHT, SEGMENTS);

void drawFilledCircle(int centerX, int centerY, int radius) {
    int outerRadius = radius + 1;
    int radiusSquared = radius * radius;
    int outerRadiusSquared = outerRadius * outerRadius;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
    for (int w = -outerRadius; w <= outerRadius; w++)
        for (int h = -outerRadius; h <= outerRadius; h++)
            if (w * w + h * h >= radiusSquared && w * w + h * h < outerRadiusSquared)
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = -radius; w <= radius; w++)
        for (int h = -radius; h <= radius; h++)
            if (w * w + h * h <= radiusSquared)
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
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
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH,
                                                   WINDOW_HEIGHT);
    SDL_SetRenderTarget(renderer, screenTexture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < SEGMENTS; i++) {
        int angle = (360 / SEGMENTS) * i;
        for (const auto &particle : snowflakeLogic.getParticles()) {
            int x = particle.x, y = particle.y;
            rotatePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, angle, x, y);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//            SDL_RenderDrawPoint(renderer, x, y);
//            SDL_RenderDrawPoint(renderer, x, WINDOW_HEIGHT - y);
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

    if (!snowflakeLogic.update(PARTICLE_RADIUS)) {
        emscripten_pause_main_loop();
    }
    draw();
}


extern "C" {
EMSCRIPTEN_KEEPALIVE
void keyboardInput(int keycode) {
    if (keycode == 82) {
        snowflakeLogic.reset();
        emscripten_resume_main_loop();
    }
}
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    emscripten_set_main_loop(loop, 0, 1);
}