#include "../include/Snowflake.h"

bool Snowflake::update(int radius) {
    if (finished) return false;

    auto particle = SnowflakeParticle(width - radius, centerY, radius);
    moveParticle(particle);
    addParticle(particle);
    return true;
}

bool Snowflake::isParticleColliding(const SnowflakeParticle &particle) {
    for (const auto &existingParticle: particles) {
        // euclidean distance
        double distance = std::sqrt(std::pow(particle.x - existingParticle.x, 2) +
                                    std::pow(particle.y - existingParticle.y, 2));

        if (distance < (particle.radius + existingParticle.radius)) {
            return true;
        }
    }
    return false;
}

void Snowflake::moveParticle(SnowflakeParticle &particle) {
    while (!isParticleColliding(particle) && !isParticleAtCenter(particle)) {
        particle.x -= 1;
        particle.y += dis(gen);

        int upperBoundaryY = centerY + slope * (centerX - particle.x);
        if (particle.y < upperBoundaryY) {
            particle.y = upperBoundaryY;
        } else if (particle.y > centerY) {
            particle.y = centerY;
        }
    }

    if (isParticleAtEdge(particle)) {
        finished = true;
    }
}
