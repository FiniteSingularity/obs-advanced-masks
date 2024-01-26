#pragma once

#include <obs-module.h>
#include <graphics/image-file.h>
#include "base-filter.h"
#include "color-adjustments.h"

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
#define MASK_SOURCE_COMPRESSION_THRESHOLD_LABEL "AdvancedMasks.SourceMask.Compression.Threshold"
#define MASK_SOURCE_COMPRESSION_RANGE 3
#define MASK_SOURCE_COMPRESSION_RANGE_LABEL "AdvancedMasks.SourceMask.Compression.Range"

#define MASK_SOURCE_SCALING_STRETCH 0
#define MASK_SOURCE_SCALING_STRETCH_LABEL "AdvancedMasks.SourceMask.SourceScaling.Stretch"
#define MASK_SOURCE_SCALING_MANUAL 1
#define MASK_SOURCE_SCALING_MANUAL_LABEL "AdvancedMasks.SourceMask.SourceScaling.Manual"

#define MASK_SOURCE_SCALE_BY_PERCENT 0
#define MASK_SOURCE_SCALE_BY_PERCENT_LABEL "AdvancedMasks.SourceMask.SourceScaling.ScaleBy.Percent"
#define MASK_SOURCE_SCALE_BY_WIDTH 1
#define MASK_SOURCE_SCALE_BY_WIDTH_LABEL "AdvancedMasks.SourceMask.SourceScaling.ScaleBy.Width"
#define MASK_SOURCE_SCALE_BY_HEIGHT 2
#define MASK_SOURCE_SCALE_BY_HEIGHT_LABEL "AdvancedMasks.SourceMask.SourceScaling.ScaleBy.Height"
#define MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT 3
#define MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT_LABEL "AdvancedMasks.SourceMask.SourceScaling.ScaleBy.WidthHeight"

#define MASK_SOURCE_BOUNDARY_NONE 0
#define MASK_SOURCE_BOUNDARY_NONE_LABEL "AdvancedMasks.SourceMask.SourceScaling.Boundary.None"
#define MASK_SOURCE_BOUNDARY_EXTEND 1
#define MASK_SOURCE_BOUNDARY_EXTEND_LABEL "AdvancedMasks.SourceMask.SourceScaling.Boundary.Extend"
#define MASK_SOURCE_BOUNDARY_TILE 2
#define MASK_SOURCE_BOUNDARY_TILE_LABEL "AdvancedMasks.SourceMask.SourceScaling.Boundary.Tile"
#define MASK_SOURCE_BOUNDARY_MIRROR 3
#define MASK_SOURCE_BOUNDARY_MIRROR_LABEL "AdvancedMasks.SourceMask.SourceScaling.Boundary.Mirror"

#define MASK_SOURCE_MASK_POSITION_TL 0
#define MASK_SOURCE_MASK_POSITION_TL_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.TL"
#define MASK_SOURCE_MASK_POSITION_TC 1
#define MASK_SOURCE_MASK_POSITION_TC_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.TC"
#define MASK_SOURCE_MASK_POSITION_TR 2
#define MASK_SOURCE_MASK_POSITION_TR_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.TR"
#define MASK_SOURCE_MASK_POSITION_CL 3
#define MASK_SOURCE_MASK_POSITION_CL_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.CL"
#define MASK_SOURCE_MASK_POSITION_CC 4
#define MASK_SOURCE_MASK_POSITION_CC_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.CC"
#define MASK_SOURCE_MASK_POSITION_CR 5
#define MASK_SOURCE_MASK_POSITION_CR_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.CR"
#define MASK_SOURCE_MASK_POSITION_BL 6
#define MASK_SOURCE_MASK_POSITION_BL_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.BL"
#define MASK_SOURCE_MASK_POSITION_BC 7
#define MASK_SOURCE_MASK_POSITION_BC_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.BC"
#define MASK_SOURCE_MASK_POSITION_BR 8
#define MASK_SOURCE_MASK_POSITION_BR_LABEL "AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment.BR"

struct mask_source_data;
typedef struct mask_source_data mask_source_data_t;

struct mask_source_data {
	gs_texrender_t *source_mask_texrender;
	gs_effect_t *effect_source_mask;

	gs_image_file_t *mask_image;

	bool loading_effect;
	uint32_t source_mask_filter_type;
	obs_weak_source_t *mask_source_source;
	struct vec4 channel_multipliers;
	float multiplier;
	bool source_invert;
	bool threshold;
	uint32_t compression_type;
	float threshold_value;
	float range_min;
	float range_max;
	char *mask_image_path;
	uint32_t mask_source_scaling;
	uint32_t mask_source_scale_by;
	float mask_scale_pct;
	float mask_scale_width;
	float mask_scale_height;
	struct vec2 mask_source_size;
	struct vec2 source_size;
	struct vec2 mask_offset;
	struct vec2 mask_position;
	uint32_t boundary_horizontal;
	uint32_t boundary_vertical;
	struct vec2 positional_alignment;
	float rotation;

	// shader params
	gs_eparam_t *param_source_mask_image;
	gs_eparam_t *param_source_mask_source_image;
	gs_eparam_t *param_source_source_image_size;
	gs_eparam_t *param_source_mask_image_size;
	gs_eparam_t *param_source_mask_offset;
	gs_eparam_t *param_source_mask_invert;
	gs_eparam_t *param_source_mask_positional_offset;
	gs_eparam_t *param_source_mask_rotation;
	gs_eparam_t *param_source_mask_position;
	gs_eparam_t *param_source_channel_multipliers;
	gs_eparam_t *param_source_multiplier;
	gs_eparam_t *param_source_threshold_value;
	gs_eparam_t *param_source_range_min;
	gs_eparam_t *param_source_range_max;
	gs_eparam_t *param_source_min_brightness;
	gs_eparam_t *param_source_max_brightness;
	gs_eparam_t *param_source_min_contrast;
	gs_eparam_t *param_source_max_contrast;
	gs_eparam_t *param_source_min_saturation;
	gs_eparam_t *param_source_max_saturation;
	gs_eparam_t *param_source_min_hue_shift;
	gs_eparam_t *param_source_max_hue_shift;
};

extern mask_source_data_t *mask_source_create(obs_data_t *settings);
extern void mask_source_destroy(mask_source_data_t *data);
extern void mask_source_update(mask_source_data_t *data,
			       obs_data_t *settings);
extern void mask_source_defaults(obs_data_t *settings);
extern void source_mask_top_properties(obs_properties_t *props,
				       mask_source_data_t *data);
extern void source_mask_bot_properties(obs_properties_t *props,
				       mask_source_data_t *data);
extern void render_source_mask(mask_source_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj);
extern void render_image_mask(mask_source_data_t *data,
			      base_filter_data_t *base,
			      color_adjustments_data_t *color_adj);
static void set_render_params(mask_source_data_t *data,
			      color_adjustments_data_t *color_adj,
			      gs_texture_t *texture);

static bool setting_mask_source_scale_by_modified(void *data,
						  obs_properties_t *props,
						  obs_property_t *p,
						  obs_data_t *settings);
static bool setting_mask_source_source_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings);
static bool setting_mask_source_compression_modified(obs_properties_t *props,
						     obs_property_t *p,
						     obs_data_t *settings);
extern bool setting_mask_source_filter_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings);
static bool setting_mask_source_scaling_modified(void *data,
						 obs_properties_t *props,
						 obs_property_t *p,
						 obs_data_t *settings);
static bool setting_mask_source_boundary_modified(void *data,
						  obs_properties_t *props,
						  obs_property_t *p,
						  obs_data_t *settings);
static const char *get_addr_mode(uint32_t index);
static void load_source_effect_files(mask_source_data_t *data);
static void load_source_mask_effect(mask_source_data_t *data);
static float get_position_width_factor(uint32_t pa);
static float get_position_height_factor(uint32_t pa);
