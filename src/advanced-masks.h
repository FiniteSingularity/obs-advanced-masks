#pragma once

#include <obs-module.h>

#define PLUGIN_INFO                                                                                                      \
	"<a href=\"https://github.com/finitesingularity/obs-stroke-glow-shadow/\">Plugin Template</a> (" PROJECT_VERSION \
	") by <a href=\"https://twitch.tv/finitesingularity\">FiniteSingularity</a>"

#define MASK_TYPE_SHAPE 1
#define MASK_TYPE_SHAPE_LABEL "AdvancedMasks.Shape"
#define MASK_TYPE_SOURCE 2
#define MASK_TYPE_SOURCE_LABEL "AdvancedMasks.Source"
#define MASK_TYPE_IMAGE 3
#define MASK_TYPE_IMAGE_LABEL "AdvancedMasks.Image"

#define SHAPE_RECTANGLE 1
#define SHAPE_RECTANGLE_LABEL "AdvancedMasks.Shape.Rectangle"
#define SHAPE_CIRCLE 2
#define SHAPE_CIRCLE_LABEL "AdvancedMasks.Shape.Circle"
#define SHAPE_ELLIPSE 3
#define SHAPE_ELLIPSE_LABEL "AdvancedMasks.Shape.Ellipse"
#define SHAPE_HEXAGON 4
#define SHAPE_HEXAGON_LABEL "AdvancedMasks.Shape.Hexagon"

#define MASK_SCALE_PERCENT 1
#define MASK_SCALE_PERCENT_LABEL "AdvancedMasks.ScaleType.Percent"
#define MASK_SCALE_WIDTH 2
#define MASK_SCALE_WIDTH_LABEL "AdvancedMasks.ScaleType.Width"
#define MASK_SCALE_HEIGHT 3
#define MASK_SCALE_HEIGHT_LABEL "AdvancedMasks.ScaleType.Height"

#define MASK_CORNER_UNIFORM 1
#define MASK_CORNER_UNIFORM_LABEL "AdvancedMasks.CornerType.Uniform"
#define MASK_CORNER_CUSTOM 2
#define MASK_CORNER_CUSTOM_LABEL "AdvancedMasks.CornerType.Custom"

#define MASK_SOURCE_FILTER_ALPHA 1
#define MASK_SOURCE_FILTER_ALPHA_LABEL "AdvancedMasks.SourceMask.Filter.Alpha"
#define MASK_SOURCE_FILTER_GRAYSCALE 2
#define MASK_SOURCE_FILTER_GRAYSCALE_LABEL "AdvancedMasks.SourceMask.Filter.Grayscale"
#define MASK_SOURCE_FILTER_LUMINOSITY 3
#define MASK_SOURCE_FILTER_LUMINOSITY_LABEL "AdvancedMasks.SourceMask.Filter.Luminosity"
#define MASK_SOURCE_FILTER_SLIDERS 4
#define MASK_SOURCE_FILTER_SLIDERS_LABEL "AdvancedMasks.SourceMask.Filter.Sliders"

struct advanced_masks_data;
typedef struct advanced_masks_data advanced_masks_data_t;

struct advanced_masks_data {
	obs_source_t *context;

	// Render pipeline
	bool input_texture_generated;
	gs_texrender_t *input_texrender;
	bool output_rendered;
	gs_texrender_t *output_texrender;

	gs_texrender_t *source_mask_texrender;

	// Effect Files
	gs_effect_t *effect_rectangle_mask;
	gs_effect_t *effect_source_mask;

	bool rendered;
	bool rendering;

	uint32_t width;
	uint32_t height;

	// Parameters go here
	uint32_t mask_type;

	// Parameters for shape masks
	struct vec2 mask_center;
	struct vec2 global_position;
	float global_scale;
	uint32_t scale_type;
	uint32_t last_scale_type;
	uint32_t corner_radius_type;

	// Parameters for rectangle mask
	float rectangle_width;
	float rectangle_height;
	struct vec4 rectangle_corner_radius;
	float rectangle_max_corner_radius;

	// Parameteres for source mask
	uint32_t source_mask_filter_type;
	obs_weak_source_t *mask_source_source;
	struct vec4 channel_multipliers;
	float multiplier;
	bool invert;

	// Shader Parameters
	gs_eparam_t *param_rectangle_image;
	gs_eparam_t *param_rectangle_mask_position;
	gs_eparam_t *param_rectangle_width;
	gs_eparam_t *param_rectangle_height;
	gs_eparam_t *param_global_position;
	gs_eparam_t *param_global_scale;
	gs_eparam_t *param_corner_radius;
	gs_eparam_t *param_max_corner_radius;
	gs_eparam_t *param_rect_aspect_ratio;
	gs_eparam_t *param_rectangle_aa_scale;

	gs_eparam_t *param_source_mask_image;
	gs_eparam_t *param_source_mask_source_image;
	gs_eparam_t *param_source_mask_invert;
	gs_eparam_t *param_source_channel_multipliers;
	gs_eparam_t *param_source_multiplier;
};
