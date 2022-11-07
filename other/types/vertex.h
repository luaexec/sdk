#pragma once
#include "vector.h"

struct vertex_t
{
	vertex_t() {}

	vertex_t(const vec2_t& pos, const vec2_t& tex_coord = {0, 0})
	{
		position = pos;
		coord = tex_coord;
	}
	void initialize(const vec2_t& pos, const vec2_t& tex_coord = {0, 0})
	{
		position = pos;
		coord = tex_coord;
	}

	vec2_t position;
	vec2_t coord;
};