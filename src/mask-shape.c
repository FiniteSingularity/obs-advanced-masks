#include "mask-shape.h"
#include "obs-utils.h"

mask_shape_data_t *mask_shape_create()
{
	mask_shape_data_t *data = bzalloc(sizeof(mask_shape_data_t));

	data->effect_rectangle_mask = NULL;
	data->effect_circle_mask = NULL;

	data->param_rectangle_image = NULL;
	data->param_rectangle_uv_size = NULL;
	data->param_rectangle_mask_position = NULL;
	data->param_rectangle_width = NULL;
	data->param_rectangle_height = NULL;
	data->param_global_position = NULL;
	data->param_global_scale = NULL;
	data->param_corner_radius = NULL;
	data->param_rect_aspect_ratio = NULL;
	data->param_rectangle_aa_scale = NULL;
	data->param_max_corner_radius = NULL;
	data->param_rectangle_zoom = NULL;
	data->param_rectangle_min_brightness = NULL;
	data->param_rectangle_max_brightness = NULL;
	data->param_rectangle_min_contrast = NULL;
	data->param_rectangle_max_contrast = NULL;
	data->param_rectangle_min_saturation = NULL;
	data->param_rectangle_max_saturation = NULL;
	data->param_rectangle_min_hue_shift = NULL;
	data->param_rectangle_max_hue_shift = NULL;

	data->param_circle_global_position = NULL;
	data->param_circle_mask_position = NULL;
	data->param_circle_image = NULL;
	data->param_circle_radius = NULL;
	data->param_circle_zoom = NULL;
	data->param_circle_aspect_ratio = NULL;
	data->param_circle_global_scale = NULL;

	load_shape_effect_files(data);

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

	obs_leave_graphics();

	bfree(data);
}

void mask_shape_update(mask_shape_data_t *data, base_filter_data_t *base, obs_data_t *settings)
{
	data->mask_shape_type =
		(uint32_t)obs_data_get_int(settings, "shape_type");

	data->shape_relative = obs_data_get_bool(settings, "shape_relative");

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

	data->rectangle_width =
		(float)obs_data_get_double(settings, "rectangle_width");
	data->rectangle_height =
		(float)obs_data_get_double(settings, "rectangle_height");
	data->zoom =
		base->mask_effect == MASK_EFFECT_ALPHA
			? (float)obs_data_get_double(settings, "source_zoom")
			: 100.0f;

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
				 settings, "rectangle_corner_radius_tl"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_tr"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_bl"),
			 (float)obs_data_get_double(
				 settings, "rectangle_corner_radius_br"));
		float max_radius = -1.0;
		for (uint32_t i = 0; i < 4; i++) {
			if (data->rectangle_corner_radius.ptr[i] >
			    max_radius) {
				max_radius =
					data->rectangle_corner_radius.ptr[i];
			}
		}
		data->rectangle_max_corner_radius = max_radius;
	}
	data->scale_type = (uint32_t)obs_data_get_int(settings, "scale_type");

	data->radius = (float)obs_data_get_double(settings, "circle_radius");
}

void mask_shape_defaults(obs_data_t* settings) {
	obs_data_set_default_int(settings, "shape_type", SHAPE_RECTANGLE);
	obs_data_set_default_double(settings, "shape_center_x", 960.0);
	obs_data_set_default_double(settings, "shape_center_y", 540.0);
	obs_data_set_default_double(settings, "rectangle_width", 250.0);
	obs_data_set_default_double(settings, "rectangle_height", 250.0);
	obs_data_set_default_double(settings, "position_x", 960.0);
	obs_data_set_default_double(settings, "position_y", 540.0);
	obs_data_set_default_double(settings, "position_scale", 100.0);
	obs_data_set_default_double(settings, "mask_source_filter_multiplier", 1.0);
	obs_data_set_default_double(settings, "source_zoom", 100.0);
	obs_data_set_default_bool(settings, "shape_relative", true);
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
	//obs_property_list_add_int(shape_type_list,
	//			  obs_module_text(SHAPE_ELLIPSE_LABEL),
	//			  SHAPE_ELLIPSE);
	//obs_property_list_add_int(shape_type_list,
	//			  obs_module_text(SHAPE_HEXAGON_LABEL),
	//			  SHAPE_HEXAGON);

	obs_property_set_modified_callback(shape_type_list,
					   setting_shape_type_modified);

	obs_property_t *shape_relative =
		obs_properties_add_bool(props, "shape_relative",
					obs_module_text("AdvancedMasks.Shape.Relative"));

	obs_property_set_modified_callback(shape_relative,
					   setting_shape_relative_modified);

	
}

void shape_mask_bot_properties(obs_properties_t *props,
				      obs_source_t *context,
				      mask_shape_data_t *data)
{
	obs_properties_t *source_rect_mask_group = obs_properties_create();
	obs_property_t *p;

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "shape_center_x",
		obs_module_text("AdvancedMasks.Shape.Center.X"), -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "shape_center_y",
		obs_module_text("AdvancedMasks.Shape.Center.Y"), -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "rectangle_width",
		obs_module_text("AdvancedMasks.Shape.Rectangle.Width"), -2000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "rectangle_height",
		obs_module_text("AdvancedMasks.Shape.Rectangle.Height"),
		-2000.0, 6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "circle_radius",
		obs_module_text("AdvancedMasks.Shape.Circle.Radius"), 0.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		source_rect_mask_group, "source_zoom",
		obs_module_text("AdvancedMasks.Shape.SourceZoom"), 1.0, 5000.0,
		1.0);
	obs_property_float_set_suffix(p, "%");

	obs_properties_add_group(
		props, "rectangle_source_group",
		obs_module_text("AdvancedMasks.Shape.Rectangle.SourceGroup"),
		OBS_GROUP_NORMAL, source_rect_mask_group);

	obs_properties_t *corner_radius_group = obs_properties_create();

	obs_property_t *corner_type_list = obs_properties_add_list(
		corner_radius_group, "rectangle_corner_type",
		obs_module_text("AdvancedMasks.CornerType"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_UNIFORM_LABEL),
				  MASK_CORNER_UNIFORM);
	obs_property_list_add_int(corner_type_list,
				  obs_module_text(MASK_CORNER_CUSTOM_LABEL),
				  MASK_CORNER_CUSTOM);

	obs_property_set_modified_callback(corner_type_list,
					   setting_corner_type_modified);

	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius",
		obs_module_text("AdvancedMasks.Shape.Rectangle.CornerRadius"),
		0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_tl",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopLeft"),
		0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_tr",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.TopRight"),
		0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_bl",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomLeft"),
		0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		corner_radius_group, "rectangle_corner_radius_br",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.BottomRight"),
		0.0, 1000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "rectangle_rounded_corners_group",
		obs_module_text(
			"AdvancedMasks.Shape.Rectangle.CornerRadius.CustomGroup"),
		OBS_GROUP_NORMAL, corner_radius_group);

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

bool setting_shape_type_modified(obs_properties_t *props,
					obs_property_t *p, obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int shape_type = (int)obs_data_get_int(settings, "shape_type");
	int effect_type = (int)obs_data_get_int(settings, "mask_effect");
	switch (shape_type) {
	case SHAPE_RECTANGLE:
		setting_visibility("rectangle_width", true, props);
		setting_visibility("rectangle_height", true, props);
		setting_visibility("circle_radius", false, props);
		setting_visibility("rectangle_rounded_corners_group", true,
				   props);
		break;
	case SHAPE_CIRCLE:
		setting_visibility("rectangle_width", false, props);
		setting_visibility("rectangle_height", false, props);
		setting_visibility("circle_radius", true, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
	}
	setting_visibility("source_zoom", effect_type == MASK_EFFECT_ALPHA,
			   props);
	obs_property_t *group =
		obs_properties_get(props, "rectangle_source_group");
	const char *group_name =
		effect_type == MASK_EFFECT_ALPHA
			? obs_module_text(
				  "AdvancedMasks.Shape.Rectangle.SourceGroup")
			: obs_module_text(
				  "AdvancedMasks.Shape.Rectangle.GeometryGroup");
	obs_property_set_description(group, group_name);
	return true;
}

static bool setting_shape_relative_modified(obs_properties_t *props, obs_property_t *p,
				 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	bool relative = obs_data_get_bool(settings, "shape_relative");
	if (relative) {
		setting_visibility("scale_position_group", true, props);
	} else{
		setting_visibility("scale_position_group", false, props);
	}
	return true;
}

static bool setting_corner_type_modified(obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
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
	mask_shape_data_t *filter = data;
	uint32_t type = (uint32_t)obs_data_get_int(settings, "scale_type");
	uint32_t last_type = filter->last_scale_type;
	if (last_type == 0 || type == last_type) {
		filter->last_scale_type = type;
		return false;
	}
	if (type == MASK_SCALE_WIDTH) {
		double width = obs_data_get_double(settings, "rectangle_width");
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      (double)width, (double)1.0);
		obs_data_set_double(settings, "position_scale", width);
		obs_property_float_set_suffix(scale_p, "px");
	} else if (type == MASK_SCALE_HEIGHT) {
		double height =
			obs_data_get_double(settings, "rectangle_height");
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      (double)height, (double)1.0);
		obs_data_set_double(settings, "position_scale", height);
		obs_property_float_set_suffix(scale_p, "px");
	} else {
		obs_property_t *scale_p =
			obs_properties_get(props, "position_scale");
		obs_property_float_set_limits(scale_p, (double)0.0,
					      (double)10000.0, (double)0.1);
		obs_data_set_double(settings, "position_scale", 100.0);
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

	float scale_factor =
		data->scale_type == MASK_SCALE_PERCENT
			? data->global_scale / 100.0f
		: data->scale_type == MASK_SCALE_WIDTH
			? data->global_scale / data->rectangle_width
			: data->global_scale / data->rectangle_height;

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

	if (data->param_rectangle_height) {
		gs_effect_set_float(data->param_rectangle_height,
				    data->rectangle_height);
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
		gs_effect_set_float(data->param_global_scale, data->shape_relative ? scale_factor : 1.0f);
	}

	if (data->param_corner_radius) {
		struct vec4 corner_radius;
		vec4_divf(&corner_radius, &data->rectangle_corner_radius,
			  scale_factor * (data->zoom / 100.0f));
		gs_effect_set_vec4(data->param_corner_radius, &corner_radius);
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
		const float min_brightness =
			color_adj->adj_brightness ? color_adj->min_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_brightness,
				    min_brightness);
	}

	if (data->param_rectangle_max_brightness) {
		const float max_brightness =
			color_adj->adj_brightness ? color_adj->max_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_brightness,
				    max_brightness);
	}

	if (data->param_rectangle_min_contrast) {
		const float min_contrast =
			color_adj->adj_contrast ? color_adj->min_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_contrast,
				    min_contrast);
	}

	if (data->param_rectangle_max_contrast) {
		const float max_contrast =
			color_adj->adj_contrast ? color_adj->max_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_contrast,
				    max_contrast);
	}

	if (data->param_rectangle_min_saturation) {
		const float min_saturation =
			color_adj->adj_saturation ? color_adj->min_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_min_saturation,
				    min_saturation);
	}

	if (data->param_rectangle_max_saturation) {
		const float max_saturation =
			color_adj->adj_saturation ? color_adj->max_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_saturation,
				    max_saturation);
	}

	if (data->param_rectangle_min_hue_shift) {
		const float min_hue_shift =
			color_adj->adj_hue_shift ? color_adj->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_rectangle_max_hue_shift) {
		const float max_hue_shift =
			color_adj->adj_hue_shift ? color_adj->max_hue_shift : 1.0f;
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
	const char *technique = base->mask_effect == MASK_EFFECT_ALPHA
					? "Alpha"
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
	gs_effect_t *effect = data->effect_rectangle_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	float scale_factor =
		data->scale_type == MASK_SCALE_PERCENT
			? data->global_scale / 100.0f
		: data->scale_type == MASK_SCALE_WIDTH
			? data->global_scale / data->rectangle_width
			: data->global_scale / data->rectangle_height;

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
		float width = 2.0f * data->radius;
		gs_effect_set_float(data->param_rectangle_width, width);
	}

	if (data->param_rectangle_height) {
		float height = 2.0f * data->radius;
		gs_effect_set_float(data->param_rectangle_height, height);
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
		struct vec4 corner_radius;
		float r = data->radius / (data->zoom / 100.0f);
		vec4_set(&corner_radius, r, r, r, r);
		gs_effect_set_vec4(data->param_corner_radius, &corner_radius);
	}

	if (data->param_max_corner_radius) {
		float max_corner_radius = data->radius / (data->zoom / 100.0f);
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
		const float min_brightness =
			color_adj->adj_brightness ? color_adj->min_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_brightness,
				    min_brightness);
	}

	if (data->param_rectangle_max_brightness) {
		const float max_brightness =
			color_adj->adj_brightness ? color_adj->max_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_brightness,
				    max_brightness);
	}

	if (data->param_rectangle_min_contrast) {
		const float min_contrast =
			color_adj->adj_contrast ? color_adj->min_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_contrast,
				    min_contrast);
	}

	if (data->param_rectangle_max_contrast) {
		const float max_contrast =
			color_adj->adj_contrast ? color_adj->max_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_contrast,
				    max_contrast);
	}

	if (data->param_rectangle_min_saturation) {
		const float min_saturation =
			color_adj->adj_saturation ? color_adj->min_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_min_saturation,
				    min_saturation);
	}

	if (data->param_rectangle_max_saturation) {
		const float max_saturation =
			color_adj->adj_saturation ? color_adj->max_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_saturation,
				    max_saturation);
	}

	if (data->param_rectangle_min_hue_shift) {
		const float min_hue_shift =
			color_adj->adj_hue_shift ? color_adj->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_rectangle_max_hue_shift) {
		const float max_hue_shift =
			color_adj->adj_hue_shift ? color_adj->max_hue_shift : 1.0f;
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

	const char *technique = base->mask_effect == MASK_EFFECT_ALPHA
					? "Alpha"
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

	data->effect_circle_mask = load_shader_effect(
		data->effect_circle_mask, effect_file_path);
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
			} else if (strcmp(info.name, "radius") == 0) {
				data->param_circle_radius = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				data->param_circle_zoom = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_circle_mask_position = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				data->param_circle_global_position = param;
			} else if (strcmp(info.name, "aspect_ratio") == 0) {
				data->param_circle_aspect_ratio = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				data->param_circle_global_scale = param;
			}
		}
	}
}
