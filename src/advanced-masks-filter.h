#pragma once

#include <obs-module.h>
// #include <util/base.h>
// #include <util/dstr.h>
// #include <util/darray.h>
// #include <util/platform.h>
// #include <graphics/image-file.h>

// #include <stdio.h>

#include "version.h"
#include "advanced-masks.h"
#include "obs-utils.h"

static const char *advanced_masks_name(void *unused);
static void *advanced_masks_create(obs_data_t *settings, obs_source_t *source);
static void advanced_masks_destroy(void *data);
static uint32_t advanced_masks_width(void *data);
static uint32_t advanced_masks_height(void *data);
static void advanced_masks_update(void *data, obs_data_t *settings);
static void advanced_masks_video_render(void *data, gs_effect_t *effect);
static obs_properties_t *advanced_masks_properties(void *data);
static void advanced_masks_video_tick(void *data, float seconds);
static void advanced_masks_defaults(obs_data_t *settings);
static void get_input_source(advanced_masks_data_t *filter);
static void draw_output(advanced_masks_data_t *filter);
static void advanced_masks_render_filter(advanced_masks_data_t *filter);
static void render_mask(advanced_masks_data_t *filter);

	// Setup Stuff
static void load_effect_files(advanced_masks_data_t *filter);

// Shape Setup Stuff
static void shape_load_rectangle_effect(advanced_masks_data_t *filter);

// Shape Rectangle Render
static void render_rect_mask(advanced_masks_data_t *data);

static bool setting_scale_type_modified(void *data,
					obs_properties_t *props,
					obs_property_t *p,
					obs_data_t *settings);
static bool setting_corner_type_modified(obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings);


// Source Mask Stuff
static void load_source_mask_effect(advanced_masks_data_t *filter);
static void render_source_mask(advanced_masks_data_t *filter);
static bool setting_mask_type_modified(obs_properties_t *props,
				       obs_property_t *p, obs_data_t *settings);
static bool setting_mask_source_filter_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings);
static bool setting_mask_source_compression_modified(obs_properties_t *props,
						     obs_property_t *p,
						     obs_data_t *settings);
