#include "Dimension3D.h"
#include "Dimension2D.h"

using namespace drawing;

Dimension3D::Dimension3D(const DrawInfo &param)
	: DrawItem { param }
{

}
int Dimension3D::Draw(PDF &pdf) {
	std::vector<drawing::Dimension2D> dims;
	dims.reserve(256);
	int xstart{ parameter_.x };
	int ystart{ parameter_.y };
	const int dim2Dwidth{ parameter_.width >> 8 };

	for (int d{ 0 }; d < 256; ++d) {
		dims.emplace_back(drawing::Dimension2D{ drawing::DrawInfo{xstart,ystart,dim2Dwidth,dim2Dwidth} });
		auto it{ std::end(dims) };
		it--;
		it->Draw(pdf);
		xstart += dim2Dwidth;
	}

	return parameter_.width;
}
