#include "Dimension4D.h"
#include "Dimension3D.h"

using namespace drawing;

Dimension4D::Dimension4D(const DrawInfo &param)
	: DrawItem {param}
{
}
int Dimension4D::draw(PDF &pdf)
{
	std::vector<drawing::Dimension3D> dims;
	const int bits_per_dimension{ parameter_.bits_per_dimension };
	const int repeats{ 1 << bits_per_dimension };
	dims.reserve(repeats);
	int ystart{ parameter_.y };	
	const int ystep{ parameter_.width};

	DrawInfo draw_info{ parameter_ };
	int one_count{};

	for (int d{ 0 }; d < repeats; ++d) {
		draw_info.y = ystart;
		dims.emplace_back(drawing::Dimension3D{ drawing::DrawInfo{ draw_info} });
		auto it{ std::end(dims) };
		it--;
		one_count += it->draw(pdf);
		ystart += ystep;
	}

	return one_count;
}
