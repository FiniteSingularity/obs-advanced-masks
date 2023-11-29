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
#define SHAPE_STAR 5
#define SHAPE_STAR_LABEL "AdvancedMasks.Shape.Star"
#define SHAPE_HEART 6
#define SHAPE_HEART_LABEL "AdvancedMasks.Shape.Heart"

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

#define MASK_SHAPE_FEATHER_NONE 1
#define MASK_SHAPE_FEATHER_NONE_LABEL "AdvancedMasks.Shape.Feather.None"
#define MASK_SHAPE_FEATHER_INNER 2
#define MASK_SHAPE_FEATHER_INNER_LABEL "AdvancedMasks.Shape.Feather.Inner"
#define MASK_SHAPE_FEATHER_MIDDLE 3
#define MASK_SHAPE_FEATHER_MIDDLE_LABEL "AdvancedMasks.Shape.Feather.Middle"
#define MASK_SHAPE_FEATHER_OUTER 4
#define MASK_SHAPE_FEATHER_OUTER_LABEL "AdvancedMasks.Shape.Feather.Outer"

struct mask_shape_data;
typedef struct mask_shape_data mask_shape_data_t;

struct mask_shape_data {
	gs_effect_t *effect_rectangle_mask;
	gs_effect_t *effect_circle_mask;
	gs_effect_t *effect_polygon_mask;
	gs_effect_t *effect_ellipse_mask;
	gs_effect_t *effect_star_mask;
	gs_effect_t *effect_heart_mask;

	// General Shape Parameters
	uint32_t mask_shape_type;
	struct vec2 mask_center;
	struct vec2 global_position;
	float global_scale;
	bool shape_relative;
	bool frame_check;
	uint32_t scale_type;
	uint32_t last_scale_type;
	uint32_t corner_radius_type;
	float feather_amount;
	float feather_shift;
	float shape_corner_radius;

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
	float theta;
	struct vec2 theta_s;

	// Parameters for ellipse mask
	struct vec2 ellipse;

	// Parameters for star mask
	float star_outer_radius;
	float star_corner_radius;
	float an;
	float en;
	struct vec2 acs;
	struct vec2 ecs;

	// Parameters for Heart mask
	float heart_size;

	// Shader file params
	gs_eparam_t *param_rectangle_image;
	gs_eparam_t *param_rectangle_uv_size;
	gs_eparam_t *param_rectangle_mask_position;
	gs_eparam_t *param_rectangle_width;
	gs_eparam_t *param_rectangle_height;
	gs_eparam_t *param_rectangle_sin_theta;
	gs_eparam_t *param_rectangle_cos_theta;
	gs_eparam_t *param_global_position;
	gs_eparam_t *param_global_scale;
	gs_eparam_t *param_corner_radius;
	gs_eparam_t *param_rectangle_alpha_zero;
	gs_eparam_t *param_max_corner_radius;
	gs_eparam_t *param_rectangle_feather_shift;
	gs_eparam_t *param_rectangle_feather_amount;
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
	gs_eparam_t *param_circle_uv_size;
	gs_eparam_t *param_circle_mask_position;
	gs_eparam_t *param_circle_global_position;
	gs_eparam_t *param_circle_global_scale;
	gs_eparam_t *param_circle_radius;
	gs_eparam_t *param_circle_zoom;
	gs_eparam_t *param_circle_alpha_zero;
	gs_eparam_t *param_circle_feather_amount;
	gs_eparam_t *param_circle_min_brightness;
	gs_eparam_t *param_circle_max_brightness;
	gs_eparam_t *param_circle_min_contrast;
	gs_eparam_t *param_circle_max_contrast;
	gs_eparam_t *param_circle_min_saturation;
	gs_eparam_t *param_circle_max_saturation;
	gs_eparam_t *param_circle_min_hue_shift;
	gs_eparam_t *param_circle_max_hue_shift;

	gs_eparam_t *param_polygon_image;
	gs_eparam_t *param_polygon_uv_size;
	gs_eparam_t *param_polygon_mask_position;
	gs_eparam_t *param_polygon_global_position;
	gs_eparam_t *param_polygon_global_scale;
	gs_eparam_t *param_polygon_zoom;
	gs_eparam_t *param_polygon_radius;
	gs_eparam_t *param_polygon_corner_radius;
	gs_eparam_t *param_polygon_num_sides;
	gs_eparam_t *param_polygon_sin_rot;
	gs_eparam_t *param_polygon_cos_rot;
	gs_eparam_t *param_polygon_alpha_zero;
	gs_eparam_t *param_polygon_theta;
	gs_eparam_t *param_polygon_theta_2;
	gs_eparam_t *param_polygon_theta_s;
	gs_eparam_t *param_polygon_feather_amount;
	gs_eparam_t *param_polygon_min_brightness;
	gs_eparam_t *param_polygon_max_brightness;
	gs_eparam_t *param_polygon_min_contrast;
	gs_eparam_t *param_polygon_max_contrast;
	gs_eparam_t *param_polygon_min_saturation;
	gs_eparam_t *param_polygon_max_saturation;
	gs_eparam_t *param_polygon_min_hue_shift;
	gs_eparam_t *param_polygon_max_hue_shift;

	gs_eparam_t *param_ellipse_image;
	gs_eparam_t *param_ellipse_uv_size;
	gs_eparam_t *param_ellipse_mask_position;
	gs_eparam_t *param_ellipse_global_position;
	gs_eparam_t *param_ellipse_global_scale;
	gs_eparam_t *param_ellipse_sin_rot;
	gs_eparam_t *param_ellipse_cos_rot;
	gs_eparam_t *param_ellipse_ellipse;
	gs_eparam_t *param_ellipse_zoom;
	gs_eparam_t *param_ellipse_alpha_zero;
	gs_eparam_t *param_ellipse_feather_amount;
	gs_eparam_t *param_ellipse_min_brightness;
	gs_eparam_t *param_ellipse_max_brightness;
	gs_eparam_t *param_ellipse_min_contrast;
	gs_eparam_t *param_ellipse_max_contrast;
	gs_eparam_t *param_ellipse_min_saturation;
	gs_eparam_t *param_ellipse_max_saturation;
	gs_eparam_t *param_ellipse_min_hue_shift;
	gs_eparam_t *param_ellipse_max_hue_shift;

	gs_eparam_t *param_star_image;
	gs_eparam_t *param_star_uv_size;
	gs_eparam_t *param_star_mask_position;
	gs_eparam_t *param_star_global_position;
	gs_eparam_t *param_star_global_scale;
	gs_eparam_t *param_star_sin_rot;
	gs_eparam_t *param_star_cos_rot;
	gs_eparam_t *param_star_radius;
	gs_eparam_t *param_star_corner_radius;
	gs_eparam_t *param_star_an;
	gs_eparam_t *param_star_en;
	gs_eparam_t *param_star_acs;
	gs_eparam_t *param_star_ecs;
	gs_eparam_t *param_star_zoom;
	gs_eparam_t *param_star_alpha_zero;
	gs_eparam_t *param_star_feather_amount;
	gs_eparam_t *param_star_min_brightness;
	gs_eparam_t *param_star_max_brightness;
	gs_eparam_t *param_star_min_contrast;
	gs_eparam_t *param_star_max_contrast;
	gs_eparam_t *param_star_min_saturation;
	gs_eparam_t *param_star_max_saturation;
	gs_eparam_t *param_star_min_hue_shift;
	gs_eparam_t *param_star_max_hue_shift;

	gs_eparam_t *param_heart_image;
	gs_eparam_t *param_heart_uv_size;
	gs_eparam_t *param_heart_mask_position;
	gs_eparam_t *param_heart_global_position;
	gs_eparam_t *param_heart_global_scale;
	gs_eparam_t *param_heart_sin_rot;
	gs_eparam_t *param_heart_cos_rot;
	gs_eparam_t *param_heart_size;
	gs_eparam_t *param_heart_zoom;
	gs_eparam_t *param_heart_alpha_zero;
	gs_eparam_t *param_heart_feather_amount;
	gs_eparam_t *param_heart_corner_radius;
	gs_eparam_t *param_heart_min_brightness;
	gs_eparam_t *param_heart_max_brightness;
	gs_eparam_t *param_heart_min_contrast;
	gs_eparam_t *param_heart_max_contrast;
	gs_eparam_t *param_heart_min_saturation;
	gs_eparam_t *param_heart_max_saturation;
	gs_eparam_t *param_heart_min_hue_shift;
	gs_eparam_t *param_heart_max_hue_shift;
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
static void render_ellipse_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj);
static void render_star_mask(mask_shape_data_t *data,
			     base_filter_data_t *base,
			     color_adjustments_data_t *color_adj);
static void render_heart_mask(mask_shape_data_t *data, base_filter_data_t *base,
			      color_adjustments_data_t *color_adj);
static bool recenter_button_clicked(obs_properties_t *props,
				    obs_property_t *property, void *data);
static bool setting_feather_type_modified(obs_properties_t *props,
					  obs_property_t *p,
					  obs_data_t *settings);
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
extern bool set_shape_settings_visibility(void *data, obs_properties_t *props,
					  obs_property_t *p,
					  obs_data_t *settings);

static void load_shape_effect_files(mask_shape_data_t *data);
static void load_rectangle_mask_effect(mask_shape_data_t *data);
static void load_circle_mask_effect(mask_shape_data_t *data);
static void load_ellipse_mask_effect(mask_shape_data_t *data);
static void load_polygon_mask_effect(mask_shape_data_t *data);
static void load_star_mask_effect(mask_shape_data_t *data);
static void load_heart_mask_effect(mask_shape_data_t *data);

static void shape_properties(obs_properties_t *props, obs_source_t *context,
			     mask_shape_data_t *data);
static void rectangle_corner_radius_properties(obs_properties_t *props);
static void feather_properties(obs_properties_t *props);
static void scale_position_properties(obs_properties_t *props,
				      obs_source_t *context,
				      mask_shape_data_t *data);
static float mask_width(obs_data_t *settings);
static float mask_height(obs_data_t *settings);
