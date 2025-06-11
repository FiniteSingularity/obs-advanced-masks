#pragma once

#include <obs-module.h>
#include <util/dstr.h>
#include <util/darray.h>
#include <graphics/matrix4.h>
#include "base-filter.h"
#include "color-adjustments.h"

struct mask_svg_data;
typedef struct mask_svg_data mask_svg_data_t;

#define SVG_SCALE_WIDTH 1
#define SVG_SCALE_WIDTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Width"
#define SVG_SCALE_HEIGHT 2
#define SVG_SCALE_HEIGHT_LABEL "AdvancedMasks.SvgMask.ScaleBy.Height"
#define SVG_SCALE_BOTH 3
#define SVG_SCALE_BOTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Both"

#define SVG_INPUT_TYPE_FILE 1
#define SVG_INPUT_TYPE_FILE_LABEL "AdvancedMasks.SvgMask.InputType.File"
#define SVG_INPUT_TYPE_TEXT 2
#define SVG_INPUT_TYPE_TEXT_LABEL "AdvancedMasks.SvgMask.InputType.Text"

#define SVG_GENERATE_NEW_SMALL 1
#define SVG_GENERATE_NEW_LARGE 2
#define SVG_GENERATE_ALL 3

#define SVG_ANCHOR_TOP_LEFT 1
#define SVG_ANCHOR_TOP_LEFT_LABEL "AdvancedMasks.SvgMask.Anchor.TopLeft"
#define SVG_ANCHOR_TOP_CENTER 2
#define SVG_ANCHOR_TOP_CENTER_LABEL "AdvancedMasks.SvgMask.Anchor.TopCenter"
#define SVG_ANCHOR_TOP_RIGHT 3
#define SVG_ANCHOR_TOP_RIGHT_LABEL "AdvancedMasks.SvgMask.Anchor.TopRight"
#define SVG_ANCHOR_CENTER_LEFT 4
#define SVG_ANCHOR_CENTER_LEFT_LABEL "AdvancedMasks.SvgMask.Anchor.CenterLeft"
#define SVG_ANCHOR_CENTER_CENTER 5
#define SVG_ANCHOR_CENTER_CENTER_LABEL "AdvancedMasks.SvgMask.Anchor.CenterCenter"
#define SVG_ANCHOR_CENTER_RIGHT 6
#define SVG_ANCHOR_CENTER_RIGHT_LABEL "AdvancedMasks.SvgMask.Anchor.CenterRight"
#define SVG_ANCHOR_BOTTOM_LEFT 7
#define SVG_ANCHOR_BOTTOM_LEFT_LABEL "AdvancedMasks.SvgMask.Anchor.BottomLeft"
#define SVG_ANCHOR_BOTTOM_CENTER 8
#define SVG_ANCHOR_BOTTOM_CENTER_LABEL "AdvancedMasks.SvgMask.Anchor.BottomCenter"
#define SVG_ANCHOR_BOTTOM_RIGHT 9
#define SVG_ANCHOR_BOTTOM_RIGHT_LABEL "AdvancedMasks.SvgMask.Anchor.BottomRight"
#define SVG_ANCHOR_MANUAL 10
#define SVG_ANCHOR_MANUAL_LABEL "AdvancedMasks.SvgMask.Anchor.Manual"


struct mask_svg_data {
	gs_effect_t* effect_svg_mask;
	gs_texture_t* imageTexture;

	gs_texture_t* next_size_greater_tex;
	gs_texture_t* next_size_smaller_tex;
	gs_texture_t* current_tex;

	gs_eparam_t* param_image;
	gs_eparam_t* param_svg_image;
	gs_eparam_t* param_uv_size;
	gs_eparam_t* param_svg_uv_size;
	gs_eparam_t* param_offset;
	gs_eparam_t* param_primary_alpha;
	gs_eparam_t* param_secondary_alpha;
	gs_eparam_t* param_invert;
	gs_eparam_t* param_anchor;
	gs_eparam_t* param_rotation_matrix;
	gs_eparam_t* param_min_brightness;
	gs_eparam_t* param_max_brightness;
	gs_eparam_t* param_min_contrast;
	gs_eparam_t* param_max_contrast;
	gs_eparam_t* param_min_saturation;
	gs_eparam_t* param_max_saturation;
	gs_eparam_t* param_min_hue_shift;
	gs_eparam_t* param_max_hue_shift;

	int input_type;

	uint32_t scale_by;
	uint32_t target_width;
	uint32_t target_height;

	uint32_t texture_width;
	uint32_t texture_height;

	uint32_t svg_render_width;
	uint32_t svg_render_height;

	uint32_t max_texture_size;

	int offset_x;
	int offset_y;

	float rotation;
	bool invert;

	struct vec2 anchor;
	struct matrix4 rotation_matrix;

	size_t textureIndex;

	struct dstr svg_image_path;
	struct dstr svg_text;

	DARRAY(gs_texture_t*) textures;
};

extern mask_svg_data_t* mask_svg_create(obs_data_t* settings, base_filter_data_t* base);
extern void mask_svg_destroy(mask_svg_data_t* data);
extern void mask_svg_update(mask_svg_data_t* data, base_filter_data_t* base,
	obs_data_t* settings);
extern void mask_svg_properties(obs_properties_t* props, mask_svg_data_t* data);
extern void mask_svg_defaults(obs_data_t* settings, uint32_t width, uint32_t height);
extern void render_mask_svg(mask_svg_data_t* data,
	base_filter_data_t* base, color_adjustments_data_t* color_adj);
static void load_svg_effect_files(mask_svg_data_t* data);
static void load_mask_svg_effect(mask_svg_data_t* data);
static void render_svg_to_texture(mask_svg_data_t* filter);

static void set_mask_anchor(mask_svg_data_t* data, int anchor);

static bool setting_file_path_modified(void* data,
	obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings);

bool svg_anchor_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings);
bool scale_by_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings);
bool input_type_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings);
