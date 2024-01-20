#pragma once

#include <obs-module.h>
#include <graphics/image-file.h>
#include "base-filter.h"
#include "color-adjustments.h"

struct mask_bsm_data;
typedef struct mask_bsm_data mask_bsm_data_t;

struct mask_bsm_data {
	gs_texrender_t *bsm_mask_texrender;
	gs_texrender_t *bsm_buffer_texrender;
	gs_effect_t *effect_bsm_mask;

	obs_weak_source_t *mask_source_source;
	float fade_time;
	float seconds;
	float alpha_reduction;

	// shader params
	gs_eparam_t *param_bsm_image;
	gs_eparam_t *param_bsm_buffer;
	gs_eparam_t *param_bsm_current_input_mask;
	gs_eparam_t *param_bsm_alpha_reduction;
	gs_eparam_t *param_bsm_min_brightness;
	gs_eparam_t *param_bsm_max_brightness;
	gs_eparam_t *param_bsm_min_contrast;
	gs_eparam_t *param_bsm_max_contrast;
	gs_eparam_t *param_bsm_min_saturation;
	gs_eparam_t *param_bsm_max_saturation;
	gs_eparam_t *param_bsm_min_hue_shift;
	gs_eparam_t *param_bsm_max_hue_shift;
};

extern mask_bsm_data_t *mask_bsm_create();
extern void mask_bsm_destroy(mask_bsm_data_t *data);
extern void mask_bsm_update(mask_bsm_data_t *data, obs_data_t *settings);
extern void mask_bsm_defaults(obs_data_t *settings);
extern void bsm_mask_tick(mask_bsm_data_t *data, float seconds);
extern void bsm_mask_top_properties(obs_properties_t *props);
extern void bsm_mask_bot_properties(obs_properties_t *props);
extern void render_bsm_mask(mask_bsm_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj);

extern bool setting_mask_source_filter_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings);

static void load_bsm_effect_files(mask_bsm_data_t *data);
static void load_bsm_mask_effect(mask_bsm_data_t *data);
