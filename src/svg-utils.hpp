#pragma once

#include <obs-module.h>

extern "C" gs_texture_t* gs_texture_from_svg_path(const char* path, int width, int height, int scale_by);
extern "C" gs_texture_t* gs_texture_from_svg(const char* svg, int width, int height, int scale_by);
