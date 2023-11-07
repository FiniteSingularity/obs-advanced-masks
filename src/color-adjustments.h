#pragma once

#include <obs-module.h>

struct color_adjustments_data;
typedef struct color_adjustments_data color_adjustments_data_t;

struct color_adjustments_data {
	// Adjustment Values
	bool adj_brightness;
	float brightness;
	float min_brightness;
	float max_brightness;

	bool adj_contrast;
	float min_contrast;
	float max_contrast;

	bool adj_saturation;
	float min_saturation;
	float max_saturation;

	bool adj_hue_shift;
	float min_hue_shift;
	float max_hue_shift;
};

extern void color_adjustments_update(color_adjustments_data_t *data,
				     obs_data_t *settings);
extern void color_adjustments_properties(obs_properties_t *props);
extern void color_adjustments_defaults(obs_data_t *settings);
static bool setting_mask_adjustment_modified(obs_properties_t *props,
					     obs_property_t *p,
					     obs_data_t *settings);
