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
		int get_stride_x();
		int get_stride_y();
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
	int Dimension<d>::get_stride_x() {
		if constexpr (d > 2) {
			const int dim{ d };
			const int xmask{ static_cast<int>(dim % 2 != 0) };
			int repeats{ 1 << parameter_.bits_per_dimension };

			if (!xmask) {
				repeats = 1;
			}
			const int stride{ Dimension<d - 1>{parameter_}.get_stride_x() };
			return repeats * stride;
		}
		else {
			return parameter_.width;
		}
	}

	template<int d>
	int Dimension<d>::get_stride_y() {
		if constexpr (d > 2) {
			const int dim{ d };
			const int ymask{ static_cast<int>(dim % 2 == 0) };
			int repeats{ 1 << parameter_.bits_per_dimension };

			if (!ymask) {
				repeats = 1;
			}

			const int stride{ Dimension<d - 1>{parameter_}.get_stride_y() };
			return repeats * stride;
		}
		else {
			return parameter_.width;
		}
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

			if (xmask) {
				x *= repeats;
			}

			if (ymask) {
				y *= repeats;
			}
		}

		const int border_x{ parameter_.origin_x };
		const int border_y{ parameter_.origin_y };

		x += (border_x << 1);
		y += (border_y << 1);
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
			int xstep{};
			int ystep{};

			for (int dw{ 0 }; dw < repeats; ++dw) {
				draw_info.x = xstart;
				draw_info.y = ystart;
				dims.emplace_back(drawing::Dimension<d - 1>{ drawing::DrawInfo{ draw_info } });
				auto it{ std::end(dims) };
				it--;
				one_count += it->draw(pdf);
				xstep = it->get_stride_x();
				ystep = it->get_stride_y();
				xstep *= xmask;
				ystep *= ymask;
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

