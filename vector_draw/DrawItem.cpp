#include "DrawItem.h"
#include <algorithm>

using namespace drawing;

DrawItem::DrawItem(const DrawInfo &param)
	: parameter_{ param }
{
}

uint64_t DrawItem::get_reduced_dimension(const int &x, const int &y)
{
	uint64_t bucket{};
	const int xr{ x - parameter_.origin_x };
	const int yr{ x - parameter_.origin_y };

	std::vector<int> dim_sum(parameter_.sub_dimension_buckets);

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

		dim_sum[d % parameter_.sub_dimension_buckets] += D;
	}

	return bucket;
}
