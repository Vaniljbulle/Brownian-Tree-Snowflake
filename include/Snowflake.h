#ifndef BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
#define BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H

#include "SnowflakeParticle.h"
#include <unordered_set>
#include <random>

class Snowflake {
    std::unordered_set<SnowflakeParticle> particles;
    int width, height, centerX, centerY, particlesPerFrame, particleRadius;
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
        return particle.x <= centerX;
    }

    bool isParticleAtEdge(const SnowflakeParticle &particle) {
        return particle.x >= width - particle.radius * 2;
    }

    void moveParticle(SnowflakeParticle &particle);

public:
    Snowflake(int width, int height, int segments, int particleRadius, int particlesPerFrame) : width(width), height(height), particleRadius(particleRadius), particlesPerFrame(particlesPerFrame) {
        centerX = width / 2;
        centerY = height / 2;
        setSegments(segments);
        setRandomWalkRange(-1, 1);
        setSpawnRange(0, 30);
    };

    bool update();

    void reset() {
        particles.clear();
        finished = false;
    }

    void setSpawnRange(int min, int max) {
        randomSpawnRange = std::uniform_int_distribution<>{centerY - max, centerY - min};
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

    std::unordered_set<SnowflakeParticle>& getParticles() { return particles; }
};


#endif //BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
