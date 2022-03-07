#pragma once

#include <memory>
#include <utility>

namespace kdtree {
	template<class Value>
	class node
	{
	public:
		using container_type = std::unique_ptr<node>;

		node() : value_({}) {}
			
		node(const Value& value, container_type left, container_type right)
			: value_(value), left_(std::move(left)), right_(std::move(right)) {
		}

		node(const Value& value) : value_(value) {
		}

		void format(std::ostream& os) const {
			format(os, *this);
		}

		const Value& value() const {
			return value_;
		}

		Value& value() {
			return value_;
		}

		const container_type& left() const {
			return left_;
		}

		container_type& left() {
			return left_;
		}

		const container_type& right() const {
			return right_;
		}

		container_type& right() {
			return right_;
		}

	private:

		Value value_;
		container_type left_;
		container_type right_;

		static void indent(std::ostream& os, const size_t& level) {
			for (size_t i = 0; i < level; ++i) {
				os << "| ";
			}
		}

		static void format(std::ostream& os, const node<Value>& node, const int& level = 0) {

			indent(os, level);
			os << node.value() << std::endl;

			if (bool(node.left())) {
				format(os, *node.left(), level + 1);
			}

			if (bool(node.right())) {
				format(os, *node.right(), level + 1);
			}
		}
	};

	template<typename Value>
	bool operator!=(const node<Value>& a, const node<Value>& b);
		
	template<typename Value>
	bool operator==(const node<Value>& a, const node<Value>& b) {
		if (a.value() != b.value()) return false;
		
		if (bool(a.left()) != bool(b.left())) return false;
		if (bool(a.left()) && *a.left() != *b.left()) return false;
		
		if (bool(a.right()) != bool(b.right())) return false;
		if (bool(a.right()) && *a.right() != *b.right()) return false;

		return true;
	}

	template<typename Value>
	bool operator!=(const node<Value>& a, const node<Value>& b) {
		return !(a == b);
	}

	template<class Value>
	static constexpr node<Value>::container_type make_leaf() {
		return std::unique_ptr<node<Value>>();
	}

	template<class Value, class... Args>
	static constexpr node<Value>::container_type make_node(Value value, Args&&... args) {
		return std::make_unique<node<Value>>(std::forward<Value>(value), std::forward<Args>(args)...);
	}
}