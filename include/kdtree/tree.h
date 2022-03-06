#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <numeric>

#include "node.h"

namespace kdtree {

	template<typename Point, typename Distance, size_t KDim>
	class tree;

	template<typename Point, typename Distance, size_t KDim>
	std::ostream& operator<<(
		std::ostream& os, tree<Point, Distance, KDim> const& tree) {
		tree.Format(os);
		return os;
	}

	template<typename Point, typename Distance, size_t KDim>
	bool operator==(
		const tree<Point, Distance, KDim>& a,
		const tree<Point, Distance, KDim>& b) {
		return a.GetRoot()->Equals(b.GetRoot());
	}

	template<typename Point, typename Distance, size_t KDim>
	class tree {
	public:

		tree() : root_(node<Point>::make_leaf()) {
		}

		tree(const tree& other) = delete;
		
		tree(tree&& other) {
			auto tmp = root_;
			root_ = std::move(other.root_);
			other.root_ = tmp;
		}

		tree& operator=(tree&& other) {
			auto tmp = root_;
			root_ = std::move(other.root_);
			other.root_ = tmp;
			return *this;
		}

		tree(const typename node<Point>::ptr_type root)
			: root_(root) {
		}

		virtual ~tree() {
			Delete(root_);
		}

		static tree Build(const std::vector<Point>& points) {

			std::vector<size_t> idx(points.size());
			std::iota(idx.begin(), idx.end(), 0);

			auto root = BuildRoot(
				points, idx.begin(), idx.end(), idx.size(), 0);

			return tree(root);
		}

		typename node<Point>::ptr_type GetRoot() const {
			return root_;
		}

		void Format(std::ostream& os) const {
			root_->Format(os);
		}

		typename node<Point>::ptr_type find_nearest(const Point& key) const {
			return find_nearest(root_, key);
		}

	private:
		typename node<Point>::ptr_type root_;

		static void Delete(const typename node<Point>::ptr_type root) {
			if (!root) return;
			Delete(root->Left());
			Delete(root->Right());
			delete root;
		}

		static void Argsort(
			const std::vector<Point>& points,
			const std::vector<size_t>::iterator& idx_begin,
			const std::vector<size_t>::iterator& idx_end,
			const size_t& length,
			const size_t& axis) {

			auto mid = idx_begin + length / 2;
			std::nth_element(idx_begin, mid, idx_end,
				[&points, &axis](size_t a, size_t b) { return points[a][axis] < points[b][axis]; });
		}

		static typename node<Point>::ptr_type BuildRoot(
			const std::vector<Point>& points,
			const std::vector<size_t>::iterator& idx_begin,
			const std::vector<size_t>::iterator& idx_end,
			const size_t& length,
			const size_t& axis) {

			if (idx_begin == idx_end) {
				return node<Point>::make_leaf();
			}

			if (length > 1) {
				Argsort(points, idx_begin, idx_end, length, axis);
			}

			const auto idx_middle = idx_begin + length / 2;

			const auto idx_begin_l = idx_begin;
			const auto idx_end_l = idx_middle;
			const auto idx_begin_r = idx_middle + 1;
			const auto idx_end_r = idx_end;

			const size_t length_l = length / 2;
			const size_t length_r = length - length_l - 1;

			const size_t next_axis = (axis + 1) % KDim;

			typename node<Point>::ptr_type left;
			if (length_l > 0 && KDim > 0) {
				left = BuildRoot(points, idx_begin_l, idx_end_l, length_l, next_axis);
			}
			else {
				left = node<Point>::make_leaf();
			}

			typename node<Point>::ptr_type right;
			if (length_r > 0 && KDim > 0) {
				right = BuildRoot(points, idx_begin_r, idx_end_r, length_r, next_axis);
			}
			else {
				right = node<Point>::make_leaf();
			}

			return node<Point>::ptr_type_type(new node<Point>(points[*idx_middle], left, right));
		}

		static Distance dist_sqr(const Point& a, const Point& b) {

			Distance dist = static_cast<Distance>(0);

			for (auto i = 0; i < KDim; ++i) {
				auto ai = a[i];
				auto bi = b[i];
				auto di = ai - bi;
				dist += di * di;
			}

			return dist;
		}

		static typename node<Point>::ptr_type find_nearest(
			const typename node<Point>::ptr_type root,
			const Point& key) {
			Distance root_dist = dist_sqr(root->Value(), key);
			return find_nearest(root, key, 0, root, root_dist);
		}

		static typename node<Point>::ptr_type find_nearest(
			const typename node<Point>::ptr_type root,
			const Point& key,
			const size_t axis,
			const typename node<Point>::ptr_type best,
			const Distance& best_dist) {

			if (!root) return node<Point>::make_leaf();

			const Distance dist = dist_sqr(key, root->Value());

			const Distance& root_at_axis = root->Value()[axis];
			const Distance& key_at_axis = key[axis];
			const Distance delta = root_at_axis - key_at_axis;
			const Distance delta2 = delta * delta;

			typename node<Point>::ptr_type best_upd = best;
			Distance best_dist_upd = best_dist;

			if (dist < best_dist) {
				best_upd = root;
				best_dist_upd = dist;
			}

			typename node<Point>::ptr_type selected, other;

			if (delta > 0) {
				selected = root->Left();
				other = root->Right();
			}
			else {
				selected = root->Right();
				other = root->Left();
			}

			const size_t next_axis = (axis + 1) % KDim;

			{
				typename node<Point>::ptr_type further = 
					find_nearest(selected, key, next_axis, best_upd, best_dist_upd);
				
				if (further) {
					Distance further_dist = dist_sqr(further->Value(), key);
					if (further_dist < best_dist_upd) {
						best_upd = further;
						best_dist_upd = further_dist;
					}
				}
			}

			if (delta2 < best_dist_upd) {
				
				typename node<Point>::ptr_type further = 
					find_nearest(other, key, next_axis, best_upd, best_dist_upd);
				
				if (further) {
					Distance further_dist = dist_sqr(further->Value(), key);
					if (further_dist < best_dist_upd) {
						best_upd = further;
						best_dist_upd = further_dist;
					}
				}
			}

			return best_upd;
		}
	};
}
