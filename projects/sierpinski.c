// Sierpinski Triangle
#include <math.h>
#include "FPToolkit.c"

const int width = 800, height = 800;

void my_line(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	float x_inc = dx / (float) steps;
	float y_inc = dy / (float) steps;
	float x = x0;
	float y = y0;
	for (int i = 0; i <= steps; ++i) {

        float tx = (float)x / (float)width;
        float ty = (float)y / (float)height;

        if (tx < 0.0f) tx = 0.0f; else if (tx > 1.0f) tx = 1.0f;
        if (ty < 0.0f) ty = 0.0f; else if (ty > 1.0f) ty = 1.0f;

        G_rgb(tx, 0.25f * (1.0f - tx) * ty, ty);
        G_point(x, y);
		x += x_inc;
		y += y_inc;
	}
}

void midpoints(double p0[2], double p1[2], double p2[2], double depth) {
    if(depth <= 0) return;
    double mid01[2] = {(p0[0] + p1[0]) / 2, (p0[1] + p1[1]) / 2};
    double mid12[2] = {(p1[0] + p2[0]) / 2, (p1[1] + p2[1]) / 2};
    double mid20[2] = {(p2[0] + p0[0]) / 2, (p2[1] + p0[1]) / 2};

    my_line(mid01[0], mid01[1], mid12[0], mid12[1]);
    my_line(mid12[0], mid12[1], mid20[0], mid20[1]);
    my_line(mid20[0], mid20[1], mid01[0], mid01[1]);
    --depth;
    G_wait_key();
    midpoints(p0, mid01, mid20, depth);
    midpoints(p1, mid01, mid12, depth);
    midpoints(p2, mid12, mid20, depth);
}

int main()
{
    G_init_graphics(width, height);

    G_rgb(0, 0 , 0);
    G_clear();
    G_rgb(1, 1, 1);

    double p0[2] = {50, 125};
    double p1[2] = {width - 50, 125};
    double p2[2] = {width/2 - 1, (700*sin(M_PI/3.0) + 125)};
    my_line(p0[0], p0[1], p1[0], p1[1]);
    my_line(p1[0], p1[1], p2[0], p2[1]);
    my_line(p2[0], p2[1], p0[0], p0[1]);
    midpoints(p0, p1, p2, 6);
    while(G_wait_key() != 'q');
	return 0;
}

