#pragma once

#include "point_traits.hpp"
#include <memory>
#include <ostream>

namespace kdtree {
    template<class Point>
    class node
    {
    public:
        using point_type = Point;
        using container_type = std::unique_ptr<node>;

        node() : value_({}) {}
            
        node(const Point& value, container_type left, container_type right)
            : value_(value), left_(std::move(left)), right_(std::move(right)) {
        }

        node(const Point& value) : value_(value) {
        }

        const Point& value() const {
            return value_;
        }

        Point& value() {
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

        Point value_;
        container_type left_;
        container_type right_;
    };

    template<typename Point>
    bool operator!=(const node<Point>& a, const node<Point>& b);
        
    template<typename Point>
    bool operator==(const node<Point>& a, const node<Point>& b) {
        if (a.value() != b.value()) return false;
        
        if (bool(a.left()) != bool(b.left())) return false;
        if (bool(a.left()) && *a.left() != *b.left()) return false;
        
        if (bool(a.right()) != bool(b.right())) return false;
        if (bool(a.right()) && *a.right() != *b.right()) return false;

        return true;
    }

    template<typename Point>
    bool operator!=(const node<Point>& a, const node<Point>& b) {
        return !(a == b);
    }

    template<class Point>
    static constexpr node<Point>::container_type make_leaf() {
        return std::unique_ptr<node<Point>>();
    }

    template<class Point, class... Args>
    static constexpr node<Point>::container_type make_node(Point value, Args&&... args) {
        return std::make_unique<node<Point>>(std::forward<Point>(value), std::forward<Args>(args)...);
    }

    template<class Point>
    using node_container_t = node<Point>::container_type;

    namespace detail {
        inline void indent(std::ostream& os, const int& level) {
            for (auto i = 0; i < level; ++i) {
                os << "| ";
            }
        }

        template<class Point>
        void format(std::ostream& os, const node<Point>& node, const int& level = 0) {

            indent(os, level);

            os << point_traits<Point>::format(node.value()) << "\n";

            if (bool(node.left())) {
                format(os, *node.left(), level + 1);
            }

            if (bool(node.right())) {
                format(os, *node.right(), level + 1);
            }
        }
    }

    template<point Point>
    std::ostream& operator<<(
        std::ostream& os, const node<Point>& node) {
        detail::format(os, node);
        return os;
    }
}