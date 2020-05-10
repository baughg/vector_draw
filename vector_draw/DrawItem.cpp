#include "DrawItem.h"
#include <algorithm>

using namespace drawing;

DrawItem::DrawItem(const DrawInfo &param)
	: parameter_{ param }
{
}

uint64_t DrawItem::get_reduced_dimension(const int &x, const int &y)
{
	const int pixels_per_step{ parameter_.width >> parameter_.bits_per_dimension };
	int gx{ x - parameter_.origin_x };
	int gy{ y - parameter_.origin_y };
	int X{ (gx % parameter_.width) / pixels_per_step };
	int Y{ (gy % parameter_.width) / pixels_per_step };
	int Z{ gx / parameter_.width };
	int T{ gy / parameter_.width };

	uint64_t bucket{};

	X += Z;
	Y += T;
	int XeqY{ X ^ Y };

	if (X == Y) {
		int add{ X & 0x1 };
		X += add;
		add = (~add & 0x1);
		Y += add;
	}
	bucket |= static_cast<uint64_t>(Y > X);
	return bucket;
}
