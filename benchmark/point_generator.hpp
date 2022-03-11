#pragma once
#include <algorithm>
#include <random>

#include <kdtree/point_traits.hpp>

namespace kdtree::benchmark {

	template<points_range Range>
	void add_noise(const Range& src, Range& dst, points_range_point_t<Range> sigma, const auto seed) {

		using point_t = points_range_point_t<Range>;
		using distance_t = point_distance_t<point_t>;

		std::default_random_engine rng;
		rng.seed(seed);

		std::normal_distribution<distance_t> nd{ distance_t(0), distance_t(1) };

		auto add_noise_to_point = [&rng, &nd, &sigma](const point_t& p) {
			point_t q = p;
			std::transform(
				std::ranges::begin(q), std::ranges::end(q),
				std::ranges::begin(sigma), std::ranges::begin(q),
				[&rng, &nd, &sigma](const distance_t& x, const distance_t& s) 
				{ return x + nd(rng) * s; });
			return q;
		};

		std::transform(std::ranges::begin(src), std::ranges::end(src), std::ranges::begin(dst), add_noise_to_point);
	}
}