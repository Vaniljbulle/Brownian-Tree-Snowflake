#ifndef BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
#define BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H

#include "SnowflakeParticle.h"
#include <unordered_set>
#include <random>

class Snowflake {
    std::unordered_set<SnowflakeParticle> particles;
    int width, height, centerX, centerY;
    double slope;
    bool finished = false;

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dis{};

    void addParticle(const SnowflakeParticle &particle);

    bool isParticleColliding(const SnowflakeParticle &particle);

    bool isParticleAtCenter(const SnowflakeParticle &particle) const;

    bool isParticleAtEdge(const SnowflakeParticle &particle);

    void moveParticle(SnowflakeParticle &particle);

public:
    Snowflake(int width, int height, int segments) : width(width), height(height) {
        centerX = width / 2;
        centerY = height / 2;
        slope = tan(M_PI/segments);

        dis = std::uniform_int_distribution<>{-5, 5};
    };

    void update(int radius);

    std::unordered_set<SnowflakeParticle>& getParticles() { return particles; }
};


#endif //BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKE_H
