#pragma once
#include <algorithm>
#include <cmath>
#include <random>

// This is an excerpt from the Storm library: https://github.com/BrokenShell/Storm


namespace Storm {

    using Integer = long long;

    namespace Engine {
        // Core Random Engine
        using MT_Engine = std::mt19937_64;
        using DB_Engine = std::discard_block_engine<MT_Engine, 64, 42>;
        using RNG_Engine = std::shuffle_order_engine<DB_Engine, 256>;
        static RNG_Engine Hurricane { std::random_device()() };
    }

    template <typename Callable>
    auto approximation_clamp(Callable && approximation, Storm::Integer target, Storm::Integer upper_bound) -> Storm::Integer {
        // Utility for approximating a distribution when the target is outside that distribution.
        if (target >= 0 and target < upper_bound) return target;
        return approximation(upper_bound);
    }

    auto canonical_variate() -> double {
        // Flat uniform distribution of floats between [0.0, 1.0) exclusive.
        return std::generate_canonical<double, std::numeric_limits<double>::digits>(Engine::Hurricane);
    }

    auto uniform_int_variate(Storm::Integer a, Storm::Integer b) -> Storm::Integer {
        // Flat uniform distribution in range [a, b] inclusive
        std::uniform_int_distribution<Storm::Integer> distribution { std::min(a, b), std::max(b, a) };
        return distribution(Engine::Hurricane);
    }

    auto random_below(Storm::Integer number) -> Storm::Integer {
        // Flat uniform distribution in range [0, number) exclusive
        return Storm::uniform_int_variate(0, std::nextafter(number, 0));
    }

    auto random_range(Storm::Integer start, Storm::Integer stop, Storm::Integer step) -> Storm::Integer {
        // Flat uniform distribution in range [start, stop) by step increments
        if (start == stop or step == 0) return start;
        const auto width { std::abs(start - stop) - 1 };
        const auto pivot { step > 0 ? std::min(start, stop) : std::max(start, stop) };
        const auto step_size { std::abs(step) };
        return pivot + step_size * Storm::random_below((width + step_size) / step);
    }

    auto triangular_variate(double low, double high, double mode) -> double {
        // Linear distribution in range [low, high] with peak at mode
        if (low == high) return low;
        const double rand { Storm::canonical_variate() };
        const double mode_factor { (mode - low) / (high - low) };
        if (rand > mode_factor) return high + (low - high) * std::sqrt((1.0 - rand) * (1.0 - mode_factor));
        return low + (high - low) * std::sqrt(rand * mode_factor);
    }

    auto front_linear(Storm::Integer number) -> Storm::Integer {
        // Linear distribution in range [0, number) with peak at 0
        return Storm::triangular_variate(0, number, 0);
    }

    auto poisson_variate(double mean) -> Storm::Integer {
        // Standard Poisson distribution
        std::poisson_distribution<Storm::Integer> distribution { mean };
        return distribution(Engine::Hurricane);
    }

    auto front_poisson(Storm::Integer number) -> Storm::Integer {
        // Custom Poisson distribution in range [0, number)
        const auto result { Storm::poisson_variate(number / 4.0) };
        return Storm::approximation_clamp(Storm::front_linear, result, number);
    }

} // end Storm namespace
