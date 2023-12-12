#include "../include/SDLWindow.h"

SDLWindow::SDLWindow(int windowSize, int segments) : windowSize(windowSize), segments(segments) {
    windowHalfSize = windowSize / 2;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(windowSize, windowSize, 0, &window, &renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowSize,
                                       windowSize);
}

void SDLWindow::drawFilledCircle(int centerX, int centerY, int radius) {
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

void SDLWindow::render(std::unordered_set<SnowflakeParticle>& particles) {
    SDL_SetRenderTarget(renderer, screenTexture);

    clear();
    if (drawBorderEnabled) drawBorder();

    for (int i = 0; i < segments; i++) {
        int angle = (360 / segments) * i;
        for (const auto &particle : particles) {
            int x = particle.x, y = particle.y;
            rotatePoint(windowHalfSize, windowHalfSize, angle, x, y);
            drawFilledCircle(x, y, particle.radius);
            drawFilledCircle(x, windowSize - y, particle.radius);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLWindow::render() {
    SDL_SetRenderTarget(renderer, screenTexture);

    if (drawBorderEnabled) drawBorder();

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLWindow::drawBorder() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_RenderDrawLine(renderer, 0, 0, windowSize, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, windowSize);
    SDL_RenderDrawLine(renderer, windowSize - 1, 0, windowSize - 1, windowSize);
    SDL_RenderDrawLine(renderer, 0, windowSize - 1, windowSize, windowSize - 1);
}

void SDLWindow::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SDLWindow::rotatePoint(int cx, int cy, int angle, int &x, int &y) {
    float rad = angle * oneDegree;
    float s = sin(rad);
    float c = cos(rad);

    x -= cx;
    y -= cy;

    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    x = xnew + cx;
    y = ynew + cy;
}

void SDLWindow::setWindowSize(int _windowSize) {
    windowSize = _windowSize;
    windowHalfSize = windowSize / 2;
    SDL_SetWindowSize(window, _windowSize, _windowSize);
    SDL_DestroyTexture(screenTexture);
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowSize,
                                      windowSize);
    render();
}