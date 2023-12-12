#ifndef BROWNIAN_TREE_SNOWFLAKE_SDLWINDOW_H
#define BROWNIAN_TREE_SNOWFLAKE_SDLWINDOW_H

#include <SDL2/SDL.h>
#include <unordered_set>
#include "SnowflakeParticle.h"

class SDLWindow {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *screenTexture = nullptr;

    int windowSize, windowHalfSize, segments;
    const float oneDegree = M_PI / 180.0f;
    bool drawBorderEnabled = false;

    void drawFilledCircle(int centerX, int centerY, int radius);

    void clear();

    void rotatePoint(int cx, int cy, int angle, int &x, int &y);

    void drawBorder();

public:
    SDLWindow(int windowSize, int segments);

    ~SDLWindow() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(screenTexture);
    }

    void render(std::unordered_set<SnowflakeParticle>& particles);

    void render();

    void setSegments(int _segments) {
        segments = _segments;
    }

    void setWindowSize(int _windowSize);

    void setDrawBorderEnabled(bool enabled) {
        drawBorderEnabled = enabled;
        render();
    }
};


#endif //BROWNIAN_TREE_SNOWFLAKE_SDLWINDOW_H
