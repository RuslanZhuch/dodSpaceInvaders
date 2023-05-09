#pragma once

#include <random>

namespace Game::Utils
{

    class RandomGenerator
    {
    public:
        [[nodiscard]] int32_t generate(int32_t minimun, int32_t maximum) noexcept
        {
            std::uniform_int_distribution<> distrib(minimun, maximum);
            return distrib(this->generator);
        }
    private:
        std::mt19937 generator;
    };

}
