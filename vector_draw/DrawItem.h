#pragma once
#include "pdf.h"

namespace drawing {
	typedef struct {
		int x{};
		int y{};
		int width{};
		int height{};
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

