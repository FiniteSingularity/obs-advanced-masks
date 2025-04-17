#pragma once

#include <obs-module.h>
#include "base-filter.h"
#include "color-adjustments.h"

struct mask_feather_data;
typedef struct mask_feather_data mask_feather_data_t;

struct mask_feather_data {
	gs_effect_t* effect_feather_mask;
	gs_effect_t* effect_jump_flood_sdf;

	gs_texrender_t* buffer_a;
	gs_texrender_t* buffer_b;

	gs_eparam_t* param_feather_distance_field;
	gs_eparam_t* param_feather_image;
	gs_eparam_t* param_feather_size;
	gs_eparam_t* param_feather_uv_size;

	gs_eparam_t* param_jf_uv_size;
	gs_eparam_t* param_jf_offset;

	float featherSize;
	float intensity;
};

extern mask_feather_data_t* mask_feather_create();
extern void mask_feather_destroy(mask_feather_data_t *data);
extern void mask_feather_update(mask_feather_data_t *data,
				 obs_data_t *settings);
extern void feather_mask_properties(obs_properties_t *props);
extern void mask_feather_defaults(obs_data_t *settings);
extern void render_feather_mask(mask_feather_data_t *data,
				  base_filter_data_t *base);
static void load_feather_effect_files(mask_feather_data_t *data);
static void load_feather_mask_effect(mask_feather_data_t *data);
static void load_jump_flood_sdf_effect(mask_feather_data_t* data);
static void render_jf_inner_threshold(mask_feather_data_t* data, base_filter_data_t* base);
static void render_jf_passes_inner(mask_feather_data_t* data, base_filter_data_t* base, float maxExtent);
