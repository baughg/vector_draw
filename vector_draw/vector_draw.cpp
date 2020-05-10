#include <iostream>
#include <vector>
#include "Dimension4D.h"
#include "pdf.h"

int main()
{
	drawing::PDF pdf;
	const int dim2Dwidth{ 256 };
	const uint32_t canvasSize{ (dim2Dwidth << 8) + 100 };
	pdf.SetWidth(canvasSize);
	pdf.SetHeight(canvasSize);
	pdf.setFont(drawing::PDF::HELVETICA, 6);
	
	pdf.setLineWidth(1);
	drawing::Dimension4D dim4d{ drawing::DrawInfo{50,50,dim2Dwidth<<8,dim2Dwidth<<8} };
	dim4d.Draw(pdf);
	std::string outputFilename {"draw.pdf"};
	std::string errMsg{};

	if (!pdf.writeToFile(outputFilename, errMsg))
	{
		std::cout << errMsg << std::endl;
	}
}

