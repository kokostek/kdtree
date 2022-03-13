#pragma once

#include "node.hpp"
#include "point_traits.hpp"

#include <algorithm>
#include <ostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <type_traits>

namespace kdtree::detail {

    template<class Range>
    concept index_range =
        std::ranges::sized_range<Range> &&
        std::convertible_to<std::ranges::range_value_t<Range>, size_t>;

    template<points_range Points, index_range Index>
    static void argsort(Points& points, Index& indices, const auto axis) {

        const auto length = std::ranges::size(indices);

        if (length <= 1) return;

        auto begin = std::ranges::begin(indices);
        auto mid = begin + length / 2;
        auto end = std::ranges::end(indices);

        std::nth_element(begin, mid, end,
            [&points, &axis](auto a, auto b) { return points[a][axis] < points[b][axis]; });
    }

    template<points_range Points, index_range Index>
    auto build_r(Points& points, Index& indices, const auto& kdim, const auto& axis) {

        using point_t = points_range_point_t<Points>;

        const auto indices_length = std::ranges::size(indices);

        if (indices_length == 0) {
            return make_leaf<point_t>();
        }

        argsort(points, indices, axis);

        const auto indices_begin = std::ranges::begin(indices);
        const auto indices_mid = indices_begin + indices_length / 2;
        const auto indices_end = std::ranges::end(indices);

        const auto indices_begin_l = indices_begin;
        const auto indices_end_l = indices_mid;
        const auto indices_l = std::ranges::subrange(indices_begin_l, indices_end_l);
        const auto length_l = std::ranges::size(indices_l);

        const auto indices_begin_r = indices_mid + 1;
        const auto indices_end_r = indices_end;
        const auto indices_r = std::ranges::subrange(indices_begin_r, indices_end_r);
        const auto length_r = std::ranges::size(indices_r);

        const auto next_axis = (axis + 1) % kdim;

        typename node<point_t>::container_type left;
        if (length_l > 0 && kdim > 0) {
            left = build_r(points, indices_l, kdim, next_axis);
        }
        else {
            left = make_leaf<point_t>();
        }

        typename node<point_t>::container_type right;
        if (length_r > 0 && kdim > 0) {
            right = build_r(points, indices_r, kdim, next_axis);
        }
        else {
            right = make_leaf<point_t>();
        }

        return make_node<point_t>(points[*indices_mid], std::move(left), std::move(right));
    }

    template<points_range Points>
    auto build(const Points& points, int kdim) {
        std::vector<size_t> indices(std::ranges::size(points));
        std::iota(indices.begin(), indices.end(), 0);
        auto root = build_r(points, indices, kdim, 0);
        return root;
    }

    template<point Point>
    using find_result_t = std::pair<node<Point> const*, point_distance_t<Point>>;

    template<point Point>
    static find_result_t<Point> min(
        const find_result_t<Point>& a,
        const find_result_t<Point>& b) {
        return a.second < b.second ? a : b;
    }

    template<point Point>
    auto dist_sqr(const Point& a, const Point& b, const auto kdim) {

        auto dist{ point_distance_t<Point>(0) };

        for (auto i = 0; i < kdim; ++i) {
            auto ai = a[i];
            auto bi = b[i];
            auto di = ai - bi;
            dist += di * di;
        }

        return dist;
    }

    template<point Point>
    find_result_t<Point> find_nearest_r(
        const node<Point>* root,
        const Point& key,
        const auto& axis,
        const auto& kdim,
        const find_result_t<Point>& best) {

        const auto dist = dist_sqr(key, root->value(), kdim);

        const auto& root_at_axis = root->value()[axis];
        const auto& key_at_axis = key[axis];
        const auto delta = root_at_axis - key_at_axis;
        const auto delta2 = delta * delta;

        auto best_upd = min(best, std::make_pair(root, dist));

        auto [selected, other] = delta > 0
            ? std::make_pair(root->left().get(), root->right().get())
            : std::make_pair(root->right().get(), root->left().get());

        const auto next_axis = (axis + 1) % kdim;

        auto further_1 = selected
            ? min(best_upd, find_nearest_r(selected, key, next_axis, kdim, best_upd))
            : best_upd;

        auto further_2 = other && delta2 < further_1.second
            ? min(further_1, find_nearest_r(other, key, next_axis, kdim, further_1))
            : further_1;

        return further_2;
    }

    template<point Point>
    Point find_nearest(
        const node_container_t<Point>& root,
        const Point& key,
        const auto& kdim) {

        if (!root) {
            return Point{};
        }

        using distance_t = point_distance_t<Point>;
        
        const find_result_t<Point> worst{ nullptr, std::numeric_limits<distance_t>::max() };
        
        return find_nearest_r(root.get(), key, 0, kdim, worst).first->value();
    }

    template<point Point>
    using find_result_vector_t = std::vector<find_result_t<Point>>;

    template<point Point>
    static void append_result(
        find_result_vector_t<Point>& results,
        const find_result_t<Point>& value,
        const auto& num) {

        if (num == 0) return;

        using distance_t = point_distance_t<Point>;

        const auto u = std::ranges::upper_bound(
            results, value.second, std::less(), &find_result_t<Point>::second);

        if (results.size() < num) {
            results.insert(u, value);
        }
        else if (u != results.end()) {
            results.insert(u, value);
            results.pop_back();
        }
    }

    template<point Point>
    void find_nearest_n_r(
        const node<Point>* root,
        const Point& key,
        const auto& axis,
        const auto& kdim,
        const auto& num,
        find_result_vector_t<Point>& results) {

        const auto dist = dist_sqr(key, root->value(), kdim);

        const auto& root_at_axis = root->value()[axis];
        const auto& key_at_axis = key[axis];
        const auto delta = root_at_axis - key_at_axis;
        const auto delta2 = delta * delta;

        append_result(results, std::make_pair(root, dist), num);

        auto [selected, other] = delta > 0
            ? std::make_pair(root->left().get(), root->right().get())
            : std::make_pair(root->right().get(), root->left().get());

        const auto next_axis = (axis + 1) % kdim;

        if (selected) {
            find_nearest_n_r(selected, key, next_axis, kdim, num, results);
        }

        if (other && (results.size() < num || delta2 < results[results.size() - 1].second)) {
            find_nearest_n_r(other, key, next_axis, kdim, num, results);
        }
    }
        
    template<point Point>
    std::vector<Point> find_nearest_n(
        const node_container_t<Point>& root,
        const Point& key,
        const auto& kdim,
        const auto& num) {

        if (!root || num == 0) return std::vector<Point>{};
        
        find_result_vector_t<Point> results;
        find_nearest_n_r(root.get(), key, 0, kdim, num, results);

        std::vector<Point> points;
        std::ranges::transform(
            results, std::back_inserter(points),
            [](const auto& res) { return res.first->value(); });

        return points;
    }
}