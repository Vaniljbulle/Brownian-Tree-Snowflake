#include "../include/Snowflake.h"

void Snowflake::update(int radius) {
    if (finished) return;

    auto particle = SnowflakeParticle(width - radius, centerY, radius);
    moveParticle(particle);
    addParticle(particle);
}

void Snowflake::addParticle(const SnowflakeParticle &particle) {
    particles.insert(particle);
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

bool Snowflake::isParticleAtCenter(const SnowflakeParticle &particle) const {
    return particle.x <= centerX;
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


bool Snowflake::isParticleAtEdge(const SnowflakeParticle &particle) {
    return particle.x >= width - particle.radius;
}
