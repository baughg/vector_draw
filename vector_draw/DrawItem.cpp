#include "DrawItem.h"
#include <algorithm>

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

	for (int b{1}; b < parameter_.sub_dimension_buckets; ++b) {
		if (dim_sum[b] > max_val) {
			bucket = b;
			max_val = dim_sum[b];
		}
	}
	return bucket;
}
