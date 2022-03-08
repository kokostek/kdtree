#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <utility>

#include "node.hpp"
#include "point_traits.hpp"

namespace kdtree {
	template<class Point, class Distance>
	class tree {
	public:

		using point_type = Point;
		using distance_type = Distance;

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

		static tree build(const std::vector<Point>& points, int kdim) {

			std::vector<size_t> idx(points.size());
			std::iota(idx.begin(), idx.end(), 0);

			auto root = build_root(
				kdim, points, idx.begin(), idx.end(), idx.size(), 0);

			return tree(std::move(root), kdim);
		}

		const typename node<Point>::container_type& root() const {
			return root_;
		}

		void format(std::ostream& os) const {
			root_->format(os);
		}

		node<Point> const& find_nearest(const Point& key) const {
			return find_nearest(*root_, key);
		}

	private:
		typename node<Point>::container_type root_;
		int kdim_;

		static void argsort(
			const std::vector<Point>& points,
			const std::vector<size_t>::iterator& idx_begin,
			const std::vector<size_t>::iterator& idx_end,
			const auto length,
			const auto axis) {

			auto mid = idx_begin + length / 2;
			std::nth_element(idx_begin, mid, idx_end,
				[&points, &axis](auto a, auto b) { return points[a][axis] < points[b][axis]; });
		}

		static typename node<Point>::container_type build_root(
			const auto kdim,
			const std::vector<Point>& points,
			const std::vector<size_t>::iterator& idx_begin,
			const std::vector<size_t>::iterator& idx_end,
			const auto length,
			const auto axis) {

			if (idx_begin == idx_end) {
				return make_leaf<Point>();
			}

			if (length > 1) {
				argsort(points, idx_begin, idx_end, length, axis);
			}

			const auto idx_middle = idx_begin + length / 2;

			const auto idx_begin_l = idx_begin;
			const auto idx_end_l = idx_middle;
			const auto idx_begin_r = idx_middle + 1;
			const auto idx_end_r = idx_end;

			const size_t length_l = length / 2;
			const size_t length_r = length - length_l - 1;

			const size_t next_axis = (axis + 1) % kdim;

			typename node<Point>::container_type left;
			if (length_l > 0 && kdim > 0) {
				left = build_root(kdim, points, idx_begin_l, idx_end_l, length_l, next_axis);
			}
			else {
				left = make_leaf<Point>();
			}

			typename node<Point>::container_type right;
			if (length_r > 0 && kdim > 0) {
				right = build_root(kdim, points, idx_begin_r, idx_end_r, length_r, next_axis);
			}
			else {
				right = make_leaf<Point>();
			}

			return make_node<Point>(points[*idx_middle], std::move(left), std::move(right));
		}

		Distance dist_sqr(const Point& a, const Point& b) const {

			Distance dist = static_cast<Distance>(0);

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

		static std::pair<node<Point> const*, Distance> min(
			std::pair<node<Point> const*, Distance> a,
			std::pair<node<Point> const*, Distance> b) {
			return a.second < b.second ? a : b;
		}

		std::pair<node<Point> const*, Distance> find_nearest(
			const node<Point>* root,
			const Point& key,
			const int axis,
			std::pair<node<Point> const*, Distance> best) const {

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

			auto further_2 = other && delta2 < best_upd.second
				? min(best_upd, find_nearest(other, key, next_axis, further_1))
				: further_1;

			return further_2;
		}
	};

	template<class Point, class Distance>
	std::ostream& operator<<(
		std::ostream& os, tree<Point, Distance> const& tree) {
		tree.format(os);
		return os;
	}

	template<class Point, class Distance>
	bool operator==(
		const tree<Point, Distance>& a,
		const tree<Point, Distance>& b) {
		if (bool(a.root()) != bool(b.root())) return false;
		if (a.root() && *a.root() != *b.root()) return false;
		return true;
	}

	template<class Point>
	struct tree_maker {
		using type = tree<Point, typename point_traits<Point>::distance_type>;
	};

	template<class Point>
	using tree_t = tree_maker<Point>::type;

	template<class Point, class... Args>
	typename tree_t<Point> make_tree(Args&&... args) {
		return tree_t<Point>(std::forward<Args>(args)..., point_traits<Point>::kdim);
	}

	template<class Point, class... Args>
	typename tree_t<Point> build_tree(Args&&... args) {
		return tree_t<Point>::build(std::forward<Args>(args)..., point_traits<Point>::kdim);
	}
}
