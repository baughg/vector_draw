#include "Dimension4D.h"
#include "Dimension3D.h"

using namespace drawing;

Dimension4D::Dimension4D(const DrawInfo &param)
	: DrawItem {param}
{
}
int Dimension4D::Draw(PDF &pdf)
{
	std::vector<drawing::Dimension3D> dims;
	dims.reserve(256);
	int xstart{ parameter_.x };
	int ystart{ parameter_.y };
	const int dim2Dwidth{ parameter_.width };
	const int ystep{ parameter_.width >> 8 };

	for (int d{ 0 }; d < 256; ++d) {
		dims.emplace_back(drawing::Dimension3D{ drawing::DrawInfo{xstart,ystart,dim2Dwidth,dim2Dwidth} });
		auto it{ std::end(dims) };
		it--;
		it->Draw(pdf);
		ystart += ystep;
	}

	return parameter_.width;
}
