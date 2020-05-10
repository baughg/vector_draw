#ifndef _METRICS_H_
#  define _METRICS_H_

#  include "pdf.h"
namespace drawing {
	class Metrics
	{
	public:

		static int *get(PDF::Font theFont);

	private:

		Metrics();
	};
}
#endif
