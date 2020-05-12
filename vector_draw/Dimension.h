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
		void set_dimension_stride();
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
	void Dimension<d>::set_dimension_stride() {
		const int colours{ parameter_.get_bucket_combinations() };
		parameter_.bucket_colours.resize(colours);
		std::vector<RGB> anchor_colours{ RGB{255U,0U,0U}, RGB{0U,255U,0U} ,RGB{0U,0U,255U} };
		
		std::vector<RGB> anchor_colour_new{ anchor_colours };

		for (int a{}; a < 3; ++a) {
			int red{ anchor_colours[a].mRed + anchor_colours[(a + 1) % 3].mRed };
			int green{ anchor_colours[a].mGreen + anchor_colours[(a + 1) % 3].mGreen };
			int blue{ anchor_colours[a].mBlue + anchor_colours[(a + 1) % 3].mBlue };
			red >>= 1; green >>= 1; blue >>= 1;

			anchor_colour_new[a].mRed = static_cast<unsigned char>(red);
			anchor_colour_new[a].mGreen = static_cast<unsigned char>(green);
			anchor_colour_new[a].mBlue = static_cast<unsigned char>(blue);
		}

		anchor_colours = std::move(anchor_colour_new);

		for (int c{}; c < colours; ++c) {
			
			parameter_.bucket_colours[c] = anchor_colours[c % 3];
			
			if (((c+1) % 3) == 0) {
				std::vector<RGB> anchor_colour_new{ anchor_colours };

				for (int a{}; a < 3; ++a) {
					int red{ anchor_colours[a].mRed + anchor_colours[(a + 1) % 3].mRed };
					int green{ anchor_colours[a].mGreen + anchor_colours[(a + 1) % 3].mGreen };
					int blue{ anchor_colours[a].mBlue + anchor_colours[(a + 1) % 3].mBlue };
					red >>= 1; green >>= 1; blue >>= 1;

					anchor_colour_new[a].mRed = static_cast<unsigned char>(red);
					anchor_colour_new[a].mGreen = static_cast<unsigned char>(green);
					anchor_colour_new[a].mBlue = static_cast<unsigned char>(blue);
				}

				anchor_colours = std::move(anchor_colour_new);
			}
		}
		const int dim_count{ d };
		parameter_.dimension_stride.resize(dim_count);
		const int repeats{ 1 << parameter_.bits_per_dimension };

		parameter_.dimension_stride[0].mod_x = parameter_.width;
		parameter_.dimension_stride[0].mod_y = parameter_.width;
		parameter_.dimension_stride[0].stride_x = parameter_.width >> parameter_.bits_per_dimension;
		parameter_.dimension_stride[0].stride_y = parameter_.dimension_stride[0].stride_x;
		parameter_.dimension_stride[0].use_x = 1;

		parameter_.dimension_stride[1] = parameter_.dimension_stride[0];
		parameter_.dimension_stride[1].use_x = 0;

		for (int dw{ 2 }; dw < dim_count; ++dw) {
			parameter_.dimension_stride[dw] = parameter_.dimension_stride[dw - 2];
			parameter_.dimension_stride[dw].mod_x *= repeats;
			parameter_.dimension_stride[dw].mod_y *= repeats;
			parameter_.dimension_stride[dw].stride_x *= repeats;
			parameter_.dimension_stride[dw].stride_y *= repeats;
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

