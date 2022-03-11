#include <cstdlib>
#include <vector>

#include <benchmark/benchmark.h>

#include <kdtree/point2d.hpp>
#include <kdtree/tree.hpp>

#include "point_generator.hpp"
#include "baseline_tree.hpp"

namespace kdtree::benchmark {

    template<class Tree>
    void measure_find_nearest(
        ::benchmark::State& state,
        Tree const& tree,
        std::vector<typename Tree::point_type> const& points) {

        for (auto _ : state) {
            for (const auto& p : points) {
                ::benchmark::DoNotOptimize(tree.find_nearest(p));
            }
        }
    }

    template<points_range Points>
    void measure_build_tree(
        ::benchmark::State& state,
        const Points& points) {

        for (auto _ : state) {
            build_tree(points);
        }
    }

    std::vector<float2> make_point_cloud(size_t count, float2 origin, float2 sigma, auto seed) {
        std::vector<float2> points(count);
        std::fill(points.begin(), points.end(), origin);
        add_noise(points, points, sigma, seed);
        return points;
    }

    class SpherialClouds : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& state) {
            tree_points = make_point_cloud(state.range(0), { 0, 0 }, { 1, 1 }, 42);
            key_points = make_point_cloud(1000, { 0, 0 }, { 1, 1 }, 142);
        }

        std::vector<float2> tree_points;
        std::vector<float2> key_points;
    };

    class DistantSpherialClouds : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& state) {
            tree_points = make_point_cloud(state.range(0), { 10, 10 }, { 1, 1 }, 42);
            key_points = make_point_cloud(1000, { 0, 0 }, { 1, 1 }, 142);
        }

        std::vector<float2> tree_points;
        std::vector<float2> key_points;
    };

    class ParallelLines : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& state) {
            tree_points = make_point_cloud(state.range(0), { 0, 0 }, { 1.f, .1f }, 42);
            key_points = make_point_cloud(1000, { 0, 0 }, { 1.f, .1f }, 142);
        }

        std::vector<float2> tree_points;
        std::vector<float2> key_points;
    };

    class NormalLines : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& state) {
            tree_points = make_point_cloud(state.range(0), { 0, 0 }, { .1f, 1.f }, 42);
            key_points = make_point_cloud(1000, { 0, 0 }, { 1.f, .1f }, 142);
        }

        std::vector<float2> tree_points;
        std::vector<float2> key_points;
    };

    BENCHMARK_DEFINE_F(SpherialClouds, tree_build)(::benchmark::State& state) {
        measure_build_tree(state, tree_points);
    }
    BENCHMARK_REGISTER_F(SpherialClouds, tree_build)->Range(1024, 1 << 17);

    BENCHMARK_DEFINE_F(SpherialClouds, baseline_tree_find)(::benchmark::State& state) {

        const auto tree{ build_baseline_tree(tree_points) };
        measure_find_nearest(state, tree, key_points);
    }
    BENCHMARK_REGISTER_F(SpherialClouds, baseline_tree_find)->Range(1024, 1 << 17);

    BENCHMARK_DEFINE_F(SpherialClouds, tree_find)(::benchmark::State& state) {

        const auto tree{ build_tree(tree_points) };
        measure_find_nearest(state, tree, key_points);
    }
    BENCHMARK_REGISTER_F(SpherialClouds, tree_find)->Range(1024, 1 << 17);

    BENCHMARK_DEFINE_F(DistantSpherialClouds, tree_find)(::benchmark::State& state) {

        const auto tree{ build_tree(tree_points) };
        measure_find_nearest(state, tree, key_points);
    }
    BENCHMARK_REGISTER_F(DistantSpherialClouds, tree_find)->Range(1024, 1 << 17);

    BENCHMARK_DEFINE_F(ParallelLines, tree_find)(::benchmark::State& state) {

        const auto tree{ build_tree(tree_points) };
        measure_find_nearest(state, tree, key_points);
    }
    BENCHMARK_REGISTER_F(ParallelLines, tree_find)->Range(1024, 1 << 17);

    BENCHMARK_DEFINE_F(NormalLines, tree_find)(::benchmark::State& state) {

        const auto tree{ build_tree(tree_points) };
        measure_find_nearest(state, tree, key_points);
    }
    BENCHMARK_REGISTER_F(NormalLines, tree_find)->Range(1024, 1 << 17);
}

BENCHMARK_MAIN();