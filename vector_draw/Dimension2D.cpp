#include "Dimension2D.h"

using namespace drawing;

Dimension2D::Dimension2D(const DrawInfo &param)
	: DrawItem { param },
	width_ { param.width }
{
}

int Dimension2D::Draw(PDF &pdf)
{
	const int log2Points{ 2 };

	const int x{ parameter_.x };
	const int y{ parameter_.y };	
	int width{ ((width_ + (1 << log2Points)-1) >> log2Points) << log2Points };
	
	int step{ width >> log2Points };
	pdf.setLineColor(150, 150, 150);
	
	if (1) {
		for (int cy{ 0 }; cy < width; cy += step) {
			const int py{ cy + y };
			for (int cx{ 0 }; cx < width; cx += step) {
				int px{ cx + x };
				pdf.drawRect(px, py, step, step);
			}
		}
	}
	pdf.setLineColor(255, 0, 0);
	pdf.drawRect(x, y, width, width);
	return width;
}
