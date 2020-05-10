#pragma once
#include "DrawItem.h"
#include "Dimension2D.h"

namespace drawing {
	template<int dim>
	class Dimension : public DrawItem
	{
	public:
		Dimension() = default;
		Dimension(const DrawInfo &param);
		int draw(PDF &pdf) override;
		void get_canvas_size(int &x, int &y);
	private:
		int width_{};
		int dimensions_{ dim };
	};

	template<int d>
	Dimension<d>::Dimension(const DrawInfo &param)
		: DrawItem { param }
	{
	}

	template<int d>
	void Dimension<d>::get_canvas_size(int &x, int &y) {
		x = parameter_.width;
		y = parameter_.width;
		const int repeats{ 1 << parameter_.bits_per_dimension };
		const int dim_count{ d };

		for (int dw{ 3 }; dw <= dim_count; ++dw) {
			const int xmask{ static_cast<int>(dw % 2 != 0) };
			const int ymask{ static_cast<int>(dw % 2 == 0) };

			x *= (xmask * repeats);
			y *= (ymask * repeats);
		}
	}

	template<int d>
	int Dimension<d>::draw(PDF &pdf) {
		if constexpr(d > 2) {
			std::vector<drawing::Dimension<d - 1>> dims;
			const int bits_per_dimension{ parameter_.bits_per_dimension };
			const int repeats{ 1 << bits_per_dimension };
			dims.reserve(repeats);
			int xstart{ parameter_.x };
			int ystart{ parameter_.y };
			DrawInfo draw_info{ parameter_ };
			int one_count{};
			const int dim_count{ d };
			const int xmask{ static_cast<int>(dim_count % 2 != 0) };
			const int ymask{ static_cast<int>(dim_count % 2 == 0) };
			const int xstep{ xmask * parameter_.width };
			const int ystep{ ymask * parameter_.width };

			for (int dw{ 0 }; dw < repeats; ++dw) {
				draw_info.x = xstart;
				draw_info.y = ystart;
				dims.emplace_back(drawing::Dimension<d - 1>{ drawing::DrawInfo{ draw_info } });
				auto it{ std::end(dims) };
				it--;
				one_count += it->draw(pdf);
				xstart += xstep;
				ystart += ystep;
			}

			return one_count;
		}
		else {
			Dimension2D dim2D{ parameter_ };
			return dim2D.draw(pdf);
		}
	}
}

