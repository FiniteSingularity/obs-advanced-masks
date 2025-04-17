#pragma once

#include <obs-module.h>
#include <util/dstr.h>
#include "base-filter.h"

struct mask_svg_data;
typedef struct mask_svg_data mask_svg_data_t;

#define SVG_SCALE_WIDTH 1
#define SVG_SCALE_WIDTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Width"
#define SVG_SCALE_HEIGHT 2
#define SVG_SCALE_HEIGHT_LABEL "AdvancedMasks.SvgMask.ScaleBy.Height"
#define SVG_SCALE_BOTH 3
#define SVG_SCALE_BOTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Both"

#define SVG_GENERATE_NEW_SMALL 1
#define SVG_GENERATE_NEW_LARGE 2
#define SVG_GENERATE_ALL 3

struct mask_svg_data {
	gs_effect_t* effect_svg_mask;
	gs_texture_t* imageTexture;

	gs_texture_t* next_size_greater_tex;
	gs_texture_t* next_size_smaller_tex;
	gs_texture_t* current_tex;

	gs_eparam_t* param_svg_image;
	gs_eparam_t* param_uv_size;
	gs_eparam_t* param_svg_uv_size;

	uint32_t scale_by;
	uint32_t target_width;
	uint32_t target_height;

	uint32_t texture_width;
	uint32_t texture_height;

	uint32_t svg_render_width;
	uint32_t svg_render_height;

	struct dstr svg_image_path;
};

extern mask_svg_data_t* mask_svg_create(obs_data_t* settings);
extern void mask_svg_destroy(mask_svg_data_t* data);
extern void mask_svg_update(mask_svg_data_t* data,
	obs_data_t* settings);
extern void mask_svg_properties(obs_properties_t* props, mask_svg_data_t* data);
extern void mask_svg_defaults(obs_data_t* settings);
extern void render_mask_svg(mask_svg_data_t* data,
	base_filter_data_t* base);
static void load_svg_effect_files(mask_svg_data_t* data);
static void load_mask_svg_effect(mask_svg_data_t* data);
static void render_svg_to_texture(mask_svg_data_t* filter);

static bool setting_file_path_modified(void* data,
	obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings);

extern uint32_t next_power_of_2(uint32_t n);
extern uint32_t previous_power_of_2(uint32_t n);
