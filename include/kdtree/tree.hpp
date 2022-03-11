#pragma once

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

			std::vector<size_t> indices(std::ranges::size(points));
			std::iota(indices.begin(), indices.end(), 0);

			auto root = build_root(kdim, points, indices, 0);

			return tree(std::move(root), kdim);
		}

		const typename node<Point>::container_type& root() const {
			return root_;
		}

		void format(std::ostream& os) const {
			root_->format(os);
		}

		Point const& find_nearest(const Point& key) const {
			return find_nearest(*root_, key).value();
		}

	private:
		typename node<Point>::container_type root_;
		int kdim_;

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
		static typename node<Point>::container_type build_root(
			const auto kdim, Points& points, Index& indices, const auto axis) {

			const auto indices_length = std::ranges::size(indices);

			if (indices_length == 0) {
				return make_leaf<Point>();
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

			const size_t next_axis = (axis + 1) % kdim;

			typename node<Point>::container_type left;
			if (length_l > 0 && kdim > 0) {
				left = build_root(kdim, points, indices_l, next_axis);
			}
			else {
				left = make_leaf<Point>();
			}

			typename node<Point>::container_type right;
			if (length_r > 0 && kdim > 0) {
				right = build_root(kdim, points, indices_r, next_axis);
			}
			else {
				right = make_leaf<Point>();
			}

			return make_node<Point>(points[*indices_mid], std::move(left), std::move(right));
		}

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

		node<Point> const& find_nearest(
			const node<Point>& root,
			const Point& key) const {
			const auto root_dist = dist_sqr(root.value(), key);
			return *find_nearest(&root, key, 0, std::make_pair(&root, root_dist)).first;
		}

		static std::pair<node<Point> const*, distance_type> min(
			std::pair<node<Point> const*, distance_type> a,
			std::pair<node<Point> const*, distance_type> b) {
			return a.second < b.second ? a : b;
		}

		std::pair<node<Point> const*, distance_type> find_nearest(
			const node<Point>* root,
			const Point& key,
			const int axis, 
			std::pair<node<Point> const*, distance_type> best) const {

			const auto dist = dist_sqr(key, root->value());

			const auto& root_at_axis = root->value()[axis];
			const auto& key_at_axis = key[axis];
			const auto delta = root_at_axis - key_at_axis;
			const auto delta2 = delta * delta;

			auto best_upd = min(best, std::make_pair(root, dist));

			auto [selected, other] = delta > 0
				? std::make_pair(root->left().get(), root->right().get())
				: std::make_pair(root->right().get(), root->left().get());

			const auto next_axis = (axis + 1) % kdim_;

			auto further_1 = selected
				? min(best_upd, find_nearest(selected, key, next_axis, best_upd))
				: best_upd;

			auto further_2 = other && delta2 < further_1.second
				? min(further_1, find_nearest(other, key, next_axis, further_1))
				: further_1;

			return further_2;
		}
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
