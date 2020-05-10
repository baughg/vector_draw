#pragma once
#include "DrawItem.h"

namespace drawing {
	class Dimension2D : public DrawItem
	{
	public:
		Dimension2D() = default;
		Dimension2D(const DrawInfo &param);
		int draw(PDF &pdf) override;
	private:
		int width_{};
	};
}

