#pragma once
#include "pdf.h"
#include <vector>

namespace drawing {
	typedef struct {
		int stride_x{ 1 };
		int stride_y{ 1 };
		int mod_x{ 1 };
		int mod_y{ 1 };
		int use_x{ 0 };
	}DimensionInfo;

	typedef struct {
		int origin_x{};
		int origin_y{};
		int x{};
		int y{};
		int width{};
		int height{};
		int bits_per_dimension{ 2 };
		int dimensions{ 2 };
		int sub_dimension_buckets{ 2 };
		std::vector<DimensionInfo> dimension_stride;
		std::vector<RGB> bucket_colours;
		std::shared_ptr<std::vector<int>> histogram;
	}DrawInfo;

	class DrawItem
	{
	public:
		DrawItem() = default;
		DrawItem(const DrawInfo &pos);
		virtual int draw(PDF &pdf) = 0;
	protected:
		int get_reduced_dimension(const int &x, const int &y);
		DrawInfo parameter_{};
	};
}

