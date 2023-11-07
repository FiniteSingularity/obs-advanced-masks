#pragma once

#include <obs-module.h>
#include "base-filter.h"
#include "color-adjustments.h"

struct mask_gradient_data;
typedef struct mask_gradient_data mask_gradient_data_t;

struct mask_gradient_data {
	gs_effect_t *effect_gradient_mask;

	// Parameters for gradient mask
	float gradient_width;
	float gradient_position;
	float gradient_rotation;
	bool gradient_invert;
	bool gradient_debug;

	// Shader Parameters
	gs_eparam_t *param_gradient_image;
	gs_eparam_t *param_gradient_width;
	gs_eparam_t *param_gradient_position;
	gs_eparam_t *param_gradient_rotation;
	gs_eparam_t *param_gradient_uv_size;
	gs_eparam_t *param_gradient_invert;
	gs_eparam_t *param_gradient_min_brightness;
	gs_eparam_t *param_gradient_max_brightness;
	gs_eparam_t *param_gradient_min_contrast;
	gs_eparam_t *param_gradient_max_contrast;
	gs_eparam_t *param_gradient_min_saturation;
	gs_eparam_t *param_gradient_max_saturation;
	gs_eparam_t *param_gradient_min_hue_shift;
	gs_eparam_t *param_gradient_max_hue_shift;
};

extern mask_gradient_data_t *mask_gradient_create();
extern void mask_gradient_destroy(mask_gradient_data_t *data);
extern void mask_gradient_update(mask_gradient_data_t *data,
				 obs_data_t *settings);
extern void gradient_mask_properties(obs_properties_t *props);
extern void mask_gradient_defaults(obs_data_t *settings);
extern void render_gradient_mask(mask_gradient_data_t *data,
				  base_filter_data_t *base,
				  color_adjustments_data_t *color_adj);

static void load_gradient_effect_files(mask_gradient_data_t *data);
static void load_gradient_mask_effect(mask_gradient_data_t *data);
