#pragma once

#include <obs-module.h>

#define PLUGIN_INFO                                                                                                      \
	"<a href=\"https://github.com/finitesingularity/obs-stroke-glow-shadow/\">Plugin Template</a> (" PROJECT_VERSION \
	") by <a href=\"https://twitch.tv/finitesingularity\">FiniteSingularity</a>"

#define MASK_EFFECT_ALPHA 1
#define MASK_EFFECT_ALPHA_LABEL "AdvancedMasks.Effects.Alpha"
#define MASK_EFFECT_ADJUSTMENT 2
#define MASK_EFFECT_ADJUSTMENT_LABEL "AdvancedMasks.Effects.Adjustment"

#define MASK_TYPE_SHAPE 1
#define MASK_TYPE_SHAPE_LABEL "AdvancedMasks.Shape"
#define MASK_TYPE_SOURCE 2
#define MASK_TYPE_SOURCE_LABEL "AdvancedMasks.Source"
#define MASK_TYPE_IMAGE 3
#define MASK_TYPE_IMAGE_LABEL "AdvancedMasks.Image"
#define MASK_TYPE_GRADIENT 4
#define MASK_TYPE_GRADIENT_LABEL "AdvancedMasks.Gradient"

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

#define MASK_SOURCE_COMPRESSION_NONE 1
#define MASK_SOURCE_COMPRESSION_NONE_LABEL "AdvancedMasks.SourceMask.Compression.None"
#define MASK_SOURCE_COMPRESSION_THRESHOLD 2
#define MASK_SOURCE_COMPRESSION_THRESHOLD_LABEL \
	"AdvancedMasks.SourceMask.Compression.Threshold"
#define MASK_SOURCE_COMPRESSION_RANGE 3
#define MASK_SOURCE_COMPRESSION_RANGE_LABEL \
	"AdvancedMasks.SourceMask.Compression.Range"

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
	gs_effect_t *effect_circle_mask;
	gs_effect_t *effect_source_mask;
	gs_effect_t *effect_gradient_mask;

	bool rendered;
	bool rendering;

	uint32_t width;
	uint32_t height;

	// Parameters go here
	uint32_t mask_effect;
	uint32_t mask_type;

	// Adjustment Values
	bool adj_brightness;
	float brightness;
	float min_brightness;
	float max_brightness;

	// Parameters for shape masks
	uint32_t mask_shape_type;
	struct vec2 mask_center;
	struct vec2 global_position;
	float global_scale;
	uint32_t scale_type;
	uint32_t last_scale_type;
	uint32_t corner_radius_type;

	// Parameters for rectangle mask
	float rectangle_width;
	float rectangle_height;
	float zoom;
	struct vec4 rectangle_corner_radius;
	float rectangle_max_corner_radius;

	// Parameters for circle mask
	float radius;

	// Parameters for source mask
	uint32_t source_mask_filter_type;
	obs_weak_source_t *mask_source_source;
	struct vec4 channel_multipliers;
	float multiplier;
	bool invert;
	bool threshold;
	uint32_t compression_type;
	float threshold_value;
	float range_min;
	float range_max;

	// Parameters for gradient mask
	float gradient_width;
	float gradient_position;
	float gradient_rotation;
	bool gradient_debug;

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
	gs_eparam_t *param_rectangle_zoom;

	gs_eparam_t *param_circle_image;
	gs_eparam_t *param_circle_radius;
	gs_eparam_t *param_circle_mask_position;
	gs_eparam_t *param_circle_zoom;
	gs_eparam_t *param_circle_global_position;
	gs_eparam_t *param_circle_global_scale;
	gs_eparam_t *param_circle_aspect_ratio;

	gs_eparam_t *param_source_mask_image;
	gs_eparam_t *param_source_mask_source_image;
	gs_eparam_t *param_source_mask_invert;
	gs_eparam_t *param_source_channel_multipliers;
	gs_eparam_t *param_source_multiplier;
	gs_eparam_t *param_source_threshold_value;
	gs_eparam_t *param_source_range_min;
	gs_eparam_t *param_source_range_max;
	gs_eparam_t *param_source_adj_brightness;
	gs_eparam_t *param_source_min_brightness;
	gs_eparam_t *param_source_max_brightness;

	gs_eparam_t *param_gradient_image;
	gs_eparam_t *param_gradient_width;
	gs_eparam_t *param_gradient_position;
	gs_eparam_t *param_gradient_rotation;
	gs_eparam_t *param_gradient_uv_size;
	gs_eparam_t *param_gradient_adj_brightness;
	gs_eparam_t *param_gradient_min_brightness;
	gs_eparam_t *param_gradient_max_brightness;
};
