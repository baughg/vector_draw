#include "Dimension3D.h"
#include "Dimension2D.h"

using namespace drawing;

Dimension3D::Dimension3D(const DrawInfo &param)
	: DrawItem { param }
{

}
int Dimension3D::Draw(PDF &pdf) {
	std::vector<drawing::Dimension2D> dims;
	const int bits_per_dimension{ parameter_.bits_per_dimension };
	const int repeats{ 1 << bits_per_dimension };
	dims.reserve(repeats);
	int xstart{ parameter_.x };	
	DrawInfo draw_info{ parameter_ };

	for (int d{ 0 }; d < repeats; ++d) {
		draw_info.x = xstart;
		dims.emplace_back(drawing::Dimension2D{	drawing::DrawInfo{draw_info} });
		auto it{ std::end(dims) };
		it--;
		it->Draw(pdf);
		xstart += parameter_.width;
	}

	return parameter_.width;
}
