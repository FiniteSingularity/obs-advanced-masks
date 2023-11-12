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

struct mask_source_data;
typedef struct mask_source_data mask_source_data_t;

struct mask_source_data {
	gs_texrender_t *source_mask_texrender;
	gs_effect_t *effect_source_mask;

	gs_image_file_t *mask_image;

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

	// shader params
	gs_eparam_t *param_source_mask_image;
	gs_eparam_t *param_source_mask_source_image;
	gs_eparam_t *param_source_mask_invert;
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

extern mask_source_data_t *mask_source_create();
extern void mask_source_destroy(mask_source_data_t *data);
extern void mask_source_update(mask_source_data_t *data, obs_data_t *settings);
extern void mask_source_defaults(obs_data_t *settings);
extern void source_mask_top_properties(obs_properties_t *props);
extern void source_mask_bot_properties(obs_properties_t *props);
extern void render_source_mask(mask_source_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj);
extern void render_image_mask(mask_source_data_t *data,
			      base_filter_data_t *base,
			      color_adjustments_data_t *color_adj);
static void set_render_params(mask_source_data_t *data,
			      color_adjustments_data_t *color_adj,
			      gs_texture_t *texture);

static bool setting_mask_source_compression_modified(obs_properties_t *props,
						     obs_property_t *p,
						     obs_data_t *settings);
extern bool setting_mask_source_filter_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings);

static void load_source_effect_files(mask_source_data_t *data);
static void load_source_mask_effect(mask_source_data_t *data);
