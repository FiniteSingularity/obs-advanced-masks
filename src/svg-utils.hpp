#pragma once

#include <obs-module.h>

extern "C" void update_svg_textures(
	const char* path,
	int width,
	int height,
	int scale_by,
	int update_type,
	gs_texture_t** small,
	gs_texture_t** current,
	gs_texture_t** large
);

extern "C" gs_texture_t* gs_texture_from_svg(const char* path, int width, int height, int scale_by);
