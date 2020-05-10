#pragma once
#include "DrawItem.h"

namespace drawing {
	class Dimension4D :
		public DrawItem
	{
	public:
		Dimension4D() = default;
		Dimension4D(const DrawInfo &param);
		int Draw(PDF &pdf) override;
	};
}

