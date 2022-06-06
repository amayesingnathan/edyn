#ifndef EDYN_MATH_MATH_HPP
#define EDYN_MATH_MATH_HPP

#include "constants.hpp"
#include <algorithm>
#include <cmath>
#include <array>

namespace edyn {

/**
 * @return Value of `radians` converted to degrees.
 */
constexpr scalar to_degrees(scalar radians) noexcept {
    return radians / pi * 180;
}

/**
 * @return Value of `degress` converted to radians.
 */
constexpr scalar to_radians(scalar degrees) noexcept {
    return degrees / 180 * pi;
}

/**
 * Converts torque units in Nm/degree to Nm/rad.
 * @param Nm_per_degree Nm/degree.
 * @return Torque in Nm/rad.
 */
constexpr scalar to_Nm_per_radian(scalar Nm_per_degree) noexcept {
    return Nm_per_degree * to_degrees(1);
}

/**
 * Converts torque units in Nm/rad to Nm/degree.
 * @param Nm_per_radian Nm/rad.
 * @return Torque in Nm/degree.
 */
constexpr scalar to_Nm_per_degree(scalar Nm_per_radian) noexcept {
    return Nm_per_radian / to_degrees(1);
}

/**
 * @return Scalar clamped to the [0, 1] interval.
 */
constexpr scalar clamp_unit(scalar s) noexcept {
    return std::clamp(s, scalar(0), scalar(1));
}

/**
 * @return Angle in [-π, π].
 */
inline scalar normalize_angle(scalar s) noexcept {
    s = std::fmod(s, pi2);

    if (s < -pi) {
        return s + pi2;
    } else if (s > pi) {
        return s - pi2;
    }

    return s;
}

/**
 * @return Linear interpolation between `a` and `b` by scalar `s`.
 */
template<typename T, typename Scalar>
constexpr auto lerp(T a, T b, Scalar s) noexcept {
    return a * (Scalar(1) - s) + b * s;
}

/**
 * @return The square of a number.
 */
template<typename T>
constexpr auto square(T a) noexcept {
    return a * a;
}

/**
 * @return 1 if `b` is true, -1 if `b is false.
 */
constexpr auto to_sign(bool b) noexcept {
    return b ? scalar(1) : scalar(-1);
}

/**
 * @brief Calculate average of N values, i.e. their sum divided by N.
 * @param array Array of values.
 * @return Average value.
 */
template<typename T, size_t N>
constexpr auto average(const std::array<T, N> &array) noexcept {
    auto sum = array[0];
    for (size_t i = 1; i < N; ++i) {
        sum += array[i];
    }
    return sum / N;
}

template<size_t L>
inline constexpr bool any(std::array<bool, L> const& v)
{
    bool Result = false;
    for (size_t i = 0; i < L; ++i)
        Result = Result || v[i];
    return Result;
}

template<size_t L>
inline constexpr bool all(std::array<bool, L> const& v)
{
    bool Result = true;
    for (size_t i = 0; i < L; ++i)
        Result = Result && v[i];
    return Result;
}

inline constexpr std::array<bool, 3> lessThanEqual(vector3 const& x, vector3 const& y)
{
    std::array<bool, 3> Result{true};
    for (size_t i = 0; i < 3; ++i)
        Result[i] = x[i] <= y[i];
    return Result;
}

inline constexpr std::array<bool, 3> equal(vector3 const& x, vector3 const& y, vector3 const& Epsilon)
{
    return lessThanEqual(abs(x - y), Epsilon);
}
inline constexpr std::array<bool, 3> equal(vector3 const& x, vector3 const& y, scalar Epsilon)
{
    return equal(x, y, vector3{ Epsilon });
}

// clamp
template<typename genType>
inline constexpr genType clamp(genType x, genType minVal, genType maxVal)
{
    static_assert(std::numeric_limits<genType>::is_iec559 || std::numeric_limits<genType>::is_integer, "'clamp' only accept floating-point or integer inputs");
    return std::min(std::max(x, minVal), maxVal);
}

}

#endif // EDYN_MATH_MATH_HPP
