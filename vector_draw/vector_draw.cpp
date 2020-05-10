#include <iostream>
#include <vector>
#include "Dimension4D.h"
#include "pdf.h"
#include "Dimension.h"

int main()
{
	drawing::PDF pdf;
	const int dim2Dwidth{ 256 };
	const int bits_per_dimension{ 3 };

	const uint32_t canvasSize{ (dim2Dwidth << bits_per_dimension) + 100 };
	pdf.SetWidth(canvasSize);
	pdf.SetHeight(canvasSize);
	pdf.setFont(drawing::PDF::HELVETICA, 6);
	pdf.setLineWidth(1);

	drawing::DrawInfo draw_info{ 50,50,50,50,dim2Dwidth,dim2Dwidth,bits_per_dimension,4 };
	drawing::Dimension<4> dim{ draw_info };
	dim.draw(pdf);
	
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

