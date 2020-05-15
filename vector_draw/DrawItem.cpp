#include "DrawItem.h"
#include <algorithm>
#include <map>
#include <deque>

using namespace drawing;

DrawItem::DrawItem(const DrawInfo &param)
	: parameter_{ param }
{
}

int DrawItem::get_reduced_dimension(const int &x, const int &y)
{
	int bucket{};
	const int xr{ x - parameter_.origin_x };
	const int yr{ y - parameter_.origin_y };

	std::vector<int> dim_sum(parameter_.sub_dimension_buckets);
	std::vector<int> dim_rank(parameter_.sub_dimension_buckets);
	std::vector<int> point(parameter_.dimensions);

	for (int d{ 0 }; d < parameter_.dimensions; ++d) {
		DimensionInfo &dim_info = parameter_.dimension_stride[d];

		int D{};

		if (dim_info.use_x) {
			D = xr % dim_info.mod_x;
			D /= dim_info.stride_x;
		}
		else {
			D = yr % dim_info.mod_y;
			D /= dim_info.stride_y;
		}

		point[d] = D;
		dim_sum[d % parameter_.sub_dimension_buckets] += D;
	}
	
	int max_val{ dim_sum[0] };
	std::map<int, std::deque<int>> sort_map;

	for (int b{0}; b < parameter_.sub_dimension_buckets; ++b) {
		sort_map[dim_sum[b]].push_back(b);	
	}

	auto end_it{ sort_map.end() };
	end_it--;

	const size_t map_size{ sort_map.size() };
	int rank{};

	for (size_t m{}; m < map_size; ++m) {
		std::deque<int> &queue = (*end_it).second;
		
		for (auto it{ std::begin(queue) }; it != std::end(queue); it++) {
			dim_rank[*it] = rank++;
		}
		end_it--;
	}
	
	for (int b{}; b < parameter_.sub_dimension_buckets; ++b) {
		bucket |= dim_rank[b];
		bucket <<= parameter_.log2_sub_dimension_buckets;
	}

	bucket >>= parameter_.log2_sub_dimension_buckets;
	return bucket;
}
