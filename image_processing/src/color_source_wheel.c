#include "image_processing.h"


unsigned int	get_color_source(double x, double y, int *colors, int num_colors, int rot, double freq)
{
	int	angle;
	if (rot < 0)
		rot += 360;
	angle = (int)floor(freq * (atan2(y, x) * 180.0 / M_PI + 360) + 360) % 360;
	return (colors[(angle + rot) % num_colors]);
}

unsigned int get_color_source_spiral(double x, double y, int *colors, int num_colors, int rot, double spiral_strength)
{
    // theta in [0, 2π)
    double theta = atan2(y, x);
    if (theta < 0) theta += 2.0 * M_PI;

    // radius (distance from center), needed to 
    double r = hypot(x, y);

    // Spiral: add radius-dependent phase to the angle
    // spiral_strength controls how tightly it spirals.
    // Units: radians per unit of r.
    double phase = theta + spiral_strength * r;

    // map phase -> [0, 1)
    double t = fmod(phase / (2.0 * M_PI), 1.0);
    if (t < 0) t += 1.0;

    int idx = (int)floor(t * (double)num_colors);
    idx = (idx + rot) % num_colors;

    return (unsigned int)colors[idx];
}