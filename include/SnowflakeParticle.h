#ifndef BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKPARTICLE_H
#define BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKPARTICLE_H
#include <functional>

struct SnowflakeParticle {
    int x, y, radius;

    SnowflakeParticle(int x, int y, int radius) : x(x), y(y), radius(radius) {}

    bool operator==(const SnowflakeParticle &other) const {
        return x == other.x && y == other.y && radius == other.radius;
    }
};

namespace std {
    template <>
    struct hash<SnowflakeParticle> {
        size_t operator()(const SnowflakeParticle& particle) const {
            return ((hash<int>()(particle.x)
                     ^ (hash<int>()(particle.y) << 1)) >> 1)
                   ^ (hash<int>()(particle.radius) << 1);
        }
    };
}

#endif //BROWNIAN_TREE_SNOWFLAKE_SNOWFLAKPARTICLE_H
