#ifndef BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
#define BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H

#include "SnowflakeParticle.h"
#include <unordered_set>
#include <random>

class Snowflake {
    std::unordered_set<SnowflakeParticle> particles;
    int windowSize, windowHalfSize, particlesPerFrame, particleRadius, spawnMin = 0, spawnMax = 30;
    double slope;
    bool finished = false;

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> yRandomWalkRange{};
    std::uniform_int_distribution<> randomSpawnRange{};

    void addParticle(const SnowflakeParticle &particle) {
        particles.insert(particle);
    }

    bool isParticleColliding(const SnowflakeParticle &particle);

    bool isParticleAtCenter(const SnowflakeParticle &particle) const {
        return particle.x <= windowHalfSize;
    }

    bool isParticleAtEdge(const SnowflakeParticle &particle) {
        return particle.x >= windowSize - particle.radius * 2;
    }

    void moveParticle(SnowflakeParticle &particle);

public:
    Snowflake(int windowSize, int segments, int particleRadius, int particlesPerFrame) : particleRadius(particleRadius), particlesPerFrame(particlesPerFrame) {
        setWindowSize(windowSize);
        setSegments(segments);
        setRandomWalkRange(-1, 1);
        setSpawnRange(spawnMin, spawnMax);
    };

    bool update();

    void reset() {
        particles.clear();
        finished = false;
    }

    void setSpawnRange(int min, int max) {
        spawnMin = min;
        spawnMax = max;
        randomSpawnRange = std::uniform_int_distribution<>{windowHalfSize - max, windowHalfSize - min};
    }

    void setRandomWalkRange(int min, int max) {
        yRandomWalkRange = std::uniform_int_distribution<>{min, max};
    }

    void setParticlesPerFrame(int _particlesPerFrame) {
        particlesPerFrame = _particlesPerFrame;
    }

    void setParticleRadius(int _particleRadius) {
        particleRadius = _particleRadius;
    }

    void setSegments(int segments) {
        slope = tan(M_PI / segments);
    }

    float maxSpawnRange() {
        return (slope * windowHalfSize);
    }

    void setWindowSize(int _windowSize) {
        windowSize = _windowSize;
        windowHalfSize = windowSize / 2;
        setSpawnRange(spawnMin, spawnMax);
    }

    std::unordered_set<SnowflakeParticle>& getParticles() { return particles; }
};


#endif //BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
