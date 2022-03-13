#pragma once

#include "tree_detail.hpp"
#include "node.hpp"
#include "point_traits.hpp"

#include <algorithm>
#include <ostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <type_traits>

namespace kdtree {

    template<class Range>
    concept index_range =
        std::ranges::sized_range<Range> &&
        std::convertible_to<std::ranges::range_value_t<Range>, size_t>;

    template<point Point>
    class tree {
    public:

        using point_type = Point;
        using distance_type = point_distance_t<Point>;

        tree()
            : root_(make_leaf<Point>())
            , kdim_(0) {}

        tree(const int kdim)
            : root_(make_leaf<Point>())
            , kdim_(kdim) {}

        tree(node<Point>::container_type root, int kdim)
            : root_(std::move(root))
            , kdim_(kdim) {}

        tree(const tree& other) = delete;

        tree(tree&& other) {
            std::swap(root_, other.root_);
        }

        tree& operator=(tree&& other) {
            std::swap(root_, other.root_);
            std::swap(kdim_, other.kdim_);
            return *this;
        }

        template<points_range Points>
        requires std::same_as<points_range_point_t<Points>, Point>
        static tree build(const Points& points, int kdim) {
            auto root = detail::build(points, kdim);
            return tree(std::move(root), kdim);
        }

        const typename node<Point>::container_type& root() const {
            return root_;
        }

        void format(std::ostream& os) const {
            root_->format(os);
        }

        Point find_nearest(const Point& key) const {
            return detail::find_nearest(root_, key, kdim_);
        }

        std::vector<Point> find_nearest_n(const Point& key, const auto& num) const {
            return detail::find_nearest_n(root_, key, kdim_, num);
        }

    private:
        typename node<Point>::container_type root_;
        int kdim_;
    };

    template<point Point>
    std::ostream& operator<<(
        std::ostream& os, tree<Point> const& tree) {
        tree.format(os);
        return os;
    }

    template<class Point>
    bool operator==(
        const tree<Point>& a,
        const tree<Point>& b) {
        if (bool(a.root()) != bool(b.root())) return false;
        if (a.root() && *a.root() != *b.root()) return false;
        return true;
    }

    template<point Point, class... Args>
    tree<Point> make_tree(Args&&... args) {
        return tree<Point>(std::forward<Args>(args)..., point_kdim_v<Point>);
    }

    template<points_range Points>
    using points_range_tree_t = tree<points_range_point_t<Points>>;

    template<points_range Points>
    points_range_tree_t<Points> build_tree(Points& points) {
        using point_t = points_range_point_t<Points>;
        return points_range_tree_t<Points>::build(points, point_kdim_v<point_t>);
    }

    template<point Point>
    tree<Point> build_tree(std::initializer_list<Point>&& points) {
        std::vector points_vec(std::forward<std::initializer_list<Point>>(points));
        return tree<Point>::build(points_vec, point_kdim_v<Point>);
    }
}
