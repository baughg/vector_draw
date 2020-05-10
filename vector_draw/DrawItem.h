#pragma once
#include "pdf.h"

namespace drawing {
	typedef struct {
		int origin_x{};
		int origin_y{};
		int x{};
		int y{};
		int width{};
		int height{};
		int bits_per_dimension{ 2 };
	}DrawInfo;

	class DrawItem
	{
	public:
		DrawItem() = default;
		DrawItem(const DrawInfo &pos);
		virtual int Draw(PDF &pdf) = 0;
	protected:
		DrawInfo parameter_{};
	};
}

