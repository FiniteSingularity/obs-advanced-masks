#pragma once

#include <obs-module.h>
#include "base-filter.h"
#include "color-adjustments.h"

#define SHAPE_RECTANGLE 1
#define SHAPE_RECTANGLE_LABEL "AdvancedMasks.Shape.Rectangle"
#define SHAPE_CIRCLE 2
#define SHAPE_CIRCLE_LABEL "AdvancedMasks.Shape.Circle"
#define SHAPE_ELLIPSE 3
#define SHAPE_ELLIPSE_LABEL "AdvancedMasks.Shape.Ellipse"
#define SHAPE_POLYGON 4
#define SHAPE_POLYGON_LABEL "AdvancedMasks.Shape.Polygon"

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

struct mask_shape_data;
typedef struct mask_shape_data mask_shape_data_t;

struct mask_shape_data {
	gs_effect_t *effect_rectangle_mask;
	gs_effect_t *effect_circle_mask;
	gs_effect_t *effect_polygon_mask;

	// General Shape Parameters
	uint32_t mask_shape_type;
	struct vec2 mask_center;
	struct vec2 global_position;
	float global_scale;
	bool shape_relative;
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

	// Parameters for Hexagon mask
	float rotation;
	float num_sides;

	// Shader file params
	gs_eparam_t *param_rectangle_image;
	gs_eparam_t *param_rectangle_uv_size;
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
	gs_eparam_t *param_rectangle_min_brightness;
	gs_eparam_t *param_rectangle_max_brightness;
	gs_eparam_t *param_rectangle_min_contrast;
	gs_eparam_t *param_rectangle_max_contrast;
	gs_eparam_t *param_rectangle_min_saturation;
	gs_eparam_t *param_rectangle_max_saturation;
	gs_eparam_t *param_rectangle_min_hue_shift;
	gs_eparam_t *param_rectangle_max_hue_shift;

	gs_eparam_t *param_circle_image;
	gs_eparam_t *param_circle_radius;
	gs_eparam_t *param_circle_mask_position;
	gs_eparam_t *param_circle_zoom;
	gs_eparam_t *param_circle_global_position;
	gs_eparam_t *param_circle_global_scale;
	gs_eparam_t *param_circle_aspect_ratio;

	// Shader file params
	gs_eparam_t *param_polygon_image;
	gs_eparam_t *param_polygon_uv_size;
	gs_eparam_t *param_polygon_mask_position;
	gs_eparam_t *param_polygon_sin_theta;
	gs_eparam_t *param_polygon_cos_theta;
	gs_eparam_t *param_polygon_radius;
	gs_eparam_t *param_polygon_num_sides;
	gs_eparam_t *param_polygon_global_position;
	gs_eparam_t *param_polygon_global_scale;
	gs_eparam_t *param_polygon_corner_radius;
	gs_eparam_t *param_polygon_max_corner_radius;
	gs_eparam_t *param_polygon_aa_scale;
	gs_eparam_t *param_polygon_zoom;
	gs_eparam_t *param_polygon_min_brightness;
	gs_eparam_t *param_polygon_max_brightness;
	gs_eparam_t *param_polygon_min_contrast;
	gs_eparam_t *param_polygon_max_contrast;
	gs_eparam_t *param_polygon_min_saturation;
	gs_eparam_t *param_polygon_max_saturation;
	gs_eparam_t *param_polygon_min_hue_shift;
	gs_eparam_t *param_polygon_max_hue_shift;

};

extern mask_shape_data_t *mask_shape_create();
extern void mask_shape_destroy(mask_shape_data_t *data);
extern void mask_shape_update(mask_shape_data_t *data, base_filter_data_t *base,
			      obs_data_t *settings);
extern void mask_shape_defaults(obs_data_t *settings);
extern void shape_mask_top_properties(obs_properties_t *props);
extern void shape_mask_bot_properties(obs_properties_t *props,
				      obs_source_t *context,
				      mask_shape_data_t *data);
extern void render_shape_mask(mask_shape_data_t *data, base_filter_data_t *base,
		       color_adjustments_data_t *color_adj);

static void render_rectangle_mask(mask_shape_data_t *data,
				  base_filter_data_t *base,
				  color_adjustments_data_t *color_adj);
static void render_circle_mask(mask_shape_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj);
static void render_polygon_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj);

extern bool setting_shape_type_modified(obs_properties_t *props,
					obs_property_t *p,
					obs_data_t *settings);
extern bool setting_shape_relative_modified(obs_properties_t *props, obs_property_t *p,
				     obs_data_t *settings);
static bool setting_corner_type_modified(obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings);
static bool setting_scale_type_modified(void *data, obs_properties_t *props,
					obs_property_t *p,
					obs_data_t *settings);

static void load_shape_effect_files(mask_shape_data_t *data);
static void load_rectangle_mask_effect(mask_shape_data_t *data);
static void load_circle_mask_effect(mask_shape_data_t *data);
static void load_polygon_mask_effect(mask_shape_data_t *data);
