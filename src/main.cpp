#include <emscripten.h>
#include "../include/Snowflake.h"
#include "../include/SDLWindow.h"

#define SLIDER_ID_SPAWN_RANGE 0
#define SLIDER_ID_RANDOM_WALK_RANGE 1
#define SLIDER_ID_SEGMENTS 2
#define SLIDER_ID_PARTICLES_PER_FRAME 3
#define SLIDER_ID_PARTICLE_RADIUS 4
#define SLIDER_ID_WINDOW_RESIZE 5

#define WINDOW_SIZE 400
#define WINDOW_SIZE_MIN 100
#define WINDOW_SIZE_MAX 1000

#define PARTICLES_PER_FRAME 2
#define PARTICLES_PER_FRAME_MIN 1
#define PARTICLES_PER_FRAME_MAX 50

#define PARTICLE_RADIUS 2
#define PARTICLE_RADIUS_MIN 2
#define PARTICLE_RADIUS_MAX 10

#define SEGMENTS 6
#define SEGMENTS_MIN 3
#define SEGMENTS_MAX 30

#define SPAWN_RANGE_MIN 0
#define SPAWN_RANGE_MAX 1000

#define RANDOM_WALK_RANGE_MIN -10
#define RANDOM_WALK_RANGE_MAX 10

SDLWindow window(WINDOW_SIZE, SEGMENTS);
Snowflake snowflakeLogic = Snowflake(WINDOW_SIZE, SEGMENTS, PARTICLE_RADIUS, PARTICLES_PER_FRAME);

void loop() {
//    SDL_Event event;
//    while (SDL_PollEvent(&event)) {
//        if (event.type == SDL_QUIT) {
//            emscripten_cancel_main_loop();
//            return;
//        }
//    }
    if (!snowflakeLogic.update()) {
        emscripten_pause_main_loop();
    }
    window.render(snowflakeLogic.getParticles());
}

void reset() {
    snowflakeLogic.reset();
    emscripten_resume_main_loop();
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void keyboardInput(int keycode) {
    if (keycode == 82) {
        reset();
    }
}
EMSCRIPTEN_KEEPALIVE
void sliderInput(int id, int min, int max) {
    if (min > max) return;

    if (id == SLIDER_ID_SPAWN_RANGE && min >= SPAWN_RANGE_MIN && max <= SPAWN_RANGE_MAX) {
        snowflakeLogic.setSpawnRange(min, max);
    } else if (id == SLIDER_ID_RANDOM_WALK_RANGE && min >= RANDOM_WALK_RANGE_MIN && max <= RANDOM_WALK_RANGE_MAX) {
        snowflakeLogic.setRandomWalkRange(min, max);
    } else if (id == SLIDER_ID_SEGMENTS && max >= SEGMENTS_MIN && max <= SEGMENTS_MAX) {
        snowflakeLogic.setSegments(max);
        window.setSegments(max);
    } else if (id == SLIDER_ID_PARTICLES_PER_FRAME && max >= PARTICLES_PER_FRAME_MIN && max <= PARTICLES_PER_FRAME_MAX) {
        snowflakeLogic.setParticlesPerFrame(max);
    } else if (id == SLIDER_ID_PARTICLE_RADIUS && max >= PARTICLE_RADIUS_MIN && max <= PARTICLE_RADIUS_MAX) {
        snowflakeLogic.setParticleRadius(max);
    } else if (id == SLIDER_ID_WINDOW_RESIZE && max >= WINDOW_SIZE_MIN && max <= WINDOW_SIZE_MAX) {
        window.setWindowSize(max);
        snowflakeLogic.setWindowSize(max);
        reset();
    }
}

EMSCRIPTEN_KEEPALIVE
int maxSpawnRange(){
    return snowflakeLogic.maxSpawnRange();
}

EMSCRIPTEN_KEEPALIVE
void setDrawBorderEnabled(bool enabled) {
    window.setDrawBorderEnabled(enabled);
}
}

int main() {
    emscripten_set_main_loop(loop, 0, 1);
}