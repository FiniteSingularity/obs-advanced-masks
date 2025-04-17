#pragma once

#include <obs-module.h>
#include "base-filter.h"

#define KEY_ADVANCED 1
#define KEY_ADVANCED_LABEL "AdvancedMasks.ChromaKey.Advanced"
#define KEY_SUPER 2
#define KEY_SUPER_LABEL "AdvancedMasks.ChromaKey.Super"

#define KEY_COLOR_TYPE_SINGLE 1
#define KEY_COLOR_TYPE_SINGLE_LABEL "AdvancedMasks.ChromaKey.Advanced.SingleColor"
#define KEY_COLOR_TYPE_DOUBLE 2
#define KEY_COLOR_TYPE_DOUBLE_LABEL "AdvancedMasks.ChromaKey.Advanced.DoubleColor"

struct mask_chroma_key_data;
typedef struct mask_chroma_key_data mask_chroma_key_data_t;

struct mask_chroma_key_data {
	gs_effect_t* effect_super_key_mask;
	gs_effect_t* effect_advanced_key_mask;

	gs_eparam_t* param_super_key_image;
	gs_eparam_t* param_super_key_k;
	gs_eparam_t* param_super_key_k2;
	gs_eparam_t* param_super_key_veil;

	gs_eparam_t* param_advanced_key_image;
	gs_eparam_t* param_advanced_key_opacity;
	gs_eparam_t* param_advanced_key_contrast;
	gs_eparam_t* param_advanced_key_brightness;
	gs_eparam_t* param_advanced_key_gamma;
	gs_eparam_t* param_advanced_key_chroma_key;
	gs_eparam_t* param_advanced_key_pixel_size;
	gs_eparam_t* param_advanced_key_similarity;
	gs_eparam_t* param_advanced_key_smoothness;
	gs_eparam_t* param_advanced_key_spill;

	uint32_t keyType;
	uint32_t advancedColorType;
	bool showMatte;

	float k;
	float k2;
	float veil;

	float opacity;
	float brightness;
	float contrast;
	float gamma;
	float similarity;
	float spill;
	float smoothness;

	struct vec2 chroma;

};

mask_chroma_key_data_t* mask_chroma_key_create();
void mask_chroma_key_destroy(mask_chroma_key_data_t* data);
void mask_chroma_key_update(mask_chroma_key_data_t* data,
	obs_data_t* settings);
void mask_chroma_key_defaults(obs_data_t* settings);
void mask_chroma_key_properties(obs_properties_t* props);
static void load_chroma_key_effect_files(mask_chroma_key_data_t* data);
static void load_super_key_mask_effect(mask_chroma_key_data_t* data);
static void load_advanced_key_mask_effect(mask_chroma_key_data_t* data);
void render_chroma_key_mask(mask_chroma_key_data_t* data, base_filter_data_t* base);
void render_advanced_key_mask(mask_chroma_key_data_t* data,
	base_filter_data_t* base);
void render_super_key_mask(mask_chroma_key_data_t* data,
	base_filter_data_t* base);
extern bool key_type_modified(obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings);
extern bool color_type_modified(obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings);
