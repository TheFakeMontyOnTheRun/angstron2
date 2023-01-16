///my take on texture mapping. 

inline Uint32 *MoveTo(int x, int y, SDL_Surface *surface) {
	Uint32 *ptr = (Uint32 *) surface->pixels;
	ptr += surface->w * y;
	ptr += x;
	return ptr;
}

inline void Set(int x, int y, SDL_Surface *surface, Uint32 color) {
	if (x < 0 || x > surface->w || y < 0 || y > surface->h) return;
	Uint32 *ptr = (Uint32 *) surface->pixels;
	ptr += surface->w * y;
	ptr += x;
	*ptr = color;
}

inline Uint32 Get(int x, int y, SDL_Surface *surface) {
	if (x < 0 || x > surface->w || y < 0 || y > surface->h) return 0;
	Uint32 *ptr = (Uint32 *) surface->pixels;
	ptr += surface->w * y;
	ptr += x;
	return *ptr;
}

/*
void MapTo(SDL_Surface *dst, int x0, int y0, int x1, int y1, SDL_Surface *texture)
{
    SDL_FillRect(dst,0,0x000000);

    int deltax = x1 - x0;
    int deltay =  y1-y0;
    float u = 0;
    float v = 0;
    float dtdx=(float)texture->w/(float)deltax;
    float dxdt=1.0/(float)dtdx;
    int c;
    int d;
    float start;


     
        start=0;
        for ( c = 1; c < deltax-1; c++) {
            v = 0;
	    start+=1.0-dxdt;
            for (d = start/2.0; d < deltay-(start/2.0); d++) {
            v+=texture->h/(deltay-start/2.0);
            Set(c, d, dst, Get(u, v, texture));
            }
            u+=dtdx;
        }        


      SDL_Flip(video);
}
*/

void MapTo(SDL_Surface *dst, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Surface *texture) {
// sentido horário!!!

	SDL_FillRect(dst, 0, 0x000000);

	int deltax = x1 - x0;
	int deltay1 = y1 - y0;
	int deltay2 = y3 - y2;
	int deltay3 = y3 - y0;
	float u = 0;
	float v = 0;
	float dtdx = (float) texture->w / (float) deltax;
	float dxdt = 1.0 / (float) dtdx;
	int c;
	int d;
	float start;
	float end;
	float dy1dx = deltay1 / (float) deltax;
	float dy2dx = deltay2 / (float) deltax;
	start = 0;
	end = 0;
	for (c = 1; c < deltax - 1; c++) {
		v = 0;
		start += dy1dx;
		end += dy1dx;
		for (d = start; d < deltay3 - end; d++) {
			v += texture->h / (deltay3 - end - start);
			Set(x0 + c, y0 + d, dst, Get(u, v, texture));
		}

		u += dtdx;
	}

}

void MapToSphere(SDL_Surface *dst, int x0, int y0, int x1, int y1, SDL_Surface *texture, int factor = 2,
				 bool alphaenabled = true) {
	int e;

	float _cos;
	float _sin;
	int u;
	int _u;
	int v;
	int inc = 4;
	int angles;

	if (alphaenabled)
		angles = 0;
	else
		angles = 90;

	float rad = 3.14159257 / 180.0;
	for (int t = 0; t < 360; t += factor * inc) {
		SDL_FillRect(dst, 0, 0x00FF00);
		for (int d = (angles) / factor; d < (360 - angles) / factor; d += factor)
			for (int c = 0; c < 180 / factor; c++) {
				_sin = sin((factor * c) * rad) * sin(factor * d * rad);
				e = _sin * 180;

				if (d * factor > 170) inc = 4 * factor;
				else if (d * factor > 130) inc = 3 * factor;
				else
					inc = 2 * factor;

				if (c < 30 / factor || c > 150 / factor)
					inc = 2 * factor;

				{
					_cos = cos((factor * c) * 3.14159257 / 180.0) * 180;

					u = t + factor * d % texture->w;
					_u = t + (360 - +factor * d) % texture->w;
					v = (180 - factor * c) % texture->h;
					if (!alphaenabled || (Get(u, v, texture) & 0xFF000000))
						for (int f = 0; f < 2 * inc * factor; f++)
							for (int g = 0; g < inc * factor; g++)
								Set(320 + g - e, 200 + f + _cos, dst, Get(u, v, texture));

				}
			}
	}
}

