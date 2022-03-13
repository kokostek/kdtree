#pragma once
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <ranges>

namespace kdtree {

    template<typename>
    struct point_kdim;

    template<typename T, auto N>
    struct point_kdim<std::array<T, N>> {
        static constexpr auto value = N;
    };

    template<typename T>
    constexpr auto point_kdim_v = point_kdim<T>::value;

    template<typename>
    struct point_distance;

    template<std::ranges::range T>
    struct point_distance<T> {
        using type = std::ranges::range_value_t<T>;
    };

    template<typename T>
    using point_distance_t = point_distance<T>::type;

    template<class Point>
    struct point_traits {
        static constexpr std::string format(const Point& p) {
            std::ostringstream os;
            os << p;
            return os.str();
        }
    };

    template<class Point>
    auto format_point(const Point& p) {
        return point_traits<Point>::format(p);
    }

    template<std::ranges::range Point>
    struct point_traits<Point> {
        static constexpr std::string format(const Point& p) {
            std::ostringstream os;
            os << "[";
            
            auto begin = std::ranges::begin(p);
            auto end = std::ranges::end(p);

            if (begin != end) {
                os << *begin;
                auto s = std::ranges::subrange(++begin, end);
                for (const auto& t : s) {
                    os << ", "  << t;
                }
            }

            os << "]";
            return os.str();
        }
    };

    template<class T, class V>
    concept indexable = requires(T t, size_t i) {
        { t[i] } -> std::convertible_to<V>;
    };

    template<class Point>
    concept point =
        indexable<Point, point_distance_t<Point>>;

    template<class Range>
    concept points_range =
        std::ranges::sized_range<Range> &&
        point<std::ranges::range_value_t<Range>>;

    template<points_range Points>
    using points_range_point_t = std::ranges::range_value_t<Points>;
    
    static_assert(std::same_as<point_distance_t<std::vector<int>>, int>);
    static_assert(std::same_as<point_distance_t<std::vector<float>>, float>);
    static_assert(std::same_as<point_distance_t<std::vector<double>>, double>);
}