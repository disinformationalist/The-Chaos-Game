#include "chaos.h"
/*how to structure

--fixed dist_ratio
--changing ratio
>function based
>> trig
>> other
>non function based, plain div, etc.
*/
//PROB FUNC BASED VERT SELECTION
/*some good fun	

		return (r->ratio_start * 2 * sin(cos(d /(r->r * r->zoom)) * cos(d /(r->r * r->zoom)) * M_PI));


*/
//---------- also make argument of trig fts adjustable..
/*nice effect but how to zoom?
if (v % 2 != 0)
		return (r->dist_ratio);	
*/
//------------------

//JUMPS TO CENTER AND MAYBE SIDES, CENTER FIXED VALUE JUMP, NOT RATIO?
/*some fts
cos
cos^2
sin^2
a * cos^2 + b * sin^2
etc...*/

// to reduce grain, try smoothing the super image, then downsample

	//return (r->dist_ratio + (r->dist_ratio - r->ratio_start) * cos(log(d /(r->r * r->zoom)) * M_PI));// try applying function dependent shifts.. this ones not great,watev
	//double shift = (r->dist_ratio - r->ratio_start) * (double)r->i / (double)r->iters;
	//double shift = cos(d /(r->r) * M_PI);
	//(void)v;






