#pragma once

#include <obs-module.h>
#include "version.h"
#include "advanced-masks.h"
#include "obs-utils.h"

static const char *advanced_masks_name(void *unused);
static void *advanced_masks_create(obs_data_t *settings, obs_source_t *source);
static void advanced_masks_destroy(void *data);
static uint32_t advanced_masks_width(void *data);
static uint32_t advanced_masks_height(void *data);
static void advanced_masks_update(void *data, obs_data_t *settings);
static void advanced_masks_update_v2(void *data, obs_data_t *settings);
static void advanced_masks_video_render(void *data, gs_effect_t *effect);
static bool advanced_masks_multi_pass(advanced_masks_data_t* filter);
static obs_properties_t *advanced_masks_properties(void *data);
static void advanced_masks_video_tick(void *data, float seconds);
static void advanced_masks_defaults(obs_data_t *settings);
static void advanced_masks_defaults_v2(obs_data_t *settings);
extern void get_input_source(base_filter_data_t* filter);
static void draw_output(advanced_masks_data_t *filter);
static void advanced_masks_render_filter(advanced_masks_data_t *filter);
static void render_mask(advanced_masks_data_t *filter);

static bool setting_mask_effect_modified(void *data, obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings);

static bool setting_mask_adjustment_modified(obs_properties_t *props,
					     obs_property_t *p,
					     obs_data_t *settings);

static bool setting_mask_type_modified(void *data, obs_properties_t *props,
				       obs_property_t *p, obs_data_t *settings);
static void load_output_effect(advanced_masks_data_t *filter);
