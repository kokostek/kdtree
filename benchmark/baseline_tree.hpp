#include <array>
#include <numeric>
#include <vector>

#include <kdtree/point_traits.hpp>

namespace kdtree::benchmark {

    template<point Point>
    class baseline_tree {
    public:
        using point_type = Point;
        using distance_type = point_distance_t<Point>;

        template<points_range Range>
        baseline_tree(Range const& points, auto kdim)
            : points_(points)
            , kdim_(kdim) {}

        Point find_nearest(Point const& key) const {
            auto min_dist = std::numeric_limits<distance_type>::max();
            point_type result;
            for (const auto& p : points_) {
                const auto dist = dist_sqr(p, key);
                if (dist < min_dist) {
                    min_dist = dist;
                    result = p;
                }
            }
            return result;
        }

    private:
        std::vector<Point> points_;
        size_t kdim_;

        distance_type dist_sqr(const Point& a, const Point& b) const {

            auto dist = distance_type(0);

            for (auto i = 0; i < kdim_; ++i) {
                auto ai = a[i];
                auto bi = b[i];
                auto di = ai - bi;
                dist += di * di;
            }

            return dist;
        }
    };

    template<points_range Points>
    using points_range_baseline_tree_t = baseline_tree<points_range_point_t<Points>>;

    template<points_range Points>
    points_range_baseline_tree_t<Points> build_baseline_tree(const Points& points) {
        using point_t = points_range_point_t<Points>;
        return points_range_baseline_tree_t<Points>(points, point_kdim_v<point_t>);
    }
}