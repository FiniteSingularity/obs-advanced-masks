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
	obs_leave_graphics();

	bfree(data);
}

static float mask_width(obs_data_t * settings)
{
	float num_sides = (float)obs_data_get_int(settings, "shape_num_sides");
	switch ((uint32_t)obs_data_get_int(settings, "shape_type")) {
	case SHAPE_RECTANGLE:
		return (float)obs_data_get_double(settings, "rectangle_width");
	case SHAPE_CIRCLE:
		return (float)obs_data_get_double(settings, "circle_radius") * 2.0f;
	case SHAPE_ELLIPSE:
		return (float)obs_data_get_double(settings, "shape_ellipse_a") * 2.0f;
	case SHAPE_POLYGON:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       (float)cos(M_PI / num_sides) * 2.0f;
	case SHAPE_STAR:
		return (float)obs_data_get_double(settings,
						  "shape_star_outer_radius") *
		       2.0f;
	case SHAPE_HEART:
		return (float)obs_data_get_double(settings, "heart_size");
	}
	return 0.0f;
}

static float mask_height(obs_data_t *settings)
{
	float num_sides = (float)obs_data_get_int(settings, "shape_num_sides");
	switch ((uint32_t)obs_data_get_int(settings, "shape_type")) {
	case SHAPE_RECTANGLE:
		return (float)obs_data_get_double(settings, "rectangle_height");
	case SHAPE_CIRCLE:
		return (float)obs_data_get_double(settings, "circle_radius") * 2.0f;
	case SHAPE_ELLIPSE:
		return (float)obs_data_get_double(settings, "shape_ellipse_b") * 2.0f;
	case SHAPE_POLYGON:
		return (float)obs_data_get_double(settings, "circle_radius") *
		       (float)cos(M_PI / num_sides) * 2.0f;
	case SHAPE_STAR:
		return (float)obs_data_get_double(settings,
						  "shape_star_outer_radius") *
		       2.0f;
	case SHAPE_HEART:
		return (float)obs_data_get_double(settings, "heart_size");
	}
	return 0.0f;
}

void mask_shape_update(mask_shape_data_t *data, base_filter_data_t *base,
		       obs_data_t *settings)
{
	data->mask_shape_type =
		(uint32_t)obs_data_get_int(settings, "shape_type");

	data->shape_relative = obs_data_get_bool(settings, "shape_relative");
	data->frame_check = obs_data_get_bool(settings, "shape_frame_check");
	data->mask_center.x =
		(float)obs_data_get_double(settings, "shape_center_x");
	data->mask_center.y =
		(float)obs_data_get_double(settings, "shape_center_y");
	data->global_position.x =
		(float)obs_data_get_double(settings, "position_x");
	data->global_position.y =
		(float)obs_data_get_double(settings, "position_y");
	data->global_scale =
		base->mask_effect == MASK_EFFECT_ALPHA
			? (float)obs_data_get_double(settings, "position_scale")
			: 100.0f;
	data->zoom =
		base->mask_effect == MASK_EFFECT_ALPHA
			? (float)obs_data_get_double(settings, "source_zoom")
			: 100.0f;
	data->scale_type = (uint32_t)obs_data_get_int(settings, "scale_type");

	if (base->mask_effect == MASK_EFFECT_ALPHA && data->scale_type == MASK_SCALE_WIDTH) {
		data->global_scale = 100.0f * data->global_scale / mask_width(settings);
	} else if (base->mask_effect == MASK_EFFECT_ALPHA && data->scale_type == MASK_SCALE_HEIGHT) {
		data->global_scale = 100.0f * data->global_scale / mask_height(settings);
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

void mask_shape_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "shape_type", SHAPE_RECTANGLE);
	obs_data_set_default_bool(settings, "shape_frame_check", false);
	obs_data_set_default_double(settings, "shape_center_x", -1.e9);
	obs_data_set_default_double(settings, "shape_center_y", -1.e9);
	obs_data_set_default_double(settings, "shape_rotation", 0.0);
	obs_data_set_default_double(settings, "rectangle_width", 800.0);
	obs_data_set_default_double(settings, "rectangle_height", 600.0);
	obs_data_set_default_double(settings, "position_x", -1.e9);
	obs_data_set_default_double(settings, "position_y", -1.e9);
	obs_data_set_default_double(settings, "position_scale", 120.0);
	obs_data_set_default_double(settings, "mask_source_filter_multiplier", 1.0);
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

	obs_property_set_modified_callback(shape_type_list,
					   setting_shape_type_modified);

	obs_property_t *shape_relative = obs_properties_add_bool(
		props, "shape_relative",
		obs_module_text("AdvancedMasks.Shape.Relative"));

	obs_property_set_modified_callback(shape_relative,
					   setting_shape_relative_modified);

	obs_properties_add_bool(
		props, "shape_frame_check",
		obs_module_text("AdvancedMasks.Shape.FrameCheck"));
}

void shape_mask_bot_properties(obs_properties_t *props, obs_source_t *context,
			       mask_shape_data_t *data)
{
	shape_properties(props, context, data);
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

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Center.X"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_center_x",
		label, -2000.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Center.Y"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_center_y",
		label, -2000.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");


	label_indent(label, obs_module_text("AdvancedMasks.Shape.RecenterButton"));
	p = obs_properties_add_button2(mask_geometry_group, "recenter_mask_button", label,
				       recenter_button_clicked, context);

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Rotation"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_rotation",
		label, -360.0, 360.0, 1.0);
	obs_property_float_set_suffix(p, "deg");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.NumSides"));
	p = obs_properties_add_int_slider(
		mask_geometry_group, "shape_num_sides",
		label, 3, 100, 1);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Rectangle.Width"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "rectangle_width",
		label, -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Rectangle.Height"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "rectangle_height",
		label,
		-2000.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Circle.Radius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "circle_radius",
		label, 0.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Ellipse.Width"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_ellipse_a",
		label, 0.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Ellipse.Height"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_ellipse_b",
		label, 0.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Star.NumPoints"));
	p = obs_properties_add_int_slider(
		mask_geometry_group, "shape_star_num_points",
		label, 3, 100,
		1);

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.Star.OuterRadius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_star_outer_radius",
		label, 0.0,
		2500.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Star.InnerRadius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_star_inner_radius",
		label, 0.0,
		100.0, 0.1);
	obs_property_float_set_suffix(p, "%");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.CornerRadius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "shape_corner_radius",
		label, 0.0,
		1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label,
		     obs_module_text("AdvancedMasks.Shape.CornerRadius"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "star_corner_radius",
		label, 0.0,
		1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Heart.Size"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "heart_size",
		label, 0.0, 6000.0,
		1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(label, obs_module_text("AdvancedMasks.Shape.SourceZoom"));
	p = obs_properties_add_float_slider(
		mask_geometry_group, "source_zoom",
		label, 1.0, 5000.0,
		1.0);
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
	label_indent(label, obs_module_text("AdvancedMasks.Shape.Feather.Type"));
	obs_property_t *feather_type = obs_properties_add_list(
		shape_feather_group, "shape_feather_type",
		label, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

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

	label_indent(label, obs_module_text("AdvancedMasks.Shape.Feather.Amount"));
	p = obs_properties_add_float_slider(
		shape_feather_group, "shape_feather_amount",
		label, 0.0, 500.0, 1.0);
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
	label_indent(label, obs_module_text("AdvancedMasks.Shape.Rectangle.CornerRadius"));
	obs_property_t *corner_type_list = obs_properties_add_list(
		corner_radius_group, "rectangle_corner_type",
		label, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_UNIFORM_LABEL),
				  MASK_CORNER_UNIFORM);
	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_CUSTOM_LABEL),
				  MASK_CORNER_CUSTOM);

	obs_property_set_modified_callback(corner_type_list,
					   setting_corner_type_modified);

	label_indent(label, obs_module_text(""));
	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius",
		label, 0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopLeft"));
	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_tl",
		label, 0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopRight"));
	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_tr",
		label, 0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomLeft"));
	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_bl",
		label, 0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	label_indent(
		label,
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomRight"));
	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_br",
		label, 0.0, 1000.0, 1.0);
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
	obs_data_set_double(settings, "shape_center_x", width/2.0);
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

bool setting_shape_relative_modified(obs_properties_t *props, obs_property_t *p,
				     obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	if ((uint32_t)obs_data_get_int(settings, "mask_type") !=
	    MASK_TYPE_SHAPE) {
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
	float width = mask_width(settings);
	float height = mask_height(settings);
	float pct =
		filter->last_scale_type == MASK_SCALE_WIDTH
			? (float)obs_data_get_double(settings,
							     "position_scale") /
				  width
		: filter->last_scale_type == MASK_SCALE_HEIGHT
			? (float)obs_data_get_double(settings,
							     "position_scale") /
				  height
			: (float)obs_data_get_double(settings,
						     "position_scale") / 100.0f;
			  
	if (last_type == 0 || type == last_type) {
		filter->last_scale_type = type;
		return false;
	}
	if (type == MASK_SCALE_WIDTH) {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");

		obs_property_float_set_limits(scale_p, (double)0.0,
					      100.0 * (double)width, (double)1.0);
		obs_data_set_double(settings, "position_scale", width * pct);
		obs_property_float_set_suffix(scale_p, "px");
	} else if (type == MASK_SCALE_HEIGHT) {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      100.0 * (double)height, (double)1.0);
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
		       color_adjustments_data_t *color_adj)
{
	switch (data->mask_shape_type) {
	case SHAPE_RECTANGLE:
		render_rectangle_mask(data, base, color_adj);
		break;
	case SHAPE_CIRCLE:
		render_circle_mask(data, base, color_adj);
		break;
	case SHAPE_POLYGON:
		render_polygon_mask(data, base, color_adj);
		break;
	case SHAPE_ELLIPSE:
		render_ellipse_mask(data, base, color_adj);
		break;
	case SHAPE_STAR:
		render_star_mask(data, base, color_adj);
		break;
	case SHAPE_HEART:
		render_heart_mask(data, base, color_adj);
		break;
	}
}

static void render_rectangle_mask(mask_shape_data_t *data,
				  base_filter_data_t *base,
				  color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_rectangle_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_rectangle_image) {
		gs_effect_set_texture(data->param_rectangle_image, texture);
	}

	if (data->param_rectangle_zoom) {
		gs_effect_set_float(data->param_rectangle_zoom,
				    data->zoom / 100.0f);
	}
	if (data->param_rectangle_mask_position) {
		gs_effect_set_vec2(data->param_rectangle_mask_position,
				   &data->mask_center);
	}

	if (data->param_rectangle_width) {
		gs_effect_set_float(data->param_rectangle_width,
				    data->rectangle_width);
	}

	if (data->param_rectangle_alpha_zero) {
		gs_effect_set_float(data->param_rectangle_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_rectangle_height) {
		gs_effect_set_float(data->param_rectangle_height,
				    data->rectangle_height);
	}

	if (data->param_rectangle_sin_theta) {
		gs_effect_set_float(data->param_rectangle_sin_theta,
				    (float)sin(data->rotation));
	}

	if (data->param_rectangle_cos_theta) {
		gs_effect_set_float(data->param_rectangle_cos_theta,
				    (float)cos(data->rotation));
	}

	if (data->param_rectangle_feather_amount) {
		gs_effect_set_float(data->param_rectangle_feather_amount,
				    data->feather_amount);
	}

	if (data->param_rectangle_feather_shift) {
		gs_effect_set_float(data->param_rectangle_feather_shift,
				    data->feather_shift);
	}

	if (data->param_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_global_scale) {
		gs_effect_set_float(data->param_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_corner_radius) {
		/*	struct vec4 corner_radius;
		vec4_divf(&corner_radius, &data->rectangle_corner_radius,
			  scale_factor * (data->zoom / 100.0f));*/
		gs_effect_set_vec4(data->param_corner_radius,
				   &data->rectangle_corner_radius);
	}

	if (data->param_max_corner_radius) {
		float max_corner_radius = data->rectangle_max_corner_radius /
					  scale_factor * (data->zoom / 100.0f);
		gs_effect_set_float(data->param_max_corner_radius,
				    max_corner_radius);
	}

	if (data->param_rect_aspect_ratio) {
		struct vec2 box_ar;
		box_ar.x =
			(float)base->width /
			(float)fmin((double)base->width, (double)base->height);
		box_ar.y =
			(float)base->height /
			(float)fmin((double)base->width, (double)base->height);

		gs_effect_set_vec2(data->param_rect_aspect_ratio, &box_ar);
	}

	if (data->param_rectangle_aa_scale) {
		float aa_scale = 5.0f / (float)base->height;
		gs_effect_set_float(data->param_rectangle_aa_scale, aa_scale);
	}

	if (data->param_rectangle_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_brightness,
				    min_brightness);
	}

	if (data->param_rectangle_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_brightness,
				    max_brightness);
	}

	if (data->param_rectangle_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_contrast,
				    min_contrast);
	}

	if (data->param_rectangle_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_contrast,
				    max_contrast);
	}

	if (data->param_rectangle_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_rectangle_min_saturation,
				    min_saturation);
	}

	if (data->param_rectangle_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_saturation,
				    max_saturation);
	}

	if (data->param_rectangle_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_rectangle_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_rectangle_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_rectangle_uv_size, &uv_size);
	}

	set_blending_parameters();
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_polygon_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_polygon_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_polygon_image) {
		gs_effect_set_texture(data->param_polygon_image, texture);
	}

	if (data->param_polygon_zoom) {
		gs_effect_set_float(data->param_polygon_zoom,
				    data->zoom / 100.0f);
	}
	if (data->param_polygon_mask_position) {
		gs_effect_set_vec2(data->param_polygon_mask_position,
				   &data->mask_center);
	}
	if (data->param_polygon_sin_rot) {
		gs_effect_set_float(data->param_polygon_sin_rot,
				    (float)sin(data->rotation));
	}

	if (data->param_polygon_cos_rot) {
		gs_effect_set_float(data->param_polygon_cos_rot,
				    (float)cos(data->rotation));
	}

	if (data->param_polygon_alpha_zero) {
		gs_effect_set_float(data->param_polygon_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_polygon_theta) {
		gs_effect_set_float(data->param_polygon_theta, data->theta);
	}

	if (data->param_polygon_theta_2) {
		gs_effect_set_float(data->param_polygon_theta_2,
				    2.0f * data->theta);
	}

	if (data->param_polygon_theta_s) {
		gs_effect_set_vec2(data->param_polygon_theta_s, &data->theta_s);
	}

	if (data->param_polygon_corner_radius) {
		gs_effect_set_float(data->param_polygon_corner_radius,
				    data->shape_corner_radius);
	}

	if (data->param_polygon_feather_amount) {
		gs_effect_set_float(data->param_polygon_feather_amount,
				    data->feather_amount);
	}
	if (data->param_polygon_radius) {
		gs_effect_set_float(data->param_polygon_radius, data->radius);
	}

	if (data->param_polygon_num_sides) {
		gs_effect_set_float(data->param_polygon_num_sides,
				    data->num_sides);
	}

	if (data->param_polygon_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_polygon_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_polygon_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_polygon_global_scale) {
		gs_effect_set_float(data->param_polygon_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_polygon_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_polygon_min_brightness,
				    min_brightness);
	}

	if (data->param_polygon_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_polygon_max_brightness,
				    max_brightness);
	}

	if (data->param_polygon_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_polygon_min_contrast,
				    min_contrast);
	}

	if (data->param_polygon_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_polygon_max_contrast,
				    max_contrast);
	}

	if (data->param_polygon_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_polygon_min_saturation,
				    min_saturation);
	}

	if (data->param_polygon_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_polygon_max_saturation,
				    max_saturation);
	}

	if (data->param_polygon_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_polygon_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_polygon_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_polygon_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_polygon_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_polygon_uv_size, &uv_size);
	}

	set_blending_parameters();
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_star_mask(mask_shape_data_t *data, base_filter_data_t *base,
			     color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_star_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_star_image) {
		gs_effect_set_texture(data->param_star_image, texture);
	}

	if (data->param_star_zoom) {
		gs_effect_set_float(data->param_star_zoom, data->zoom / 100.0f);
	}
	if (data->param_star_mask_position) {
		gs_effect_set_vec2(data->param_star_mask_position,
				   &data->mask_center);
	}
	if (data->param_star_sin_rot) {
		gs_effect_set_float(data->param_star_sin_rot,
				    (float)sin(data->rotation));
	}

	if (data->param_star_cos_rot) {
		gs_effect_set_float(data->param_star_cos_rot,
				    (float)cos(data->rotation));
	}

	if (data->param_star_alpha_zero) {
		gs_effect_set_float(data->param_star_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_star_corner_radius) {
		gs_effect_set_float(data->param_star_corner_radius,
				    data->star_corner_radius);
	}

	if (data->param_star_an) {
		gs_effect_set_float(data->param_star_an, data->an);
	}

	if (data->param_star_en) {
		gs_effect_set_float(data->param_star_en, data->en);
	}

	if (data->param_star_acs) {
		gs_effect_set_vec2(data->param_star_acs, &data->acs);
	}

	if (data->param_star_ecs) {
		gs_effect_set_vec2(data->param_star_ecs, &data->ecs);
	}

	if (data->param_star_feather_amount) {
		gs_effect_set_float(data->param_star_feather_amount,
				    data->feather_amount);
	}
	if (data->param_star_radius) {
		gs_effect_set_float(data->param_star_radius,
				    data->star_outer_radius);
	}

	if (data->param_star_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_star_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_star_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_star_global_scale) {
		gs_effect_set_float(data->param_star_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_star_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_star_min_brightness,
				    min_brightness);
	}

	if (data->param_star_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_star_max_brightness,
				    max_brightness);
	}

	if (data->param_star_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_star_min_contrast,
				    min_contrast);
	}

	if (data->param_star_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_star_max_contrast,
				    max_contrast);
	}

	if (data->param_star_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_star_min_saturation,
				    min_saturation);
	}

	if (data->param_star_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_star_max_saturation,
				    max_saturation);
	}

	if (data->param_star_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_star_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_star_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_star_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_star_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_star_uv_size, &uv_size);
	}

	set_blending_parameters();
	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_circle_mask(mask_shape_data_t *data,
			       base_filter_data_t *base,
			       color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_circle_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_circle_image) {
		gs_effect_set_texture(data->param_circle_image, texture);
	}

	if (data->param_circle_zoom) {
		gs_effect_set_float(data->param_circle_zoom,
				    data->zoom / 100.0f);
	}

	if (data->param_circle_alpha_zero) {
		gs_effect_set_float(data->param_circle_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_circle_mask_position) {
		gs_effect_set_vec2(data->param_circle_mask_position,
				   &data->mask_center);
	}

	if (data->param_circle_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_circle_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_circle_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_circle_global_scale) {

		gs_effect_set_float(data->param_circle_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_circle_radius) {
		gs_effect_set_float(data->param_circle_radius, data->radius);
	}

	if (data->param_circle_zoom) {
		gs_effect_set_float(data->param_circle_zoom,
				    data->zoom / 100.0f);
	}

	if (data->param_circle_feather_amount) {
		gs_effect_set_float(data->param_circle_feather_amount,
				    data->feather_amount);
	}

	if (data->param_circle_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_circle_min_brightness,
				    min_brightness);
	}

	if (data->param_circle_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_circle_max_brightness,
				    max_brightness);
	}

	if (data->param_circle_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_circle_min_contrast,
				    min_contrast);
	}

	if (data->param_circle_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_circle_max_contrast,
				    max_contrast);
	}

	if (data->param_circle_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_circle_min_saturation,
				    min_saturation);
	}

	if (data->param_circle_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_circle_max_saturation,
				    max_saturation);
	}

	if (data->param_circle_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_circle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_circle_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_circle_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_circle_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_circle_uv_size, &uv_size);
	}

	set_blending_parameters();

	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_heart_mask(mask_shape_data_t *data, base_filter_data_t *base,
			      color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_heart_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_heart_image) {
		gs_effect_set_texture(data->param_heart_image, texture);
	}

	if (data->param_heart_zoom) {
		gs_effect_set_float(data->param_heart_zoom,
				    data->zoom / 100.0f);
	}

	if (data->param_heart_alpha_zero) {
		gs_effect_set_float(data->param_heart_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_heart_mask_position) {
		gs_effect_set_vec2(data->param_heart_mask_position,
				   &data->mask_center);
	}

	if (data->param_heart_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_heart_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_heart_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_heart_global_scale) {

		gs_effect_set_float(data->param_heart_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_heart_size) {
		gs_effect_set_float(data->param_heart_size, data->heart_size);
	}

	if (data->param_heart_zoom) {
		gs_effect_set_float(data->param_heart_zoom,
				    data->zoom / 100.0f);
	}

	if (data->param_heart_sin_rot) {
		gs_effect_set_float(data->param_heart_sin_rot,
				    (float)sin(data->rotation));
	}

	if (data->param_heart_cos_rot) {
		gs_effect_set_float(data->param_heart_cos_rot,
				    (float)cos(data->rotation));
	}

	if (data->param_heart_corner_radius) {
		gs_effect_set_float(data->param_heart_corner_radius,
				    data->star_corner_radius);
	}

	if (data->param_heart_feather_amount) {
		gs_effect_set_float(data->param_heart_feather_amount,
				    data->feather_amount);
	}

	if (data->param_heart_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_heart_min_brightness,
				    min_brightness);
	}

	if (data->param_heart_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_heart_max_brightness,
				    max_brightness);
	}

	if (data->param_heart_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_heart_min_contrast,
				    min_contrast);
	}

	if (data->param_heart_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_heart_max_contrast,
				    max_contrast);
	}

	if (data->param_heart_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_heart_min_saturation,
				    min_saturation);
	}

	if (data->param_heart_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_heart_max_saturation,
				    max_saturation);
	}

	if (data->param_heart_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_heart_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_heart_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_heart_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_heart_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_heart_uv_size, &uv_size);
	}

	set_blending_parameters();

	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_ellipse_mask(mask_shape_data_t *data,
				base_filter_data_t *base,
				color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_ellipse_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor = data->global_scale / 100.0f;

	if (data->param_ellipse_image) {
		gs_effect_set_texture(data->param_ellipse_image, texture);
	}

	if (data->param_ellipse_zoom) {
		gs_effect_set_float(data->param_ellipse_zoom,
				    data->zoom / 100.0f);
	}
	if (data->param_ellipse_mask_position) {
		gs_effect_set_vec2(data->param_ellipse_mask_position,
				   &data->mask_center);
	}

	if (data->param_ellipse_global_position) {
		if (data->shape_relative) {
			gs_effect_set_vec2(data->param_ellipse_global_position,
					   &data->global_position);
		} else {
			gs_effect_set_vec2(data->param_ellipse_global_position,
					   &data->mask_center);
		}
	}

	if (data->param_ellipse_global_scale) {

		gs_effect_set_float(data->param_ellipse_global_scale,
				    data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_ellipse_sin_rot) {
		gs_effect_set_float(data->param_ellipse_sin_rot,
				    (float)sin(data->rotation));
	}

	if (data->param_ellipse_cos_rot) {
		gs_effect_set_float(data->param_ellipse_cos_rot,
				    (float)cos(data->rotation));
	}

	if (data->param_ellipse_alpha_zero) {
		gs_effect_set_float(data->param_ellipse_alpha_zero,
				    data->frame_check ? 0.3f : 0.0f);
	}

	if (data->param_ellipse_ellipse) {
		gs_effect_set_vec2(data->param_ellipse_ellipse, &data->ellipse);
	}

	if (data->param_ellipse_zoom) {
		gs_effect_set_float(data->param_ellipse_zoom,
				    data->zoom / 100.0f);
	}

	if (data->param_ellipse_feather_amount) {
		gs_effect_set_float(data->param_ellipse_feather_amount,
				    data->feather_amount);
	}

	if (data->param_ellipse_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_brightness,
				    min_brightness);
	}

	if (data->param_ellipse_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_ellipse_max_brightness,
				    max_brightness);
	}

	if (data->param_ellipse_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_contrast,
				    min_contrast);
	}

	if (data->param_ellipse_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_ellipse_max_contrast,
				    max_contrast);
	}

	if (data->param_ellipse_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_ellipse_min_saturation,
				    min_saturation);
	}

	if (data->param_ellipse_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_ellipse_max_saturation,
				    max_saturation);
	}

	if (data->param_ellipse_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_ellipse_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_ellipse_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_ellipse_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_ellipse_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_ellipse_uv_size, &uv_size);
	}

	set_blending_parameters();

	const char *technique =
		base->mask_effect == MASK_EFFECT_ALPHA && !data->frame_check
			? "Alpha"
		: base->mask_effect == MASK_EFFECT_ALPHA && data->frame_check
			? "AlphaFrameCheck"
			: "Adjustments";

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}

	gs_blend_state_pop();
}

static void load_shape_effect_files(mask_shape_data_t *data)
{
	load_rectangle_mask_effect(data);
	load_circle_mask_effect(data);
	load_polygon_mask_effect(data);
	load_ellipse_mask_effect(data);
	load_star_mask_effect(data);
	load_heart_mask_effect(data);
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
			} else if (strcmp(info.name, "max_corner_radius") == 0) {
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
			}
		}
	}
}
