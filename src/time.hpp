#pragma once

#include <cstdint>

struct Time
{
    static constexpr std::uint64_t secsPerTick = 60;
    static constexpr std::uint64_t secsPerYear = 31'556'952;
    std::uint64_t ticks = 0;

    // --- Helper methods ---

    explicit operator std::uint64_t() const
    {
        return ticks;
    }
    [[nodiscard]] std::uint64_t seconds() const
    {
        return ticks * secsPerTick;
    }
    [[nodiscard]] std::uint64_t year() const
    {
        return seconds() / secsPerYear;
    }

    Time& operator++()
    {
        ticks++;
        return *this;
    } // Prefix
    Time& operator--()
    {
        ticks--;
        return *this;
    } // Prefix
    Time operator++(int)
    {
        ticks++;
        return Time{ticks + 1};
    } // Postfix
    Time operator--(int)
    {
        ticks--;
        return Time{ticks - 1};
    } // Postfix

    Time operator+(Time other) const
    {
        return Time{ticks + other.ticks};
    }
    Time operator-(Time other) const
    {
        return Time{ticks - other.ticks};
    }
    Time& operator+=(Time other)
    {
        ticks += other.ticks;
        return *this;
    }
    Time& operator-=(Time other)
    {
        ticks -= other.ticks;
        return *this;
    }

    bool operator>(const Time& other) const
    {
        return ticks > other.ticks;
    }
    bool operator<(const Time& other) const
    {
        return ticks < other.ticks;
    }
    bool operator>=(const Time& other) const
    {
        return ticks >= other.ticks;
    }
    bool operator<=(const Time& other) const
    {
        return ticks <= other.ticks;
    }
    bool operator==(const Time& other) const
    {
        return ticks == other.ticks;
    }
    bool operator!=(const Time& other) const
    {
        return ticks != other.ticks;
    }
};