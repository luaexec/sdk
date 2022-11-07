#pragma once
#include <cstdint>
#include <d3d9.h>

typedef struct {
	double r;
	double g;
	double b;
} rgb;

typedef struct {
	double h;
	double s;
	double v;
} hsv;

struct color {
	int a, r, g, b;
	color() = default;
	color(const int r, const int g, const int b, const int a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	explicit color(const uint32_t color)
	{
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}

	static color from_uint(const uint32_t uint)
	{
		return color(uint);
	}

	static D3DCOLOR from_color(const color col)
	{
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}

	// @doc: modifiers.
	[[nodiscard]] color alpha(int _a) const
	{
		return { r, g, b, _a };
	}

	[[nodiscard]] color blend(color _tmp, float fraction) const
	{
		auto r_d = float(this->r - _tmp.r), g_d = float(this->g - _tmp.g), b_d = float(this->b - _tmp.b), a_d = float(this->a - _tmp.a);
		return { int(this->r - (r_d * fraction)), int(this->g - (g_d * fraction)), int(this->b - (b_d * fraction)), int(this->a - (a_d * fraction)) };
	}

	// @doc: presets.
	static color black(const int a = 255) { return { 0, 0, 0, a }; }
	static color white(const int a = 255) { return { 255, 255, 255, a }; }
	static color red(const int a = 255) { return { 255, 0, 0, a }; }
	static color green(const int a = 255) { return { 0, 255, 0, a }; }
	static color blue(const int a = 255) { return { 0, 0, 255, a }; }

	// @doc: conversions.
	__forceinline static color hsv_to_rgb(double h, double s, double v)
	{
		double      hh, p, q, t, ff;
		long        i;
		rgb         out;

		if (s <= 0.0) {
			out.r = v;
			out.g = v;
			out.b = v;
			return color::white();
		}
		hh = h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * ff));
		t = v * (1.0 - (s * (1.0 - ff)));

		switch (i) {
			case 0:
				out.r = v;
				out.g = t;
				out.b = p;
				break;
			case 1:
				out.r = q;
				out.g = v;
				out.b = p;
				break;
			case 2:
				out.r = p;
				out.g = v;
				out.b = t;
				break;

			case 3:
				out.r = p;
				out.g = q;
				out.b = v;
				break;
			case 4:
				out.r = t;
				out.g = p;
				out.b = v;
				break;
			case 5:
			default:
				out.r = v;
				out.g = p;
				out.b = q;
				break;
		}
		return color(int(out.r * 255.f), int(out.g * 255.f), int(out.b * 255.f));
	}

	__forceinline static hsv rgb_to_hsv(int r, int g, int b)
	{
		rgb in = { (float)(r / 255), (float)(g / 255) , (float)(b / 255) };
		hsv         out;
		double      min, max, delta;

		min = in.r < in.g ? in.r : in.g;
		min = min < in.b ? min : in.b;

		max = in.r > in.g ? in.r : in.g;
		max = max > in.b ? max : in.b;

		out.v = max;
		delta = max - min;
		if (delta < 0.00001)
		{
			out.s = 0;
			out.h = 0;
			return out;
		}
		if (max > 0.0) {
			out.s = (delta / max);
		}
		else {
			out.s = 0.0;
			out.h = NAN;
			return out;
		}
		if (in.r >= max)
			out.h = (in.g - in.b) / delta;
		else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;
		else
			out.h = 4.0 + (in.r - in.g) / delta;

		out.h *= 60.0;

		if (out.h < 0.0)
			out.h += 360.0;

		return out;
	}

	// @doc: operators.
	bool operator==(const color& v) const {
		return r == v.r && g == v.g && b == v.b && a == v.a;
	}

	bool operator!=( const color& v ) const {
		return r != v.r || g != v.g || b != v.b || a != v.a;
	}
};