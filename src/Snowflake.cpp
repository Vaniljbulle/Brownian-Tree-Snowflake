#include "../include/Snowflake.h"

bool Snowflake::update() {
    if (finished) return false;

    for (int i = 0; i < particlesPerFrame; i++) {
        if (finished) return false;
        auto particle = SnowflakeParticle(width - particleRadius, randomSpawnRange(gen), particleRadius);
        moveParticle(particle);
        addParticle(particle);
    }


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
        particle.y += yRandomWalkRange(gen);

        int upperBoundaryY = centerY + slope * (centerX - particle.x);
        //int lowerBoundaryY = centerY - slope * (centerX - particle.x);
        if (particle.y < upperBoundaryY) {
            particle.y = upperBoundaryY + particleRadius;
        } else if (particle.y > centerY) {
            particle.y = centerY - particleRadius;
        }

//        else if (particle.y > lowerBoundaryY) {
//            particle.y = lowerBoundaryY - particleRadius;
//        }
    }

    if (isParticleAtEdge(particle)) {
        finished = true;
    }
}
