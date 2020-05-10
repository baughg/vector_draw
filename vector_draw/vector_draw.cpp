#include <iostream>
#include <vector>
#include "Dimension4D.h"
#include "pdf.h"
#include "Dimension.h"

int main()
{
	drawing::PDF pdf;
	const int dim2Dwidth{ 256 };
	const int bits_per_dimension{ 2 };
		
	pdf.setFont(drawing::PDF::HELVETICA, 6);
	pdf.setLineWidth(1);
	const int dim_count{ 6 };

	drawing::DrawInfo draw_info{ 50,50,50,50,dim2Dwidth,dim2Dwidth,bits_per_dimension,dim_count };
	drawing::Dimension<dim_count> dim{ draw_info };
	dim.set_dimension_stride();
	int canvas_width{};
	int canvas_height{};

	dim.get_canvas_size(canvas_width, canvas_height);
	
	pdf.SetWidth(canvas_width);
	pdf.SetHeight(canvas_height);

	int one_count{ dim.draw(pdf) };
	
	//drawing::Dimension4D dim4d{ draw_info };
	//int one_count{ dim4d.draw(pdf) };
	//const int points{ 1 << (bits_per_dimension << 2) };
	std::string outputFilename {"draw.pdf"};
	std::string errMsg{};

	if (!pdf.writeToFile(outputFilename, errMsg))
	{
		std::cout << errMsg << std::endl;
	}
}

