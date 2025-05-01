#pragma once

#include <obs-module.h>

extern "C" void update_svg_textures(
	const char* path,
	int width,
	int height,
	int scale_by,
	int update_type,
	gs_texture_t** smallTex,
	gs_texture_t** currentTex,
	gs_texture_t** largeTex
);

extern "C" gs_texture_t* gs_texture_from_svg_path(const char* path, int width, int height, int scale_by);
extern "C" gs_texture_t* gs_texture_from_svg(const char* svg, int width, int height, int scale_by);
