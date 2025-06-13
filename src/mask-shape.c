#include "mask-shape.h"
#include "obs-utils.h"

mask_shape_data_t *mask_shape_create()
{
	mask_shape_data_t *data = bzalloc(sizeof(mask_shape_data_t));

	data->effect_rectangle_mask = NULL;
	data->effect_circle_mask = NULL;
	data->effect_polygon_mask = NULL;
	data->effect_ellipse_mask = NULL;
	data->effect_star_mask = NULL;
	data->effect_heart_mask = NULL;

	data->param_rectangle_image = NULL;
	data->param_rectangle_uv_size = NULL;
	data->param_rectangle_mask_position = NULL;
	data->param_rectangle_width = NULL;
	data->param_rectangle_height = NULL;
	data->param_rectangle_sin_theta = NULL;
	data->param_rectangle_cos_theta = NULL;
	data->param_rectangle_alpha_zero = NULL;
	data->param_global_position = NULL;
	data->param_global_scale = NULL;
	data->param_corner_radius = NULL;
	data->param_rect_aspect_ratio = NULL;
	data->param_rectangle_aa_scale = NULL;
	data->param_max_corner_radius = NULL;
	data->param_rectangle_feather_amount = NULL;
	data->param_rectangle_feather_shift = NULL;
	data->param_rectangle_zoom = NULL;
	data->param_rectangle_min_brightness = NULL;
	data->param_rectangle_max_brightness = NULL;
	data->param_rectangle_min_contrast = NULL;
	data->param_rectangle_max_contrast = NULL;
	data->param_rectangle_min_saturation = NULL;
	data->param_rectangle_max_saturation = NULL;
	data->param_rectangle_min_hue_shift = NULL;
	data->param_rectangle_max_hue_shift = NULL;

	data->param_circle_image = NULL;
	data->param_circle_uv_size = NULL;
	data->param_circle_mask_position = NULL;
	data->param_circle_global_position = NULL;
	data->param_circle_global_scale = NULL;
	data->param_circle_radius = NULL;
	data->param_circle_zoom = NULL;
	data->param_circle_alpha_zero = NULL;
	data->param_circle_feather_amount = NULL;
	data->param_circle_min_brightness = NULL;
	data->param_circle_max_brightness = NULL;
	data->param_circle_min_contrast = NULL;
	data->param_circle_max_contrast = NULL;
	data->param_circle_min_saturation = NULL;
	data->param_circle_max_saturation = NULL;
	data->param_circle_min_hue_shift = NULL;
	data->param_circle_max_hue_shift = NULL;

	data->param_polygon_image = NULL;
	data->param_polygon_uv_size = NULL;
	data->param_polygon_mask_position = NULL;
	data->param_polygon_sin_rot = NULL;
	data->param_polygon_cos_rot = NULL;
	data->param_polygon_theta = NULL;
	data->param_polygon_theta_2 = NULL;
	data->param_polygon_theta_s = NULL;
	data->param_polygon_radius = NULL;
	data->param_polygon_num_sides = NULL;
	data->param_polygon_global_position = NULL;
	data->param_polygon_global_scale = NULL;
	data->param_polygon_corner_radius = NULL;
	data->param_polygon_zoom = NULL;
	data->param_polygon_alpha_zero = NULL;
	data->param_polygon_feather_amount = NULL;
	data->param_polygon_min_brightness = NULL;
	data->param_polygon_max_brightness = NULL;
	data->param_polygon_min_contrast = NULL;
	data->param_polygon_max_contrast = NULL;
	data->param_polygon_min_saturation = NULL;
	data->param_polygon_max_saturation = NULL;
	data->param_polygon_min_hue_shift = NULL;
	data->param_polygon_max_hue_shift = NULL;

	data->param_ellipse_image = NULL;
	data->param_ellipse_uv_size = NULL;
	data->param_ellipse_mask_position = NULL;
	data->param_ellipse_global_position = NULL;
	data->param_ellipse_global_scale = NULL;
	data->param_ellipse_sin_rot = NULL;
	data->param_ellipse_cos_rot = NULL;
	data->param_ellipse_ellipse = NULL;
	data->param_ellipse_zoom = NULL;
	data->param_ellipse_alpha_zero = NULL;
	data->param_ellipse_feather_amount = NULL;
	data->param_ellipse_min_brightness = NULL;
	data->param_ellipse_max_brightness = NULL;
	data->param_ellipse_min_contrast = NULL;
	data->param_ellipse_max_contrast = NULL;
	data->param_ellipse_min_saturation = NULL;
	data->param_ellipse_max_saturation = NULL;
	data->param_ellipse_min_hue_shift = NULL;
	data->param_ellipse_max_hue_shift = NULL;

	data->param_star_image = NULL;
	data->param_star_uv_size = NULL;
	data->param_star_mask_position = NULL;
	data->param_star_global_position = NULL;
	data->param_star_global_scale = NULL;
	data->param_star_sin_rot = NULL;
	data->param_star_cos_rot = NULL;
	data->param_star_alpha_zero = NULL;
	data->param_star_radius = NULL;
	data->param_star_corner_radius = NULL;
	data->param_star_an = NULL;
	data->param_star_en = NULL;
	data->param_star_acs = NULL;
	data->param_star_ecs = NULL;
	data->param_star_zoom = NULL;
	data->param_star_feather_amount = NULL;
	data->param_star_min_brightness = NULL;
	data->param_star_max_brightness = NULL;
	data->param_star_min_contrast = NULL;
	data->param_star_max_contrast = NULL;
	data->param_star_min_saturation = NULL;
	data->param_star_max_saturation = NULL;
	data->param_star_min_hue_shift = NULL;
	data->param_star_max_hue_shift = NULL;

	data->param_heart_image = NULL;
	data->param_heart_uv_size = NULL;
	data->param_heart_mask_position = NULL;
	data->param_heart_global_position = NULL;
	data->param_heart_global_scale = NULL;
	data->param_heart_sin_rot = NULL;
	data->param_heart_cos_rot = NULL;
	data->param_heart_alpha_zero = NULL;
	data->param_heart_size = NULL;
	data->param_heart_zoom = NULL;
	data->param_heart_corner_radius = NULL;
	data->param_heart_feather_amount = NULL;
	data->param_heart_min_brightness = NULL;
	data->param_heart_max_brightness = NULL;
	data->param_heart_min_contrast = NULL;
	data->param_heart_max_contrast = NULL;
	data->param_heart_min_saturation = NULL;
	data->param_heart_max_saturation = NULL;
	data->param_heart_min_hue_shift = NULL;
	data->param_heart_max_hue_shift = NULL;

	data->param_super_uv_size = NULL;
	data->param_super_mask_position = NULL;
	data->param_super_zoom = NULL;
	data->param_super_global_position = NULL;
	data->param_super_global_scale = NULL;
	data->param_super_sin_theta = NULL;
	data->param_super_cos_theta = NULL;
	data->param_super_invert = NULL;
	data->param_super_alpha_zero = NULL;
	data->param_super_m = NULL;
	data->param_super_n1 = NULL;
	data->param_super_n2 = NULL;
	data->param_super_n3 = NULL;
	data->param_super_a = NULL;
	data->param_super_b = NULL;
	data->param_super_min_r = NULL;
	data->param_super_min_brightness = NULL;
	data->param_super_max_brightness = NULL;
	data->param_super_min_contrast = NULL;
	data->param_super_max_contrast = NULL;
	data->param_super_min_saturation = NULL;
	data->param_super_max_saturation = NULL;
	data->param_super_min_hue_shift = NULL;
	data->param_super_max_hue_shift = NULL;

	load_shape_effect_files(data);
	data->last_scale_type = (uint32_t)0;
	return data;
}

void mask_shape_destroy(mask_shape_data_t *data)
{
	obs_enter_graphics();
	if (data->effect_rectangle_mask) {
		gs_effect_destroy(data->effect_rectangle_mask);
	}
	if (data->effect_circle_mask) {
		gs_effect_destroy(data->effect_circle_mask);
	}
	if (data->effect_ellipse_mask) {
		gs_effect_destroy(data->effect_ellipse_mask);
	}
	if (data->effect_polygon_mask) {
		gs_effect_destroy(data->effect_polygon_mask);
	}
	if (data->effect_star_mask) {
		gs_effect_destroy(data->effect_star_mask);
	}
	if (data->effect_heart_mask) {
		gs_effect_destroy(data->effect_heart_mask);
	}
	if (data->effect_super_mask) {
		gs_effect_destroy(data->effect_super_mask);
	}
	obs_leave_graphics();

	bfree(data);
}

static float mask_width(obs_data_t *settings)
{
	float num_sides = (float)obs_data_get_int(settings, "shape_num_sides");
	uint32_t mode = (uint32_t)obs_data_get_int(settings, "super_mode");

	switch ((uint32_t)obs_data_get_int(settings, "shape_type")) {
	case SHAPE_RECTANGLE:
		return (float)obs_data_get_double(settings, "rectangle_width");
	case SHAPE_CIRCLE:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       2.0f;
	case SHAPE_ELLIPSE:
		return (float)obs_data_get_double(settings, "shape_ellipse_a") *
		       2.0f;
	case SHAPE_POLYGON:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       (float)cos(M_PI / num_sides) * 2.0f;
	case SHAPE_STAR:
		return (float)obs_data_get_double(settings,
						  "shape_star_outer_radius") *
		       2.0f;
	case SHAPE_HEART:
		return (float)obs_data_get_double(settings, "heart_size");
	case SHAPE_SUPERFORMULA:
		return mode == SHAPE_SUPER_SQUIRCLE
			       ? (float)obs_data_get_double(
					 settings, "super_squircle_size")
			       : (float)obs_data_get_double(
					 settings, "super_ellipse_width");
	}
	return 0.0f;
}

static float mask_height(obs_data_t *settings)
{
	float num_sides = (float)obs_data_get_int(settings, "shape_num_sides");
	uint32_t mode = (uint32_t)obs_data_get_int(settings, "shape_mode");

	switch ((uint32_t)obs_data_get_int(settings, "shape_type")) {
	case SHAPE_RECTANGLE:
		return (float)obs_data_get_double(settings, "rectangle_height");
	case SHAPE_CIRCLE:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       2.0f;
	case SHAPE_ELLIPSE:
		return (float)obs_data_get_double(settings, "shape_ellipse_b") *
		       2.0f;
	case SHAPE_POLYGON:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       (float)cos(M_PI / num_sides) * 2.0f;
	case SHAPE_STAR:
		return (float)obs_data_get_double(settings,
						  "shape_star_outer_radius") *
		       2.0f;
	case SHAPE_HEART:
		return (float)obs_data_get_double(settings, "heart_size");
	case SHAPE_SUPERFORMULA:
		return mode == SHAPE_SUPER_SQUIRCLE
			       ? (float)obs_data_get_double(
					 settings, "super_squircle_size")
			       : (float)obs_data_get_double(
					 settings, "super_ellipse_height");
	}
	return 0.0f;
}

void mask_shape_update(mask_shape_data_t *data, base_filter_data_t *base,
		       obs_data_t *settings, int version)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t iWidth = obs_source_get_base_width(target);
	uint32_t iHeight = obs_source_get_base_height(target);

	data->mask_shape_type =
		(uint32_t)obs_data_get_int(settings, "shape_type");

	data->fWidth = (float)iWidth;
	data->fHeight = (float)iHeight;

	data->shape_relative = obs_data_get_bool(settings, "shape_relative");
	data->frame_check = obs_data_get_bool(settings, "shape_frame_check");
	data->invert_mask = obs_data_get_bool(settings, "shape_invert");
	data->mask_center.x =
		(float)obs_data_get_double(settings, "shape_center_x");
	data->mask_center.y =
		(float)obs_data_get_double(settings, "shape_center_y");
	data->global_position.x =
		(float)obs_data_get_double(settings, "position_x");
	data->global_position.y =
		(float)obs_data_get_double(settings, "position_y");
	if (version == 1) {
		data->global_scale =
			base->mask_effect == MASK_EFFECT_ALPHA
				? (float)obs_data_get_double(settings,
							     "position_scale")
				: 100.0f;
	} else {
		data->global_scale =
			base->mask_effect == MASK_EFFECT_ALPHA &&
					data->shape_relative
				? (float)obs_data_get_double(settings,
							     "position_scale")
				: 100.0f;
	}

	data->zoom =
		base->mask_effect == MASK_EFFECT_ALPHA
			? (float)obs_data_get_double(settings, "source_zoom")
			: 100.0f;
	data->scale_type = (uint32_t)obs_data_get_int(settings, "scale_type");

	if (data->shape_relative) {
		if (base->mask_effect == MASK_EFFECT_ALPHA &&
		    data->scale_type == MASK_SCALE_WIDTH) {
			uint32_t sfMode = (uint32_t)obs_data_get_int(
				settings, "super_mode");
			bool isSuperForumula =
				data->mask_shape_type == SHAPE_SUPERFORMULA &&
				sfMode == SHAPE_SUPER_SUPERFORMULA;
			float width = isSuperForumula ? data->fWidth
						      : mask_width(settings);
			data->global_scale =
				100.0f * data->global_scale / width;
		} else if (base->mask_effect == MASK_EFFECT_ALPHA &&
			   data->scale_type == MASK_SCALE_HEIGHT) {
			uint32_t sfMode = (uint32_t)obs_data_get_int(
				settings, "super_mode");
			bool isSuperForumula =
				data->mask_shape_type == SHAPE_SUPERFORMULA &&
				sfMode == SHAPE_SUPER_SUPERFORMULA;
			float height = isSuperForumula ? data->fHeight
						       : mask_height(settings);
			data->global_scale =
				100.0f * data->global_scale / height;
		}
	} else {
		data->global_scale = 100.0f;
	}

	data->corner_radius_type =
		(uint32_t)obs_data_get_int(settings, "rectangle_corner_type");
	if (data->corner_radius_type == MASK_CORNER_UNIFORM) {
		float r = (float)obs_data_get_double(settings,
						     "rectangle_corner_radius");
		vec4_set(&data->rectangle_corner_radius, r, r, r, r);
		data->rectangle_max_corner_radius = r;
	} else {
		vec4_set(&data->rectangle_corner_radius,
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_br"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_tr"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_bl"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_tl"));
		float max_radius = -1.0;
		for (uint32_t i = 0; i < 4; i++) {
			if (data->rectangle_corner_radius.ptr[i] > max_radius) {
				max_radius =
					data->rectangle_corner_radius.ptr[i];
			}
		}
		data->rectangle_max_corner_radius = max_radius;
	}

	const double rotation = obs_data_get_double(settings, "shape_rotation");
	data->rotation = (float)(rotation * M_PI / 180.0f);

	data->num_sides = (float)obs_data_get_int(settings, "shape_num_sides");
	data->theta = M_PI / data->num_sides;
	data->theta_s.x = (float)cos(data->theta);
	data->theta_s.y = (float)sin(data->theta);

	const float radius =
		data->mask_shape_type == SHAPE_CIRCLE
			? (float)obs_data_get_double(settings, "circle_radius")
			: (float)obs_data_get_double(settings,
						     "circle_radius") *
				  (float)cos(M_PI / data->num_sides);

	data->radius = radius * data->global_scale / 100.0f;

	data->ellipse.x =
		(float)obs_data_get_double(settings, "shape_ellipse_a");
	data->ellipse.y =
		(float)obs_data_get_double(settings, "shape_ellipse_b");
	int super_mode = (int)obs_data_get_int(settings, "super_mode");

	switch (super_mode) {
	case SHAPE_SUPER_SQUIRCLE:
		data->a = 0.5f *
			  (float)obs_data_get_double(settings,
						     "super_squircle_size") /
			  data->fHeight;
		data->b = 0.5f *
			  (float)obs_data_get_double(settings,
						     "super_squircle_size") /
			  data->fHeight;
		data->m = 4.0f;
		data->n1 = (float)obs_data_get_double(
			settings, "super_squircle_curvature");
		data->n2 = (float)obs_data_get_double(
			settings, "super_squircle_curvature");
		data->n3 = (float)obs_data_get_double(
			settings, "super_squircle_curvature");
		break;
	case SHAPE_SUPER_SUPERELLIPSE:
		data->a = 0.5f *
			  (float)obs_data_get_double(settings,
						     "super_ellipse_width") /
			  data->fHeight;
		data->b = 0.5f *
			  (float)obs_data_get_double(settings,
						     "super_ellipse_height") /
			  data->fHeight;
		data->m = 4.0f;
		data->n1 = (float)obs_data_get_double(
			settings, "super_ellipse_curvature");
		data->n2 = (float)obs_data_get_double(
			settings, "super_ellipse_curvature");
		data->n3 = (float)obs_data_get_double(
			settings, "super_ellipse_curvature");
		break;
	case SHAPE_SUPER_SUPERFORMULA:
		data->a = 0.5f *
			  (float)obs_data_get_double(settings, "super_width");
		data->b = 0.5f *
			  (float)obs_data_get_double(settings, "super_height");
		data->m = (float)obs_data_get_double(settings, "super_m");
		data->n1 = (float)obs_data_get_double(settings, "super_n1");
		data->n2 = (float)obs_data_get_double(settings, "super_n2");
		data->n3 = (float)obs_data_get_double(settings, "super_n3");
		break;
	}

	if (data->mask_shape_type == SHAPE_SUPERFORMULA) {
		float min_r = 1.e9f;
		for (float phi = 0.0f; phi < 6.2831853f; phi += 0.0062831853f) {
			float r = 1.0f /
				  powf(powf(fabsf(cosf(data->m * phi / 4.0f)) /
						    data->a,
					    data->n2) +
					       powf(fabsf(sinf(data->m * phi /
							       4.0f)) /
							    data->b,
						    data->n3),
				       1.0f / data->n1);
			min_r = r < min_r ? r : min_r;
		}
		min_r *= 0.95f;
		data->min_r = min_r;
	}

	float shape_corner_radius =
		(float)obs_data_get_double(settings, "shape_corner_radius");
	data->shape_corner_radius = shape_corner_radius < data->radius
					    ? shape_corner_radius
					    : data->radius;

	data->feather_amount =
		(uint32_t)obs_data_get_int(settings, "shape_feather_type") ==
				MASK_SHAPE_FEATHER_NONE
			? 0.0f
			: (float)obs_data_get_double(settings,
						     "shape_feather_amount");

	switch ((uint32_t)obs_data_get_int(settings, "shape_feather_type")) {
	case MASK_SHAPE_FEATHER_INNER:
		data->feather_shift = (float)data->feather_amount;
		break;
	case MASK_SHAPE_FEATHER_MIDDLE:
		data->feather_shift = (float)data->feather_amount / 2.0f;
		break;
	case MASK_SHAPE_FEATHER_OUTER:
	case MASK_SHAPE_FEATHER_NONE:
		data->feather_shift = 0.0f;
	}
	const float radius_shift =
		data->mask_shape_type == SHAPE_POLYGON
			? (data->feather_shift + data->shape_corner_radius)
			: data->feather_shift;
	data->radius -= radius_shift;

	data->rectangle_width =
		(float)obs_data_get_double(settings, "rectangle_width") / 2.0f *
			data->global_scale / 100.0f -
		data->feather_shift;
	data->rectangle_height =
		(float)obs_data_get_double(settings, "rectangle_height") /
			2.0f * data->global_scale / 100.0f -
		data->feather_shift;

	data->ellipse.x =
		(float)obs_data_get_double(settings, "shape_ellipse_a") / 2.0f *
			data->global_scale / 100.0f -
		data->feather_shift;
	data->ellipse.y =
		(float)obs_data_get_double(settings, "shape_ellipse_b") / 2.0f *
			data->global_scale / 100.0f -
		data->feather_shift;

	data->star_outer_radius =
		(float)obs_data_get_double(settings,
					   "shape_star_outer_radius") *
			data->global_scale / 100.0f -
		(data->feather_shift + data->star_corner_radius);
	float star_inner_radius =
		(float)obs_data_get_double(settings, "shape_star_inner_radius");
	float points =
		(float)obs_data_get_int(settings, "shape_star_num_points");
	float ratio = 1.0f - star_inner_radius / 100.0f;
	float m = 2.0f + ratio * (points - 2.0f);
	data->an = M_PI / points;
	data->en = M_PI / m;
	data->acs.x = (float)cos(data->an);
	data->acs.y = (float)sin(data->an);
	data->ecs.x = (float)cos(data->en);
	data->ecs.y = (float)sin(data->en);

	data->star_corner_radius =
		(float)obs_data_get_double(settings, "star_corner_radius");

	float heart_size = (float)obs_data_get_double(settings, "heart_size");
	data->heart_size = heart_size * data->global_scale / 100.0f -
			   (data->feather_shift + data->star_corner_radius);
}

void mask_shape_defaults(obs_data_t *settings, int version)
{
	// Version specific settings
	double position_scale = (version == 1 ? 120.0 : 100.0);

	obs_data_set_default_int(settings, "shape_type", SHAPE_RECTANGLE);
	obs_data_set_default_bool(settings, "shape_frame_check", false);
	obs_data_set_default_double(settings, "shape_center_x", -1.e9);
	obs_data_set_default_double(settings, "shape_center_y", -1.e9);
	obs_data_set_default_double(settings, "shape_rotation", 0.0);
	obs_data_set_default_double(settings, "rectangle_width", 800.0);
	obs_data_set_default_double(settings, "rectangle_height", 600.0);
	obs_data_set_default_double(settings, "position_x", -1.e9);
	obs_data_set_default_double(settings, "position_y", -1.e9);
	obs_data_set_default_double(settings, "position_scale", position_scale);
	obs_data_set_default_double(settings, "mask_source_filter_multiplier",
				    1.0);
	obs_data_set_default_double(settings, "source_zoom", 100.0);
	obs_data_set_default_bool(settings, "shape_relative", false);
	obs_data_set_default_int(settings, "shape_num_sides", 6);
	obs_data_set_default_double(settings, "shape_corner_radius", 0.0);
	obs_data_set_default_double(settings, "shape_ellipse_a", 800.0);
	obs_data_set_default_double(settings, "shape_ellipse_b", 600.0);
	obs_data_set_default_int(settings, "shape_star_num_points", 5);
	obs_data_set_default_double(settings, "shape_star_outer_radius", 400.0);
	obs_data_set_default_double(settings, "shape_star_inner_radius", 50.0);
	obs_data_set_default_double(settings, "star_corner_radius", 0.0);
	obs_data_set_default_double(settings, "heart_size", 800.0);
	obs_data_set_default_double(settings, "circle_radius", 400.0);
	obs_data_set_default_int(settings, "scale_type", MASK_SCALE_PERCENT);
	obs_data_set_default_int(settings, "super_mode", SHAPE_SUPER_SQUIRCLE);
	obs_data_set_default_double(settings, "super_squircle_size", 800.0);
	obs_data_set_default_double(settings, "super_squircle_curvature", 3.0);
	obs_data_set_default_double(settings, "super_ellipse_width", 800.0);
	obs_data_set_default_double(settings, "super_ellipse_height", 600.0);
	obs_data_set_default_double(settings, "super_ellipse_curvature", 4.0);
	obs_data_set_default_double(settings, "super_width", 0.5);
	obs_data_set_default_double(settings, "super_height", 1.9);
	obs_data_set_default_double(settings, "super_m", 16.0);
	obs_data_set_default_double(settings, "super_n1", 0.5);
	obs_data_set_default_double(settings, "super_n2", 0.5);
	obs_data_set_default_double(settings, "super_n3", 16.0);
}

void shape_mask_top_properties(obs_properties_t *props)
{
	obs_property_t *shape_type_list = obs_properties_add_list(
		props, "shape_type", obs_module_text("AdvancedMasks.Shape"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_RECTANGLE_LABEL),
				  SHAPE_RECTANGLE);
	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_CIRCLE_LABEL),
				  SHAPE_CIRCLE);
	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_ELLIPSE_LABEL),
				  SHAPE_ELLIPSE);
	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_POLYGON_LABEL),
				  SHAPE_POLYGON);
	obs_property_list_add_int(
		shape_type_list, obs_module_text(SHAPE_STAR_LABEL), SHAPE_STAR);
	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_HEART_LABEL),
				  SHAPE_HEART);
	obs_property_list_add_int(shape_type_list,
				  obs_module_text(SHAPE_SUPERFORMULA_LABEL),
				  SHAPE_SUPERFORMULA);

	obs_property_set_modified_callback(shape_type_list,
					   setting_shape_type_modified);

	obs_properties_add_bool(
		props, "shape_frame_check",
		obs_module_text("AdvancedMasks.Shape.FrameCheck"));
}

void shape_mask_bot_properties(obs_properties_t *props, obs_source_t *context,
			       mask_shape_data_t *data)
{
	shape_properties(props, context, data);
	obs_property_t *shape_relative = obs_properties_add_bool(
		props, "shape_relative",
		obs_module_text("AdvancedMasks.Shape.Relative"));

	obs_property_set_modified_callback(shape_relative,
					   setting_shape_relative_modified);
	scale_position_properties(props, context, data);
}

static void shape_properties(obs_properties_t *props, obs_source_t *context,
			     mask_shape_data_t *data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t *source_rect_mask_group = obs_properties_create();
	obs_property_t *p;

	obs_properties_t *mask_geometry_group = obs_properties_create();
	char label[255];

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Invert"));
	obs_properties_add_bool(mask_geometry_group, "shape_invert", label);

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Center.X"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_center_x", label, -2000.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Center.Y"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_center_y", label, -2000.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.RecenterButton"));
	p = obs_properties_add_button2(mask_geometry_group,
				       "recenter_mask_button", label,
				       recenter_button_clicked, context);

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Rotation"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_rotation", label, -360.0,
					    360.0, 1.0);
	obs_property_float_set_suffix(p, "deg");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.NumSides"));
	p = obs_properties_add_int_slider(mask_geometry_group,
					  "shape_num_sides", label, 3, 100, 1);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Rectangle.Width"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "rectangle_width", label, -2000.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Rectangle.Height"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "rectangle_height", label, -2000.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Circle.Radius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "circle_radius", label, 0.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Ellipse.Width"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_ellipse_a", label, 0.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Ellipse.Height"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_ellipse_b", label, 0.0,
					    6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Star.NumPoints"));
	p = obs_properties_add_int_slider(
		mask_geometry_group, "shape_star_num_points", label, 3, 100, 1);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Star.OuterRadius"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_star_outer_radius", label,
					    0.0, 2500.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Star.InnerRadius"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_star_inner_radius", label,
					    0.0, 100.0, 0.1);
	obs_property_float_set_suffix(p, "%");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.CornerRadius"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "shape_corner_radius", label, 0.0,
					    1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.CornerRadius"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "star_corner_radius", label, 0.0,
					    1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Heart.Size"));
	p = obs_properties_add_float_slider(mask_geometry_group, "heart_size",
					    label, 0.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.Mode"));
	p = obs_properties_add_list(mask_geometry_group, "super_mode", label,
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(p,
				  obs_module_text(SHAPE_SUPER_SQUIRCLE_LABEL),
				  SHAPE_SUPER_SQUIRCLE);
	obs_property_list_add_int(
		p, obs_module_text(SHAPE_SUPER_SUPERELLIPSE_LABEL),
		SHAPE_SUPER_SUPERELLIPSE);
	obs_property_list_add_int(
		p, obs_module_text(SHAPE_SUPER_SUPERFORMULA_LABEL),
		SHAPE_SUPER_SUPERFORMULA);

	obs_property_set_modified_callback(p, setting_super_mode_modified);

	obs_properties_add_text(mask_geometry_group, "super_info", SUPER_INFO,
				OBS_TEXT_INFO);

	// Squircle
	label_indent(label,
		     obs_module_text(
			     "AdvancedMasks.Shape.Superformula.SquircleSize"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "super_squircle_size", label, 0.0,
					    15000.0, 1.0);

	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text("AdvancedMasks.Shape.Superformula.Curvature"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "super_squircle_curvature", label,
					    0.0, 50.0, 0.001);

	label_indent(label,
		     obs_module_text(
			     "AdvancedMasks.Shape.Superformula.EllipseWidth"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "super_ellipse_width", label, 0.0,
					    15000.0, 1.0);

	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text(
			     "AdvancedMasks.Shape.Superformula.EllipseHeight"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "super_ellipse_height", label, 0.0,
					    15000.0, 1.0);

	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text("AdvancedMasks.Shape.Superformula.Curvature"));
	p = obs_properties_add_float_slider(mask_geometry_group,
					    "super_ellipse_curvature", label,
					    0.0, 50.0, 0.001);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.Width"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_width",
					    label, 0.0, 5.0, 0.001);

	label_indent(label, obs_module_text(
				    "AdvancedMasks.Shape.Superformula.Height"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_height",
					    label, 0.0, 5.0, 0.001);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.m"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_m",
					    label, 0.0, 50.0, 0.01);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.n1"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_n1",
					    label, 0.0, 50.0, 0.01);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.n2"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_n2",
					    label, 0.0, 50.0, 0.01);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Superformula.n3"));
	p = obs_properties_add_float_slider(mask_geometry_group, "super_n3",
					    label, 0.0, 50.0, 0.01);

	label_indent(label, obs_module_text("AdvancedMasks.Shape.SourceZoom"));
	p = obs_properties_add_float_slider(mask_geometry_group, "source_zoom",
					    label, 1.0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "%");

	obs_properties_add_group(
		source_rect_mask_group, "mask_geometry_group",
		obs_module_text("AdvancedMasks.Shape.MaskGeometry"),
		OBS_GROUP_NORMAL, mask_geometry_group);

	rectangle_corner_radius_properties(source_rect_mask_group);
	feather_properties(source_rect_mask_group);

	obs_properties_add_group(
		props, "rectangle_source_group",
		obs_module_text("AdvancedMasks.Shape.Rectangle.SourceGroup"),
		OBS_GROUP_NORMAL, source_rect_mask_group);
}

static void feather_properties(obs_properties_t *props)
{
	obs_property_t *p;
	obs_properties_t *shape_feather_group = obs_properties_create();

	char label[255];
	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Feather.Type"));
	obs_property_t *feather_type = obs_properties_add_list(
		shape_feather_group, "shape_feather_type", label,
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		feather_type, obs_module_text(MASK_SHAPE_FEATHER_NONE_LABEL),
		MASK_SHAPE_FEATHER_NONE);
	obs_property_list_add_int(
		feather_type, obs_module_text(MASK_SHAPE_FEATHER_INNER_LABEL),
		MASK_SHAPE_FEATHER_INNER);
	obs_property_list_add_int(
		feather_type, obs_module_text(MASK_SHAPE_FEATHER_MIDDLE_LABEL),
		MASK_SHAPE_FEATHER_MIDDLE);
	obs_property_list_add_int(
		feather_type, obs_module_text(MASK_SHAPE_FEATHER_OUTER_LABEL),
		MASK_SHAPE_FEATHER_OUTER);

	obs_property_set_modified_callback(feather_type,
					   setting_feather_type_modified);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Feather.Amount"));
	p = obs_properties_add_float_slider(shape_feather_group,
					    "shape_feather_amount", label, 0.0,
					    500.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "shape_feather_group",
		obs_module_text("AdvancedMasks.Shape.Feather.Group"),
		OBS_GROUP_NORMAL, shape_feather_group);
}

static void rectangle_corner_radius_properties(obs_properties_t *props)
{
	obs_property_t *p;
	obs_properties_t *corner_radius_group = obs_properties_create();

	char label[255];
	label_indent(
		label,
		obs_module_text("AdvancedMasks.Shape.Rectangle.CornerRadius"));
	obs_property_t *corner_type_list = obs_properties_add_list(
		corner_radius_group, "rectangle_corner_type", label,
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_UNIFORM_LABEL),
				  MASK_CORNER_UNIFORM);
	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_CUSTOM_LABEL),
				  MASK_CORNER_CUSTOM);

	obs_property_set_modified_callback(corner_type_list,
					   setting_corner_type_modified);

	label_indent(label, obs_module_text(""));
	p = obs_properties_add_float_slider(corner_radius_group,
					    "rectangle_corner_radius", label,
					    0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopLeft"));
	p = obs_properties_add_float_slider(corner_radius_group,
					    "rectangle_corner_radius_tl", label,
					    0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopRight"));
	p = obs_properties_add_float_slider(corner_radius_group,
					    "rectangle_corner_radius_tr", label,
					    0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomLeft"));
	p = obs_properties_add_float_slider(corner_radius_group,
					    "rectangle_corner_radius_bl", label,
					    0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomRight"));
	p = obs_properties_add_float_slider(corner_radius_group,
					    "rectangle_corner_radius_br", label,
					    0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "rectangle_rounded_corners_group",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.CustomGroup"),
		OBS_GROUP_NORMAL, corner_radius_group);
}

static void scale_position_properties(obs_properties_t *props,
				      obs_source_t *context,
				      mask_shape_data_t *data)
{
	obs_property_t *p;
	obs_properties_t *scale_position_group = obs_properties_create();

	p = obs_properties_add_float_slider(
		scale_position_group, "position_x",
		obs_module_text("AdvancedMasks.Shape.Position.X"), -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		scale_position_group, "position_y",
		obs_module_text("AdvancedMasks.Shape.Position.Y"), -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	obs_property_t *scale_type_list = obs_properties_add_list(
		scale_position_group, "scale_type",
		obs_module_text("AdvancedMasks.ScaleType"), OBS_COMBO_TYPE_LIST,
		OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(scale_type_list,
				  obs_module_text(MASK_SCALE_PERCENT_LABEL),
				  MASK_SCALE_PERCENT);
	obs_property_list_add_int(scale_type_list,
				  obs_module_text(MASK_SCALE_WIDTH_LABEL),
				  MASK_SCALE_WIDTH);
	obs_property_list_add_int(scale_type_list,
				  obs_module_text(MASK_SCALE_HEIGHT_LABEL),
				  MASK_SCALE_HEIGHT);

	obs_property_set_modified_callback2(scale_type_list,
					    setting_scale_type_modified, data);
	uint32_t type = MASK_SCALE_PERCENT;
	if (context) {
		obs_data_t *settings = obs_source_get_settings(context);
		type = (uint32_t)obs_data_get_int(settings, "scale_type");
		obs_data_release(settings);
	}

	float scale_max = type == MASK_SCALE_PERCENT ? 10000.0f : 1920.0f;

	p = obs_properties_add_float_slider(
		scale_position_group, "position_scale",
		obs_module_text("AdvancedMasks.Shape.Position.Scale"), 1.0,
		scale_max, 0.1);

	obs_property_float_set_suffix(p,
				      type == MASK_SCALE_PERCENT ? "%" : "px");

	obs_properties_add_group(
		props, "scale_position_group",
		obs_module_text("AdvancedMasks.Shape.ScalePosition"),
		OBS_GROUP_NORMAL, scale_position_group);
}

bool set_shape_settings_visibility(void *data, obs_properties_t *props,
				   obs_property_t *p, obs_data_t *settings)
{
	setting_feather_type_modified(props, p, settings);
	setting_shape_type_modified(props, p, settings);
	setting_shape_relative_modified(props, p, settings);
	setting_corner_type_modified(props, p, settings);
	setting_scale_type_modified(data, props, p, settings);
	return true;
}

static bool recenter_button_clicked(obs_properties_t *props,
				    obs_property_t *property, void *data)
{
	UNUSED_PARAMETER(props);
	UNUSED_PARAMETER(property);
	obs_source_t *source = (obs_source_t *)data;
	obs_data_t *settings = obs_source_get_settings(source);
	double width = (double)obs_source_get_width(source);
	double height = (double)obs_source_get_height(source);
	obs_data_set_double(settings, "shape_center_x", width / 2.0);
	obs_data_set_double(settings, "shape_center_y", height / 2.0);
	obs_source_update(source, settings);
	obs_data_release(settings);
	return true;
}

static bool setting_feather_type_modified(obs_properties_t *props,
					  obs_property_t *p,
					  obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	if ((uint32_t)obs_data_get_int(settings, "mask_type") !=
	    MASK_TYPE_SHAPE) {
		return false;
	}
	int feather_type =
		(int)obs_data_get_int(settings, "shape_feather_type");
	switch (feather_type) {
	case MASK_SHAPE_FEATHER_NONE:
		setting_visibility("shape_feather_amount", false, props);
		break;
	case MASK_SHAPE_FEATHER_INNER:
	case MASK_SHAPE_FEATHER_MIDDLE:
	case MASK_SHAPE_FEATHER_OUTER:
		setting_visibility("shape_feather_amount", true, props);
		break;
	}

	return true;
}

bool setting_shape_type_modified(obs_properties_t *props, obs_property_t *p,
				 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	uint32_t mask_type = (uint32_t)obs_data_get_int(settings, "mask_type");
	if (mask_type != MASK_TYPE_SHAPE) {
		return false;
	}
	uint32_t shape_type =
		(uint32_t)obs_data_get_int(settings, "shape_type");
	uint32_t effect_type =
		(uint32_t)obs_data_get_int(settings, "mask_effect");
	bool relative = obs_data_get_bool(settings, "shape_relative");
	switch (shape_type) {
	case SHAPE_RECTANGLE:
		setting_visibility("rectangle_width", true, props);
		setting_visibility("rectangle_height", true, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", false, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("rectangle_rounded_corners_group", true,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);

		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_CIRCLE:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", true, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", false, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", false, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_ELLIPSE:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("shape_ellipse_a", true, props);
		setting_visibility("shape_ellipse_b", true, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", false, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_POLYGON:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", true, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("shape_num_sides", true, props);
		setting_visibility("shape_corner_radius", true, props);
		setting_visibility("star_corner_radius", false, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_STAR:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", true, props);
		setting_visibility("shape_star_outer_radius", true, props);
		setting_visibility("shape_star_inner_radius", true, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", true, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_HEART:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", true, props);
		setting_visibility("heart_size", true, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", false, props);
		setting_visibility("super_info", false, props);
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		setting_visibility("shape_feather_group", true, props);
		break;
	case SHAPE_SUPERFORMULA:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("shape_ellipse_a", false, props);
		setting_visibility("shape_ellipse_b", false, props);
		setting_visibility("shape_star_num_points", false, props);
		setting_visibility("shape_star_outer_radius", false, props);
		setting_visibility("shape_star_inner_radius", false, props);
		setting_visibility("shape_corner_radius", false, props);
		setting_visibility("star_corner_radius", false, props);
		setting_visibility("heart_size", false, props);
		setting_visibility("shape_rotation", true, props);
		setting_visibility("shape_num_sides", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("super_mode", true, props);
		setting_visibility("super_info", true, props);
		setting_visibility("super_width", true, props);
		setting_visibility("super_height", true, props);
		setting_visibility("super_m", true, props);
		setting_visibility("super_n1", true, props);
		setting_visibility("super_n2", true, props);
		setting_visibility("super_n3", true, props);
		setting_super_mode_modified(props, p, settings);

		setting_visibility("shape_feather_group", false, props);

		break;
	}
	setting_visibility("source_zoom", effect_type == MASK_EFFECT_ALPHA,
			   props);
	obs_property_t *group =
		obs_properties_get(props, "rectangle_source_group");
	const char *group_name =
		effect_type == MASK_EFFECT_ALPHA && relative
			? obs_module_text(
				  "AdvancedMasks.Shape.Rectangle.SourceGroup")
		: effect_type == MASK_EFFECT_ALPHA
			? obs_module_text(
				  "AdvancedMasks.Shape.Rectangle.MaskGeometryGroup")
			: obs_module_text(
				  "AdvancedMasks.Shape.Rectangle.GeometryGroup");
	obs_property_set_description(group, group_name);
	setting_visibility("shape_relative", effect_type == MASK_EFFECT_ALPHA,
			   props);
	return true;
}

bool setting_super_mode_modified(obs_properties_t *props, obs_property_t *p,
				 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	uint32_t mask_type = (uint32_t)obs_data_get_int(settings, "mask_type");
	if (mask_type != MASK_TYPE_SHAPE) {
		return false;
	}
	uint32_t mode = (uint32_t)obs_data_get_int(settings, "super_mode");

	switch (mode) {
	case SHAPE_SUPER_SQUIRCLE:
		setting_visibility("super_squircle_size", true, props);
		setting_visibility("super_squircle_curvature", true, props);

		setting_visibility("super_ellipse_width", false, props);
		setting_visibility("super_ellipse_height", false, props);
		setting_visibility("super_ellipse_curvature", false, props);

		setting_visibility("super_info", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		break;
	case SHAPE_SUPER_SUPERELLIPSE:
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);

		setting_visibility("super_ellipse_width", true, props);
		setting_visibility("super_ellipse_height", true, props);
		setting_visibility("super_ellipse_curvature", true, props);

		setting_visibility("super_info", false, props);
		setting_visibility("super_width", false, props);
		setting_visibility("super_height", false, props);
		setting_visibility("super_m", false, props);
		setting_visibility("super_n1", false, props);
		setting_visibility("super_n2", false, props);
		setting_visibility("super_n3", false, props);
		break;
	case SHAPE_SUPER_SUPERFORMULA:
		setting_visibility("super_squircle_size", false, props);
		setting_visibility("super_squircle_curvature", false, props);

		setting_visibility("super_ellipse_width", false, props);
		setting_visibility("super_ellipse_height", false, props);
		setting_visibility("super_ellipse_curvature", false, props);

		setting_visibility("super_info", true, props);
		setting_visibility("super_width", true, props);
		setting_visibility("super_height", true, props);
		setting_visibility("super_m", true, props);
		setting_visibility("super_n1", true, props);
		setting_visibility("super_n2", true, props);
		setting_visibility("super_n3", true, props);
		break;
	}

	return true;
}

bool setting_shape_relative_modified(obs_properties_t *props, obs_property_t *p,
				     obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	uint32_t mask_type = (uint32_t)obs_data_get_int(settings, "mask_type");
	if (mask_type != MASK_TYPE_SHAPE &&
	    mask_type != MASK_TYPE_FONT_AWESOME) {
		return false;
	}
	bool relative = obs_data_get_bool(settings, "shape_relative");
	uint32_t mask_effect =
		(uint32_t)obs_data_get_int(settings, "mask_effect");
	if (relative && mask_effect == MASK_EFFECT_ALPHA) {
		setting_visibility("scale_position_group", true, props);
	} else {
		setting_visibility("scale_position_group", false, props);
	}
	setting_shape_type_modified(props, p, settings);
	return true;
}

static bool setting_corner_type_modified(obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	if ((uint32_t)obs_data_get_int(settings, "mask_type") !=
	    MASK_TYPE_SHAPE) {
		return false;
	}
	int corner_type =
		(int)obs_data_get_int(settings, "rectangle_corner_type");
	switch (corner_type) {
	case MASK_CORNER_UNIFORM:
		setting_visibility("rectangle_corner_radius", true, props);
		setting_visibility("rectangle_corner_radius_tl", false, props);
		setting_visibility("rectangle_corner_radius_tr", false, props);
		setting_visibility("rectangle_corner_radius_bl", false, props);
		setting_visibility("rectangle_corner_radius_br", false, props);
		return true;
	case MASK_CORNER_CUSTOM:
		setting_visibility("rectangle_corner_radius", false, props);
		setting_visibility("rectangle_corner_radius_tl", true, props);
		setting_visibility("rectangle_corner_radius_tr", true, props);
		setting_visibility("rectangle_corner_radius_bl", true, props);
		setting_visibility("rectangle_corner_radius_br", true, props);
		return true;
	}
	return false;
}

static bool setting_scale_type_modified(void *data, obs_properties_t *props,
					obs_property_t *p, obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	if ((uint32_t)obs_data_get_int(settings, "mask_type") !=
	    MASK_TYPE_SHAPE) {
		return false;
	}
	mask_shape_data_t *filter = data;
	uint32_t type = (uint32_t)obs_data_get_int(settings, "scale_type");
	uint32_t last_type = filter->last_scale_type;

	uint32_t sfMode = (uint32_t)obs_data_get_int(settings, "super_mode");
	bool isSuperForumula = filter->mask_shape_type == SHAPE_SUPERFORMULA &&
			       sfMode == SHAPE_SUPER_SUPERFORMULA;
	float width = filter->mask_shape_type == isSuperForumula
			      ? filter->fWidth
			      : mask_width(settings);
	float height = filter->mask_shape_type == isSuperForumula
			       ? filter->fHeight
			       : mask_height(settings);
	float pct = filter->last_scale_type == MASK_SCALE_WIDTH
			    ? (float)obs_data_get_double(settings,
							 "position_scale") /
				      width
		    : filter->last_scale_type == MASK_SCALE_HEIGHT
			    ? (float)obs_data_get_double(settings,
							 "position_scale") /
				      height
			    : (float)obs_data_get_double(settings,
							 "position_scale") /
				      100.0f;

	if (last_type == 0 || type == last_type) {
		filter->last_scale_type = type;
		return false;
	}
	if (type == MASK_SCALE_WIDTH) {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");

		obs_property_float_set_limits(scale_p, (double)0.0,
					      100.0 * (double)width,
					      (double)1.0);
		obs_data_set_double(settings, "position_scale",
				    (double)width * pct);
		obs_property_float_set_suffix(scale_p, "px");
	} else if (type == MASK_SCALE_HEIGHT) {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      100.0 * (double)height,
					      (double)1.0);
		obs_data_set_double(settings, "position_scale", height * pct);
		obs_property_float_set_suffix(scale_p, "px");
	} else {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      (double)10000.0, (double)0.1);
		obs_data_set_double(settings, "position_scale", pct * 100.0f);
		obs_property_float_set_suffix(scale_p, "%");
	}
	filter->last_scale_type = type;
	return true;
}

void render_shape_mask(mask_shape_data_t *data, base_filter_data_t *base,
		       color_adjustments_data_t *color_adj,
		       mask_shape_data_t *data_to,
		       color_adjustments_data_t *color_adj_to, float f)
{
	if (data_to && (data->mask_shape_type != data_to->mask_shape_type ||
			data->invert_mask != data_to->invert_mask)) {
		if (f > 0.5f) {
			f = (f - 0.5f) * 2.0f;
		} else {
			f *= 2.0f;
		}
		data_to = NULL;
	}
	switch (data->mask_shape_type) {
	case SHAPE_RECTANGLE:
		render_rectangle_mask(data, base, color_adj, data_to,
				      color_adj_to, f);
		break;
	case SHAPE_CIRCLE:
		render_circle_mask(data, base, color_adj);
		break;
	case SHAPE_POLYGON:
		render_polygon_mask(data, base, color_adj);
		break;
	case SHAPE_ELLIPSE:
		if (fabs(data->ellipse.x - data->ellipse.y) > 0.01) {
			render_ellipse_mask(data, base, color_adj);
		} else {
			float tmp = data->radius;
			data->radius = data->ellipse.x;
			render_circle_mask(data, base, color_adj);
			data->radius = tmp;
		}
		break;
	case SHAPE_STAR:
		render_star_mask(data, base, color_adj);
		break;
	case SHAPE_HEART:
		render_heart_mask(data, base, color_adj);
		break;
	case SHAPE_SUPERFORMULA:
		render_superfunction_mask(data, base, color_adj);
		break;
	}
}

static void render_rectangle_mask(mask_shape_data_t *data,
				  base_filter_data_t *base,
				  color_adjustments_data_t *color_adj,
				  mask_shape_data_t *data_to,
				  color_adjustments_data_t *color_adj_to,
				  float f)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {
		if (f != 0.0f) {
			if (data_to) {
				float scale_factor =
					(data->shape_relative
						 ? data->global_scale / 100.0f
						 : 1.0f) *
						(1.0f - f) +
					(data_to->shape_relative
						 ? data_to->global_scale /
							   100.0f
						 : 1.0f) *
						f;

				gs_effect_set_float(
					data->param_rectangle_zoom,
					data->zoom / 100.0f * (1.0f - f) +
						data_to->zoom / 100.f * f);
				struct vec2 mask_center;
				mask_center.x =
					data->mask_center.x * (1.0f - f) +
					data_to->mask_center.x * f;
				mask_center.y =
					data->mask_center.y * (1.0f - f) +
					data_to->mask_center.y * f;
				gs_effect_set_vec2(
					data->param_rectangle_mask_position,
					&mask_center);
				gs_effect_set_float(
					data->param_rectangle_width,
					data->rectangle_width * (1.0f - f) +
						data_to->rectangle_width * f);
				gs_effect_set_float(
					data->param_rectangle_alpha_zero,
					(data->frame_check ? 0.3f : 0.0f) *
							(1.0f - f) +
						(data_to->frame_check ? 0.3f
								      : 0.0f) *
							f);
				gs_effect_set_float(
					data->param_rectangle_height,
					data->rectangle_height * (1.0f - f) +
						data_to->rectangle_height * f);
				gs_effect_set_float(
					data->param_rectangle_sin_theta,
					sinf(data->rotation) * (1.0f - f) +
						sinf(data_to->rotation) * f);
				gs_effect_set_float(
					data->param_rectangle_cos_theta,
					cosf(data->rotation) * (1.0f - f) +
						cosf(data_to->rotation) * f);
				gs_effect_set_float(
					data->param_rectangle_feather_amount,
					data->feather_amount * (1.0f - f) +
						data_to->feather_amount * f);
				gs_effect_set_float(
					data->param_rectangle_feather_shift,
					data->feather_shift * (1.0f - f) +
						data_to->feather_shift * f);
				struct vec2 global_position;
				if (data->shape_relative) {
					global_position.x =
						data->global_position.x *
						(1.0f - f);
					global_position.y =
						data->global_position.y *
						(1.0f - f);
				} else {
					global_position.x =
						data->mask_center.x *
						(1.0f - f);
					global_position.y =
						data->mask_center.y *
						(1.0f - f);
				}
				if (data_to->shape_relative) {
					global_position.x +=
						data_to->global_position.x * f;
					global_position.y +=
						data_to->global_position.y * f;
				} else {
					global_position.x +=
						data_to->mask_center.x * f;
					global_position.y +=
						data_to->mask_center.y * f;
				}
				gs_effect_set_vec2(data->param_global_position,
						   &global_position);

				gs_effect_set_float(data->param_global_scale,
						    scale_factor);

				struct vec4 rectangle_corner_radius;
				rectangle_corner_radius.x =
					data->rectangle_corner_radius.x *
						(1.0f - f) +
					data_to->rectangle_corner_radius.x * f;
				rectangle_corner_radius.y =
					data->rectangle_corner_radius.y *
						(1.0f - f) +
					data_to->rectangle_corner_radius.y * f;
				rectangle_corner_radius.z =
					data->rectangle_corner_radius.z *
						(1.0f - f) +
					data_to->rectangle_corner_radius.z * f;
				rectangle_corner_radius.w =
					data->rectangle_corner_radius.w *
						(1.0f - f) +
					data_to->rectangle_corner_radius.w * f;
				gs_effect_set_vec4(data->param_corner_radius,
						   &rectangle_corner_radius);

				float max_corner_radius =
					(data->rectangle_max_corner_radius /
					 scale_factor * (data->zoom / 100.0f)) *
						(1.0f - f) +
					(data_to->rectangle_max_corner_radius /
					 scale_factor *
					 (data_to->zoom / 100.0f)) *
						f;
				gs_effect_set_float(
					data->param_max_corner_radius,
					max_corner_radius);
			} else {
				float scale_factor =
					(data->shape_relative
						 ? data->global_scale / 100.0f
						 : 1.0f) *
						(1.0f - f) +
					f;

				gs_effect_set_float(
					data->param_rectangle_zoom,
					data->zoom / 100.0f * (1.0f - f) + f);
				struct vec2 mask_center;
				mask_center.x =
					data->mask_center.x * (1.0f - f) +
					base->width / 2.0f * f;
				mask_center.y =
					data->mask_center.y * (1.0f - f) +
					base->height / 2.0f * f;
				gs_effect_set_vec2(
					data->param_rectangle_mask_position,
					&mask_center);
				gs_effect_set_float(data->param_rectangle_width,
						    data->rectangle_width *
								    (1.0f - f) +
							    base->width * f);
				gs_effect_set_float(
					data->param_rectangle_alpha_zero,
					(data->frame_check ? 0.3f : 0.0f) *
						(1.0f - f));
				gs_effect_set_float(
					data->param_rectangle_height,
					data->rectangle_height * (1.0f - f) +
						base->height * f);
				gs_effect_set_float(
					data->param_rectangle_sin_theta,
					sinf(data->rotation) * (1.0f - f));
				gs_effect_set_float(
					data->param_rectangle_cos_theta,
					cosf(data->rotation) * (1.0f - f) + f);
				gs_effect_set_float(
					data->param_rectangle_feather_amount,
					data->feather_amount * (1.0f - f));
				gs_effect_set_float(
					data->param_rectangle_feather_shift,
					data->feather_shift * (1.0f - f));
				struct vec2 global_position;
				if (data->shape_relative) {
					global_position.x =
						data->global_position.x *
						(1.0f - f);
					global_position.y =
						data->global_position.y *
						(1.0f - f);
				} else {
					global_position.x =
						data->mask_center.x *
							(1.0f - f) +
						base->width / 2.0f * f;
					global_position.y =
						data->mask_center.y *
							(1.0f - f) +
						base->height / 2.0f * f;
				}
				gs_effect_set_vec2(data->param_global_position,
						   &global_position);

				gs_effect_set_float(data->param_global_scale,
						    scale_factor);

				struct vec4 rectangle_corner_radius;
				rectangle_corner_radius.x =
					data->rectangle_corner_radius.x *
					(1.0f - f);
				rectangle_corner_radius.y =
					data->rectangle_corner_radius.y *
					(1.0f - f);
				rectangle_corner_radius.z =
					data->rectangle_corner_radius.z *
					(1.0f - f);
				rectangle_corner_radius.w =
					data->rectangle_corner_radius.w *
					(1.0f - f);
				gs_effect_set_vec4(data->param_corner_radius,
						   &rectangle_corner_radius);

				float max_corner_radius =
					(data->rectangle_max_corner_radius /
					 scale_factor * (data->zoom / 100.0f)) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_max_corner_radius,
					max_corner_radius);
			}
			if (color_adj_to) {
				const float min_brightness =
					(color_adj->adj_brightness
						 ? color_adj->min_brightness
						 : 0.0f) *
						(1.0f - f) +
					(color_adj_to->adj_brightness
						 ? color_adj_to->min_brightness
						 : 0.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_min_brightness,
					min_brightness);

				const float max_brightness =
					(color_adj->adj_brightness
						 ? color_adj->max_brightness
						 : 0.0f) *
						(1.0f - f) +
					(color_adj_to->adj_brightness
						 ? color_adj_to->max_brightness
						 : 0.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_max_brightness,
					max_brightness);

				const float min_contrast =
					(color_adj->adj_contrast
						 ? color_adj->min_contrast
						 : 0.0f) *
						(1.0f - f) +
					(color_adj_to->adj_contrast
						 ? color_adj_to->min_contrast
						 : 0.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_min_contrast,
					min_contrast);

				const float max_contrast =
					(color_adj->adj_contrast
						 ? color_adj->max_contrast
						 : 0.0f) *
						(1.0f - f) +
					(color_adj_to->adj_contrast
						 ? color_adj_to->max_contrast
						 : 0.0f);
				gs_effect_set_float(
					data->param_rectangle_max_contrast,
					max_contrast);

				const float min_saturation =
					(color_adj->adj_saturation
						 ? color_adj->min_saturation
						 : 1.0f) *
						(1.0f - f) +
					(color_adj_to->adj_saturation
						 ? color_adj_to->min_saturation
						 : 1.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_min_saturation,
					min_saturation);

				const float max_saturation =
					(color_adj->adj_saturation
						 ? color_adj->max_saturation
						 : 1.0f) *
						(1.0f - f) +
					(color_adj_to->adj_saturation
						 ? color_adj_to->max_saturation
						 : 1.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_max_saturation,
					max_saturation);

				const float min_hue_shift =
					(color_adj->adj_hue_shift
						 ? color_adj->min_hue_shift
						 : 0.0f) *
						(1.0f - f) +
					(color_adj_to->adj_hue_shift
						 ? color_adj_to->min_hue_shift
						 : 0.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_min_hue_shift,
					min_hue_shift);

				const float max_hue_shift =
					(color_adj->adj_hue_shift
						 ? color_adj->max_hue_shift
						 : 1.0f) *
						(1.0f - f) +
					(color_adj_to->adj_hue_shift
						 ? color_adj_to->max_hue_shift
						 : 1.0f) *
						f;
				gs_effect_set_float(
					data->param_rectangle_max_hue_shift,
					max_hue_shift);
			} else {
				const float min_brightness =
					(color_adj->adj_brightness
						 ? color_adj->min_brightness
						 : 0.0f) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_rectangle_min_brightness,
					min_brightness);

				const float max_brightness =
					(color_adj->adj_brightness
						 ? color_adj->max_brightness
						 : 0.0f) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_rectangle_max_brightness,
					max_brightness);

				const float min_contrast =
					(color_adj->adj_contrast
						 ? color_adj->min_contrast
						 : 0.0f) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_rectangle_min_contrast,
					min_contrast);

				const float max_contrast =
					(color_adj->adj_contrast
						 ? color_adj->max_contrast
						 : 0.0f) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_rectangle_max_contrast,
					max_contrast);

				const float min_saturation =
					(color_adj->adj_saturation
						 ? color_adj->min_saturation
						 : 1.0f) *
						(1.0f - f) +
					f;
				gs_effect_set_float(
					data->param_rectangle_min_saturation,
					min_saturation);

				const float max_saturation =
					(color_adj->adj_saturation
						 ? color_adj->max_saturation
						 : 1.0f) *
						(1.0f - f) +
					f;
				gs_effect_set_float(
					data->param_rectangle_max_saturation,
					max_saturation);

				const float min_hue_shift =
					(color_adj->adj_hue_shift
						 ? color_adj->min_hue_shift
						 : 0.0f) *
					(1.0f - f);
				gs_effect_set_float(
					data->param_rectangle_min_hue_shift,
					min_hue_shift);

				const float max_hue_shift =
					(color_adj->adj_hue_shift
						 ? color_adj->max_hue_shift
						 : 1.0f) *
						(1.0f - f) +
					f;
				gs_effect_set_float(
					data->param_rectangle_max_hue_shift,
					max_hue_shift);
			}
		} else {
			float scale_factor =
				data->shape_relative
					? data->global_scale / 100.0f
					: 1.0f;
			gs_effect_set_float(data->param_rectangle_zoom,
					    data->zoom / 100.0f);
			gs_effect_set_vec2(data->param_rectangle_mask_position,
					   &data->mask_center);
			gs_effect_set_float(data->param_rectangle_width,
					    data->rectangle_width);
			gs_effect_set_float(data->param_rectangle_alpha_zero,
					    data->frame_check ? 0.3f : 0.0f);
			gs_effect_set_float(data->param_rectangle_height,
					    data->rectangle_height);
			gs_effect_set_float(data->param_rectangle_sin_theta,
					    (float)sin(data->rotation));
			gs_effect_set_float(data->param_rectangle_cos_theta,
					    (float)cos(data->rotation));
			gs_effect_set_float(
				data->param_rectangle_feather_amount,
				data->feather_amount);
			gs_effect_set_float(data->param_rectangle_feather_shift,
					    data->feather_shift);

			if (data->shape_relative) {
				gs_effect_set_vec2(data->param_global_position,
						   &data->global_position);
			} else {
				gs_effect_set_vec2(data->param_global_position,
						   &data->mask_center);
			}

			gs_effect_set_float(data->param_global_scale,
					    scale_factor);
			gs_effect_set_vec4(data->param_corner_radius,
					   &data->rectangle_corner_radius);

			float max_corner_radius =
				data->rectangle_max_corner_radius /
				scale_factor * (data->zoom / 100.0f);
			gs_effect_set_float(data->param_max_corner_radius,
					    max_corner_radius);

			const float min_brightness =
				color_adj->adj_brightness
					? color_adj->min_brightness
					: 0.0f;
			gs_effect_set_float(
				data->param_rectangle_min_brightness,
				min_brightness);

			const float max_brightness =
				color_adj->adj_brightness
					? color_adj->max_brightness
					: 0.0f;
			gs_effect_set_float(
				data->param_rectangle_max_brightness,
				max_brightness);

			const float min_contrast =
				color_adj->adj_contrast
					? color_adj->min_contrast
					: 0.0f;
			gs_effect_set_float(data->param_rectangle_min_contrast,
					    min_contrast);

			const float max_contrast =
				color_adj->adj_contrast
					? color_adj->max_contrast
					: 0.0f;
			gs_effect_set_float(data->param_rectangle_max_contrast,
					    max_contrast);

			const float min_saturation =
				color_adj->adj_saturation
					? color_adj->min_saturation
					: 1.0f;
			gs_effect_set_float(
				data->param_rectangle_min_saturation,
				min_saturation);

			const float max_saturation =
				color_adj->adj_saturation
					? color_adj->max_saturation
					: 1.0f;
			gs_effect_set_float(
				data->param_rectangle_max_saturation,
				max_saturation);

			const float min_hue_shift =
				color_adj->adj_hue_shift
					? color_adj->min_hue_shift
					: 0.0f;
			gs_effect_set_float(data->param_rectangle_min_hue_shift,
					    min_hue_shift);

			const float max_hue_shift =
				color_adj->adj_hue_shift
					? color_adj->max_hue_shift
					: 1.0f;
			gs_effect_set_float(data->param_rectangle_max_hue_shift,
					    max_hue_shift);
		}

		gs_effect_set_float(data->param_rectangle_invert,
				    data->invert_mask ? 1.0f : 0.0f);

		struct vec2 box_ar;
		box_ar.x =
			(float)base->width /
			(float)fmin((double)base->width, (double)base->height);
		box_ar.y =
			(float)base->height /
			(float)fmin((double)base->width, (double)base->height);

		gs_effect_set_vec2(data->param_rect_aspect_ratio, &box_ar);

		float aa_scale = 5.0f / (float)base->height;
		gs_effect_set_float(data->param_rectangle_aa_scale, aa_scale);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_rectangle_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_rectangle_mask,
						   0, 0, technique);
		gs_blend_state_pop();
	}
}

static void render_polygon_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_polygon_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_polygon_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_vec2(data->param_polygon_mask_position,
				   &data->mask_center);
		gs_effect_set_float(data->param_polygon_sin_rot,
				    (float)sin(data->rotation));
		gs_effect_set_float(data->param_polygon_cos_rot,
				    (float)cos(data->rotation));
		gs_effect_set_float(data->param_polygon_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_float(data->param_polygon_theta, data->theta);
		gs_effect_set_float(data->param_polygon_theta_2,
				    2.0f * data->theta);
		gs_effect_set_vec2(data->param_polygon_theta_s, &data->theta_s);
		gs_effect_set_float(data->param_polygon_corner_radius,
				    data->shape_corner_radius);
		gs_effect_set_float(data->param_polygon_feather_amount,
				    data->feather_amount);
		gs_effect_set_float(data->param_polygon_radius, data->radius);
		gs_effect_set_float(data->param_polygon_num_sides,
				    data->num_sides);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_polygon_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_polygon_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_polygon_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_polygon_min_brightness,
				    min_brightness);

		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_polygon_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_polygon_min_contrast,
				    min_contrast);

		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_polygon_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_polygon_min_saturation,
				    min_saturation);

		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_polygon_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_polygon_min_hue_shift,
				    min_hue_shift);

		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_polygon_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_polygon_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_polygon_mask, 0,
						   0, technique);
		gs_blend_state_pop();
	}
}

static void render_star_mask(mask_shape_data_t *data, base_filter_data_t *base,
			     color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_star_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_star_zoom, data->zoom / 100.0f);
		gs_effect_set_vec2(data->param_star_mask_position,
				   &data->mask_center);
		gs_effect_set_float(data->param_star_sin_rot,
				    (float)sin(data->rotation));
		gs_effect_set_float(data->param_star_cos_rot,
				    (float)cos(data->rotation));
		gs_effect_set_float(data->param_star_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_float(data->param_star_corner_radius,
				    data->star_corner_radius);
		gs_effect_set_float(data->param_star_an, data->an);
		gs_effect_set_float(data->param_star_en, data->en);
		gs_effect_set_vec2(data->param_star_acs, &data->acs);
		gs_effect_set_vec2(data->param_star_ecs, &data->ecs);
		gs_effect_set_float(data->param_star_feather_amount,
				    data->feather_amount);
		gs_effect_set_float(data->param_star_radius,
				    data->star_outer_radius);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_star_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_star_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_star_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_star_min_brightness,
				    min_brightness);
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_star_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_star_min_contrast,
				    min_contrast);
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_star_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_star_min_saturation,
				    min_saturation);
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_star_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_star_min_hue_shift,
				    min_hue_shift);
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_star_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_star_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(
			base->context, data->effect_star_mask, 0, 0, technique);
		gs_blend_state_pop();
	}
}

static void render_circle_mask(mask_shape_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_circle_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_circle_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_float(data->param_circle_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_vec2(data->param_circle_mask_position,
				   &data->mask_center);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_circle_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_circle_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_circle_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
		gs_effect_set_float(data->param_circle_radius, data->radius);
		gs_effect_set_float(data->param_circle_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_float(data->param_circle_feather_amount,
				    data->feather_amount);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_circle_min_brightness,
				    min_brightness);
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_circle_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_circle_min_contrast,
				    min_contrast);
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_circle_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_circle_min_saturation,
				    min_saturation);
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_circle_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_circle_min_hue_shift,
				    min_hue_shift);
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_circle_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_circle_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_circle_mask, 0,
						   0, technique);
		gs_blend_state_pop();
	}
}

static void render_heart_mask(mask_shape_data_t *data, base_filter_data_t *base,
			      color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_heart_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_heart_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_float(data->param_heart_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_vec2(data->param_heart_mask_position,
				   &data->mask_center);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_heart_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_heart_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_heart_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
		gs_effect_set_float(data->param_heart_size, data->heart_size);
		gs_effect_set_float(data->param_heart_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_float(data->param_heart_sin_rot,
				    (float)sin(data->rotation));
		gs_effect_set_float(data->param_heart_cos_rot,
				    (float)cos(data->rotation));
		gs_effect_set_float(data->param_heart_corner_radius,
				    data->star_corner_radius);
		gs_effect_set_float(data->param_heart_feather_amount,
				    data->feather_amount);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_heart_min_brightness,
				    min_brightness);
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_heart_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_heart_min_contrast,
				    min_contrast);
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_heart_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_heart_min_saturation,
				    min_saturation);
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_heart_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_heart_min_hue_shift,
				    min_hue_shift);
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_heart_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_heart_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_heart_mask, 0,
						   0, technique);
		gs_blend_state_pop();
	}
}

static void render_ellipse_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_ellipse_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_ellipse_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_vec2(data->param_ellipse_mask_position,
				   &data->mask_center);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_ellipse_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_ellipse_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_ellipse_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
		gs_effect_set_float(data->param_ellipse_sin_rot,
				    (float)sin(data->rotation));
		gs_effect_set_float(data->param_ellipse_cos_rot,
				    (float)cos(data->rotation));
		gs_effect_set_float(data->param_ellipse_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_vec2(data->param_ellipse_ellipse, &data->ellipse);
		gs_effect_set_float(data->param_ellipse_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_float(data->param_ellipse_feather_amount,
				    data->feather_amount);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_brightness,
				    min_brightness);
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_ellipse_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_contrast,
				    min_contrast);
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_ellipse_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_ellipse_min_saturation,
				    min_saturation);
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_ellipse_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_hue_shift,
				    min_hue_shift);
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_ellipse_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_ellipse_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_ellipse_mask, 0,
						   0, technique);
		gs_blend_state_pop();
	}
}

static void render_superfunction_mask(mask_shape_data_t *data,
				      base_filter_data_t *base,
				      color_adjustments_data_t *color_adj)
{
	obs_source_t *target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(base->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";
	const enum gs_color_format format =
		gs_get_format_from_space(source_space);
	if (obs_source_process_filter_begin_with_color_space(
		    base->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {

		float scale_factor = data->global_scale / 100.0f;

		gs_effect_set_float(data->param_super_invert,
				    data->invert_mask ? 1.0f : 0.0f);
		gs_effect_set_float(data->param_super_zoom,
				    data->zoom / 100.0f);
		gs_effect_set_vec2(data->param_super_mask_position,
				   &data->mask_center);
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_super_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_super_global_position,
					   &data->mask_center);
		}
		gs_effect_set_float(data->param_super_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
		gs_effect_set_float(data->param_super_sin_theta,
				    (float)sin(data->rotation));
		gs_effect_set_float(data->param_super_cos_theta,
				    (float)cos(data->rotation));
		gs_effect_set_float(data->param_super_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
		gs_effect_set_float(data->param_super_zoom,
				    data->zoom / 100.0f);

		gs_effect_set_float(data->param_super_a, data->a);
		gs_effect_set_float(data->param_super_b, data->b);
		gs_effect_set_float(data->param_super_m, data->m);
		gs_effect_set_float(data->param_super_n1, data->n1);
		gs_effect_set_float(data->param_super_n2, data->n2);
		gs_effect_set_float(data->param_super_n3, data->n3);
		gs_effect_set_float(data->param_super_min_r, data->min_r);

		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_super_min_brightness,
				    min_brightness);
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_super_max_brightness,
				    max_brightness);

		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_super_min_contrast,
				    min_contrast);
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_super_max_contrast,
				    max_contrast);

		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_super_min_saturation,
				    min_saturation);
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_super_max_saturation,
				    max_saturation);

		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_super_min_hue_shift,
				    min_hue_shift);
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_super_max_hue_shift,
				    max_hue_shift);

		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_super_uv_size, &uv_size);

		gs_blend_state_push();
		gs_blend_function_separate(GS_BLEND_SRCALPHA,
					   GS_BLEND_INVSRCALPHA, GS_BLEND_ONE,
					   GS_BLEND_INVSRCALPHA);
		obs_source_process_filter_tech_end(base->context,
						   data->effect_super_mask, 0,
						   0, technique);
		gs_blend_state_pop();
	}
}

static void load_shape_effect_files(mask_shape_data_t *data)
{
	load_rectangle_mask_effect(data);
	load_circle_mask_effect(data);
	load_polygon_mask_effect(data);
	load_ellipse_mask_effect(data);
	load_star_mask_effect(data);
	load_heart_mask_effect(data);
	load_super_mask_effect(data);
}

static void load_rectangle_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/rectangular-mask.effect";

	data->effect_rectangle_mask = load_shader_effect(
		data->effect_rectangle_mask, effect_file_path);
	if (data->effect_rectangle_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_rectangle_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_rectangle_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_rectangle_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_rectangle_uv_size = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_rectangle_mask_position = param;
			} else if (strcmp(info.name, "width") == 0) {
				data->param_rectangle_width = param;
			} else if (strcmp(info.name, "height") == 0) {
				data->param_rectangle_height = param;
			} else if (strcmp(info.name, "sin_theta") == 0) {
				data->param_rectangle_sin_theta = param;
			} else if (strcmp(info.name, "cos_theta") == 0) {
				data->param_rectangle_cos_theta = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_rectangle_alpha_zero = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_rectangle_feather_amount = param;
			} else if (strcmp(info.name, "feather_shift") == 0) {
				data->param_rectangle_feather_shift = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_global_scale = param;
			} else if (strcmp(info.name, "corner_radius") == 0) {
				data->param_corner_radius = param;
			} else if (strcmp(info.name, "max_corner_radius") ==
				   0) {
				data->param_max_corner_radius = param;
			} else if (strcmp(info.name, "aspect_ratio") == 0) {
				data->param_rect_aspect_ratio = param;
			} else if (strcmp(info.name, "aa_scale") == 0) {
				data->param_rectangle_aa_scale = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_rectangle_zoom = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_rectangle_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_rectangle_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_rectangle_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_rectangle_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_rectangle_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_rectangle_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_rectangle_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_rectangle_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_rectangle_invert = param;
			}
		}
	}
}

static void load_circle_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/circle-mask.effect";

	data->effect_circle_mask =
		load_shader_effect(data->effect_circle_mask, effect_file_path);
	if (data->effect_circle_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_circle_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_circle_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_circle_image = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_circle_mask_position = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_circle_uv_size = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_circle_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_circle_global_scale = param;
			} else if (strcmp(info.name, "radius") == 0) {
				data->param_circle_radius = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_circle_zoom = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_circle_alpha_zero = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_circle_feather_amount = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_circle_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_circle_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_circle_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_circle_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_circle_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_circle_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_circle_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_circle_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_circle_invert = param;
			}
		}
	}
}

static void load_ellipse_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/ellipse-mask.effect";

	data->effect_ellipse_mask =
		load_shader_effect(data->effect_ellipse_mask, effect_file_path);
	if (data->effect_ellipse_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_ellipse_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_ellipse_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_ellipse_image = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_ellipse_mask_position = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_ellipse_uv_size = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_ellipse_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_ellipse_global_scale = param;
			} else if (strcmp(info.name, "sin_rot") == 0) {
				data->param_ellipse_sin_rot = param;
			} else if (strcmp(info.name, "cos_rot") == 0) {
				data->param_ellipse_cos_rot = param;
			} else if (strcmp(info.name, "ellipse") == 0) {
				data->param_ellipse_ellipse = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_ellipse_zoom = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_ellipse_alpha_zero = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_ellipse_feather_amount = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_ellipse_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_ellipse_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_ellipse_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_ellipse_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_ellipse_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_ellipse_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_ellipse_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_ellipse_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_ellipse_invert = param;
			}
		}
	}
}

static void load_star_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/star-mask.effect";

	data->effect_star_mask =
		load_shader_effect(data->effect_star_mask, effect_file_path);
	if (data->effect_star_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_star_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_star_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_star_image = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_star_mask_position = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_star_uv_size = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_star_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_star_global_scale = param;
			} else if (strcmp(info.name, "sin_rot") == 0) {
				data->param_star_sin_rot = param;
			} else if (strcmp(info.name, "cos_rot") == 0) {
				data->param_star_cos_rot = param;
			} else if (strcmp(info.name, "radius") == 0) {
				data->param_star_radius = param;
			} else if (strcmp(info.name, "corner_radius") == 0) {
				data->param_star_corner_radius = param;
			} else if (strcmp(info.name, "an") == 0) {
				data->param_star_an = param;
			} else if (strcmp(info.name, "en") == 0) {
				data->param_star_en = param;
			} else if (strcmp(info.name, "acs") == 0) {
				data->param_star_acs = param;
			} else if (strcmp(info.name, "ecs") == 0) {
				data->param_star_ecs = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_star_zoom = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_star_alpha_zero = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_star_feather_amount = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_star_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_star_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_star_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_star_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_star_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_star_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_star_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_star_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_star_invert = param;
			}
		}
	}
}

static void load_polygon_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/polygon-mask.effect";

	data->effect_polygon_mask =
		load_shader_effect(data->effect_polygon_mask, effect_file_path);
	if (data->effect_polygon_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_polygon_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_polygon_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_polygon_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_polygon_uv_size = param;
			} else if (strcmp(info.name, "radius") == 0) {
				data->param_polygon_radius = param;
			} else if (strcmp(info.name, "num_sides") == 0) {
				data->param_polygon_num_sides = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_polygon_mask_position = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_polygon_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_polygon_global_scale = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_polygon_zoom = param;
			} else if (strcmp(info.name, "sin_rot") == 0) {
				data->param_polygon_sin_rot = param;
			} else if (strcmp(info.name, "cos_rot") == 0) {
				data->param_polygon_cos_rot = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_polygon_alpha_zero = param;
			} else if (strcmp(info.name, "theta") == 0) {
				data->param_polygon_theta = param;
			} else if (strcmp(info.name, "theta_2") == 0) {
				data->param_polygon_theta_2 = param;
			} else if (strcmp(info.name, "theta_s") == 0) {
				data->param_polygon_theta_s = param;
			} else if (strcmp(info.name, "corner_radius") == 0) {
				data->param_polygon_corner_radius = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_polygon_feather_amount = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_polygon_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_polygon_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_polygon_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_polygon_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_polygon_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_polygon_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_polygon_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_polygon_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_polygon_invert = param;
			}
		}
	}
}

static void load_heart_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/heart-mask.effect";

	data->effect_heart_mask =
		load_shader_effect(data->effect_heart_mask, effect_file_path);
	if (data->effect_heart_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_heart_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_heart_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_heart_image = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_heart_mask_position = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_heart_uv_size = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_heart_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_heart_global_scale = param;
			} else if (strcmp(info.name, "size") == 0) {
				data->param_heart_size = param;
			} else if (strcmp(info.name, "sin_rot") == 0) {
				data->param_heart_sin_rot = param;
			} else if (strcmp(info.name, "cos_rot") == 0) {
				data->param_heart_cos_rot = param;
			} else if (strcmp(info.name, "corner_radius") == 0) {
				data->param_heart_corner_radius = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_heart_zoom = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_heart_alpha_zero = param;
			} else if (strcmp(info.name, "feather_amount") == 0) {
				data->param_heart_feather_amount = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_heart_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_heart_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_heart_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_heart_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_heart_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_heart_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_heart_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_heart_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_heart_invert = param;
			}
		}
	}
}

static void load_super_mask_effect(mask_shape_data_t *data)
{
	const char *effect_file_path = "/shaders/superformula-mask.effect";

	data->effect_super_mask =
		load_shader_effect(data->effect_super_mask, effect_file_path);
	if (data->effect_super_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_super_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_super_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "uv_size") == 0) {
				data->param_super_uv_size = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_super_mask_position = param;
			} else if (strcmp(info.name, "m") == 0) {
				data->param_super_m = param;
			} else if (strcmp(info.name, "n1") == 0) {
				data->param_super_n1 = param;
			} else if (strcmp(info.name, "n2") == 0) {
				data->param_super_n2 = param;
			} else if (strcmp(info.name, "n3") == 0) {
				data->param_super_n3 = param;
			} else if (strcmp(info.name, "a") == 0) {
				data->param_super_a = param;
			} else if (strcmp(info.name, "b") == 0) {
				data->param_super_b = param;
			} else if (strcmp(info.name, "sin_theta") == 0) {
				data->param_super_sin_theta = param;
			} else if (strcmp(info.name, "cos_theta") == 0) {
				data->param_super_cos_theta = param;
			} else if (strcmp(info.name, "alpha_zero") == 0) {
				data->param_super_alpha_zero = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_super_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_super_global_scale = param;
			} else if (strcmp(info.name, "min_r") == 0) {
				data->param_super_min_r = param;
			}
			//else if (strcmp(info.name, "aspect_ratio") == 0) {
			//	data->param_rect_aspect_ratio = param;
			//}
			//else if (strcmp(info.name, "aa_scale") == 0) {
			//	data->param_rectangle_aa_scale = param;
			//}
			else if (strcmp(info.name, "zoom") == 0) {
				data->param_super_zoom = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_super_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_super_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_super_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_super_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_super_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_super_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_super_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_super_max_hue_shift = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_super_invert = param;
			}
		}
	}
}
