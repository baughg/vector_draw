#pragma once
#include "DrawItem.h"

namespace drawing {
	class Dimension3D :
		public DrawItem
	{
	public:
		Dimension3D() = default;
		Dimension3D(const DrawInfo &param);
		int draw(PDF &pdf) override;
	};
}

