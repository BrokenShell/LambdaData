#pragma once
#include <algorithm>
#include <cmath>
#include <random>


namespace Storm {
    using Integer = long long;

    namespace Engine {
        using MT_Engine = std::mt19937_64;
        using DB_Engine = std::discard_block_engine<MT_Engine, 64, 42>;
        using RNG_Engine = std::shuffle_order_engine<DB_Engine, 256>;
        static RNG_Engine Hurricane { std::random_device()() };
    }

    template <typename Callable>
    auto approximation_clamp(Callable && approximation, Storm::Integer target, Storm::Integer upper_bound) -> Storm::Integer {
        if (target >= 0 and target < upper_bound) return target;
        return approximation(upper_bound);
    }

    auto canonical_variate() -> double {
        return std::generate_canonical<double, std::numeric_limits<double>::digits>(Engine::Hurricane);
    }

    auto uniform_int_variate(Storm::Integer a, Storm::Integer b) -> Storm::Integer {
        std::uniform_int_distribution<Storm::Integer> distribution { std::min(a, b), std::max(b, a) };
        return distribution(Engine::Hurricane);
    }

    auto random_below(Storm::Integer number) -> Storm::Integer {
        return Storm::uniform_int_variate(0, std::nextafter(number, 0));
    }

    auto random_range(Storm::Integer start, Storm::Integer stop, Storm::Integer step) -> Storm::Integer {
        if (start == stop or step == 0) return start;
        const auto width { std::abs(start - stop) - 1 };
        const auto pivot { step > 0 ? std::min(start, stop) : std::max(start, stop) };
        const auto step_size { std::abs(step) };
        return pivot + step_size * Storm::random_below((width + step_size) / step);
    }

    auto triangular_variate(double low, double high, double mode) -> double {
        if (low == high) return low;
        const double rand { Storm::canonical_variate() };
        const double mode_factor { (mode - low) / (high - low) };
        if (rand > mode_factor) return high + (low - high) * std::sqrt((1.0 - rand) * (1.0 - mode_factor));
        return low + (high - low) * std::sqrt(rand * mode_factor);
    }

    auto front_linear(Storm::Integer number) -> Storm::Integer {
        return Storm::triangular_variate(0, number, 0);
    }

    auto poisson_variate(double mean) -> Storm::Integer {
        std::poisson_distribution<Storm::Integer> distribution { mean };
        return distribution(Engine::Hurricane);
    }

    auto front_poisson(Storm::Integer number) -> Storm::Integer {
        const auto result { Storm::poisson_variate(number / 4.0) };
        return Storm::approximation_clamp(Storm::front_linear, result, number);
    }

} // end Storm namespace
