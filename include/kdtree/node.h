#pragma once

#include <memory>

namespace kdtree {
	template<class Value>
	class node
	{
	public:

		using ptr_type = std::unique_ptr<node>;

		node(const Value& val, ptr_type left, ptr_type right)
			: val(val), left(left), right(right) {
		}

		node(const Value& val) : val(val), left(make_leaf()), right(make_leaf()) {
		}

		static ptr_type make_leaf() {
			return ptr_type();
		}

		void format(std::ostream& os) const {
			Format(os, this);
		}

		bool equals(ptr_type const other) const {
			return Equals(this, other);
		}

		const Value& Value() const {
			return val;
		}

		Value& Value() {
			return val;
		}

		const ptr_type& Left() const {
			return left;
		}

		ptr_type& Left() {
			return left;
		}

		const ptr_type& Right() const {
			return right;
		}

		ptr_type& Right() {
			return right;
		}

	private:

		Value val;
		ptr_type left;
		ptr_type right;

		static void Indent(std::ostream& os, const size_t& level) {

			for (size_t i = 0; i < level; ++i) {
				os << "| ";
			}

			//os << "- ";
		}

		static void format(std::ostream& os, const node<Value>* node, const int& level = 0) {

			if (!bool(node)) return;

			Indent(os, level);
			os << node->val << std::endl;

			if (bool(node->left)) {
				Format(os, node->left, level + 1);
			}

			if (bool(node->right)) {
				Format(os, node->right, level + 1);
			}
		}

		static bool equals(const node<Value>* a, const node<Value>* b) {

			if (a == b) return true;
			if (!a || !b) return false;
			if (a->val != b->val) return false;
			if (!Equals(a->left, b->left)) return false;
			if (!Equals(a->right, b->right)) return false;
			return true;
		}
	};


	template<typename Value>
	bool operator==(
		const node<Value>& a, const node<Value>& b) {
		return a.Equals(b->shared_from_this());
	}
}