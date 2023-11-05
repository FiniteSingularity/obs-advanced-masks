#include "advanced-masks-filter.h"
#include "advanced-masks.h"

struct obs_source_info advanced_masks_filter = {
	.id = "advanced_masks_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_SRGB,
	.get_name = advanced_masks_name,
	.create = advanced_masks_create,
	.destroy = advanced_masks_destroy,
	.update = advanced_masks_update,
	.video_render = advanced_masks_video_render,
	.video_tick = advanced_masks_video_tick,
	.get_width = advanced_masks_width,
	.get_height = advanced_masks_height,
	.get_properties = advanced_masks_properties,
	.get_defaults = advanced_masks_defaults};

static const char *advanced_masks_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("AdvancedMasks");
}

static void *advanced_masks_create(obs_data_t *settings, obs_source_t *source)
{
	// This function should initialize all pointers in the data
	// structure.
	advanced_masks_data_t *filter = bzalloc(sizeof(advanced_masks_data_t));

	filter->input_texrender =
		create_or_reset_texrender(filter->input_texrender);
	filter->output_texrender =
		create_or_reset_texrender(filter->output_texrender);
	filter->source_mask_texrender =
		create_or_reset_texrender(filter->source_mask_texrender);

	filter->effect_rectangle_mask = NULL;
	filter->effect_source_mask = NULL;
	filter->effect_circle_mask = NULL;

	filter->context = source;
	filter->rendered = false;
	filter->rendering = false;

	filter->param_rectangle_image = NULL;
	filter->param_rectangle_uv_size = NULL;
	filter->param_rectangle_mask_position = NULL;
	filter->param_rectangle_width = NULL;
	filter->param_rectangle_height = NULL;
	filter->param_global_position = NULL;
	filter->param_global_scale = NULL;
	filter->param_corner_radius = NULL;
	filter->param_rect_aspect_ratio = NULL;
	filter->param_rectangle_aa_scale = NULL;
	filter->param_max_corner_radius = NULL;
	filter->param_rectangle_zoom = NULL;
	filter->param_rectangle_min_brightness = NULL;
	filter->param_rectangle_max_brightness = NULL;
	filter->param_rectangle_min_contrast = NULL;
	filter->param_rectangle_max_contrast = NULL;
	filter->param_rectangle_min_saturation = NULL;
	filter->param_rectangle_max_saturation = NULL;
	filter->param_rectangle_min_hue_shift = NULL;
	filter->param_rectangle_max_hue_shift = NULL;


	filter->param_circle_global_position = NULL;
	filter->param_circle_mask_position = NULL;
	filter->param_circle_image = NULL;
	filter->param_circle_radius = NULL;
	filter->param_circle_zoom = NULL;
	filter->param_circle_aspect_ratio = NULL;
	filter->param_circle_global_scale = NULL;

	filter->param_source_mask_image = NULL;
	filter->param_source_mask_source_image = NULL;
	filter->param_source_mask_invert = NULL;
	filter->param_source_channel_multipliers = NULL;
	filter->param_source_multiplier = NULL;
	filter->param_source_threshold_value = NULL;
	filter->param_source_range_min = NULL;
	filter->param_source_range_max = NULL;
	filter->param_source_min_brightness = NULL;
	filter->param_source_max_brightness = NULL;
	filter->param_source_min_contrast = NULL;
	filter->param_source_max_contrast = NULL;
	filter->param_source_min_saturation = NULL;
	filter->param_source_max_saturation = NULL;
	filter->param_source_min_hue_shift = NULL;
	filter->param_source_max_hue_shift = NULL;

	filter->param_gradient_image = NULL;
	filter->param_gradient_width = NULL;
	filter->param_gradient_position = NULL;
	filter->param_gradient_rotation = NULL;
	filter->param_gradient_uv_size = NULL;
	filter->param_gradient_invert = NULL;
	filter->param_gradient_min_brightness = NULL;
	filter->param_gradient_max_brightness = NULL;
	filter->param_gradient_min_contrast = NULL;
	filter->param_gradient_max_contrast = NULL;
	filter->param_gradient_min_saturation = NULL;
	filter->param_gradient_max_saturation = NULL;
	filter->param_gradient_min_hue_shift = NULL;
	filter->param_gradient_max_hue_shift = NULL;

	load_effect_files(filter);
	obs_source_update(source, settings);
	return filter;
}

static void advanced_masks_destroy(void *data)
{
	// This function should clear up all memory the plugin uses.
	advanced_masks_data_t *filter = data;

	obs_enter_graphics();

	// EXAMPLE OF DESTROYING EFFECTS AND TEXRENDER
	if (filter->effect_rectangle_mask) {
		gs_effect_destroy(filter->effect_rectangle_mask);
	}
	if (filter->effect_circle_mask) {
		gs_effect_destroy(filter->effect_circle_mask);
	}
	if (filter->effect_source_mask) {
		gs_effect_destroy(filter->effect_source_mask);
	}

	if (filter->input_texrender) {
		gs_texrender_destroy(filter->input_texrender);
	}
	if (filter->output_texrender) {
		gs_texrender_destroy(filter->output_texrender);
	}
	if (filter->source_mask_texrender) {
		gs_texrender_destroy(filter->source_mask_texrender);
	}

	if (filter->mask_source_source) {
		obs_weak_source_release(filter->mask_source_source);
	}

	obs_leave_graphics();
	bfree(filter);
}

static uint32_t advanced_masks_width(void *data)
{
	advanced_masks_data_t *filter = data;
	return filter->width;
}

static uint32_t advanced_masks_height(void *data)
{
	advanced_masks_data_t *filter = data;
	return filter->height;
}

static void advanced_masks_update(void *data, obs_data_t *settings)
{
	// Called after UI is updated, should assign new UI values to
	// data structure pointers/values/etc..
	advanced_masks_data_t *filter = data;

	filter->mask_effect =
		(uint32_t)obs_data_get_int(settings, "mask_effect");

	filter->adj_brightness = obs_data_get_bool(settings, "brightness");
	filter->min_brightness =
		(float)obs_data_get_double(settings, "min_brightness_value");
	filter->max_brightness =
		(float)obs_data_get_double(settings, "max_brightness_value");

	filter->adj_contrast = obs_data_get_bool(settings, "contrast");
	filter->min_contrast =
		(float)obs_data_get_double(settings, "min_contrast_value");
	filter->max_contrast =
		(float)obs_data_get_double(settings, "max_contrast_value");

	filter->adj_saturation = obs_data_get_bool(settings, "saturation");
	filter->min_saturation =
		(float)obs_data_get_double(settings, "min_saturation_value");
	filter->max_saturation =
		(float)obs_data_get_double(settings, "max_saturation_value");

	filter->adj_hue_shift = obs_data_get_bool(settings, "hue_shift");
	filter->min_hue_shift =
		(float)obs_data_get_double(settings, "min_hue_shift_value");
	filter->max_hue_shift =
		(float)obs_data_get_double(settings, "max_hue_shift_value");

	filter->mask_type = (uint32_t)obs_data_get_int(settings, "mask_type");
	filter->mask_shape_type =
		(uint32_t)obs_data_get_int(settings, "shape_type");

	filter->mask_center.x =
		(float)obs_data_get_double(settings, "shape_center_x");
	filter->mask_center.y =
		(float)obs_data_get_double(settings, "shape_center_y");
	filter->global_position.x =
		(float)obs_data_get_double(settings, "position_x");
	filter->global_position.y =
		(float)obs_data_get_double(settings, "position_y");
	filter->global_scale =
		filter->mask_effect == MASK_EFFECT_ALPHA ? (float)obs_data_get_double(settings, "position_scale") : 100.0f;

	filter->rectangle_width =
		(float)obs_data_get_double(settings, "rectangle_width");
	filter->rectangle_height =
		(float)obs_data_get_double(settings, "rectangle_height");
	filter->zoom = filter->mask_effect == MASK_EFFECT_ALPHA
		? (float)obs_data_get_double(settings, "source_zoom") : 100.0f;

	filter->corner_radius_type =
		(uint32_t)obs_data_get_int(settings, "rectangle_corner_type");
	if (filter->corner_radius_type == MASK_CORNER_UNIFORM) {
		float r = (float)obs_data_get_double(settings,
						     "rectangle_corner_radius");
		vec4_set(&filter->rectangle_corner_radius, r, r, r, r);
		filter->rectangle_max_corner_radius = r;
	} else {
		vec4_set(&filter->rectangle_corner_radius,
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
			if (filter->rectangle_corner_radius.ptr[i] >
			    max_radius) {
				max_radius =
					filter->rectangle_corner_radius.ptr[i];
			}
		}
		filter->rectangle_max_corner_radius = max_radius;
	}
	filter->scale_type = (uint32_t)obs_data_get_int(settings, "scale_type");

	filter->radius = (float)obs_data_get_double(settings, "circle_radius");

	const char *mask_source_name =
		obs_data_get_string(settings, "mask_source");
	obs_source_t *mask_source =
		(mask_source_name && strlen(mask_source_name))
			? obs_get_source_by_name(mask_source_name)
			: NULL;

	if (mask_source) {
		obs_weak_source_release(filter->mask_source_source);
		filter->mask_source_source =
			obs_source_get_weak_source(mask_source);
		obs_source_release(mask_source);
	} else {
		filter->mask_source_source = NULL;
	}

	filter->source_mask_filter_type = (uint32_t)obs_data_get_int(
		settings, "mask_source_mask_properties_list");

	switch (filter->source_mask_filter_type) {
	case MASK_SOURCE_FILTER_ALPHA:
		vec4_set(&filter->channel_multipliers, 0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case MASK_SOURCE_FILTER_GRAYSCALE:
		vec4_set(&filter->channel_multipliers, 0.33333f, 0.33333f,
			 0.33334f, 0.0f);
		break;
	case MASK_SOURCE_FILTER_LUMINOSITY:
		vec4_set(&filter->channel_multipliers, 0.299f, 0.587f, 0.114f,
			 0.0f);
		break;
	case MASK_SOURCE_FILTER_SLIDERS:
		vec4_set(&filter->channel_multipliers,
			 (float)obs_data_get_double(settings,
						    "mask_source_filter_red"),
			 (float)obs_data_get_double(settings,
						    "mask_source_filter_green"),
			 (float)obs_data_get_double(settings,
						    "mask_source_filter_blue"),
			 (float)obs_data_get_double(
				 settings, "mask_source_filter_alpha"));
		break;
	}
	filter->multiplier = (float)obs_data_get_double(
		settings, "mask_source_filter_multiplier");
	filter->invert = obs_data_get_bool(settings, "source_invert");
	filter->compression_type = (uint32_t)obs_data_get_int(
		settings, "mask_source_compression_list");
	filter->threshold_value =
		(float)obs_data_get_double(settings, "source_threshold_value");
	filter->range_min =
		(float)obs_data_get_double(settings, "source_range_min");
	filter->range_max =
		(float)obs_data_get_double(settings, "source_range_max");

	filter->gradient_position =
		(float)obs_data_get_double(settings, "mask_gradient_position");
	filter->gradient_width =
		(float)obs_data_get_double(settings, "mask_gradient_width");
	filter->gradient_rotation =
		(float)obs_data_get_double(settings, "mask_gradient_rotation");
	filter->gradient_debug =
		obs_data_get_bool(settings, "mask_gradient_debug");
	filter->gradient_invert =
		obs_data_get_bool(settings, "gradient_invert");
}

static void advanced_masks_video_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);
	advanced_masks_data_t *filter = data;
	if (filter->rendered) {
		draw_output(filter);
		return;
	}

	filter->rendering = true;

	// 1. Get the input source as a texture renderer
	//    accessed as filter->input_texrender after call
	get_input_source(filter);
	if (!filter->input_texture_generated) {
		filter->rendering = false;
		obs_source_skip_video_filter(filter->context);
		return;
	}

	// 3. Create Stroke Mask
	// Call a rendering functioner, e.g.:
	render_mask(filter);

	// 3. Draw result (filter->output_texrender) to source
	draw_output(filter);
	filter->rendered = true;
	filter->rendering = false;
}

static void render_mask(advanced_masks_data_t *filter)
{
	switch (filter->mask_type) {
	case MASK_TYPE_SHAPE:
		render_shape_mask(filter);
		break;
	case MASK_TYPE_SOURCE:
		render_source_mask(filter);
		break;
	case MASK_TYPE_GRADIENT:
		render_gradient_mask(filter);
		break;
	}
}

static void render_shape_mask(advanced_masks_data_t *filter)
{
	switch (filter->mask_shape_type) {
	case SHAPE_RECTANGLE:
		render_rect_mask(filter);
		break;
	case SHAPE_CIRCLE:
		render_circle_mask(filter);
		break;
	}
}

static obs_properties_t *advanced_masks_properties(void *data)
{
	advanced_masks_data_t *filter = data;

	obs_property_t *p = NULL;

	obs_properties_t *props = obs_properties_create();
	obs_properties_set_param(props, filter, NULL);

	obs_property_t *mask_effect_list = obs_properties_add_list(
		props, "mask_effect", obs_module_text("AdvancedMasks.Effect"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(mask_effect_list,
				  obs_module_text(MASK_EFFECT_ALPHA_LABEL),
				  MASK_EFFECT_ALPHA);
	obs_property_list_add_int(mask_effect_list,
				  obs_module_text(MASK_EFFECT_ADJUSTMENT_LABEL),
				  MASK_EFFECT_ADJUSTMENT);

	obs_property_set_modified_callback(mask_effect_list,
					   setting_mask_effect_modified);

	obs_property_t *mask_type_list = obs_properties_add_list(
		props, "mask_type", obs_module_text("AdvancedMasks.Type"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_SHAPE_LABEL),
				  MASK_TYPE_SHAPE);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_SOURCE_LABEL),
				  MASK_TYPE_SOURCE);
	//obs_property_list_add_int(mask_type_list,
	//			  obs_module_text(MASK_TYPE_IMAGE_LABEL),
	//			  MASK_TYPE_IMAGE);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_GRADIENT_LABEL),
				  MASK_TYPE_GRADIENT);

	obs_property_set_modified_callback(mask_type_list,
					   setting_mask_type_modified);

	obs_property_t *mask_source = obs_properties_add_list(
		props, "mask_source",
		obs_module_text("AdvancedMasks.SourceMask.Source"),
		OBS_COMBO_TYPE_EDITABLE, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(
		mask_source, obs_module_text("AdvancedMasks.Common.None"), "");
	obs_enum_sources(add_source_to_list, mask_source);
	obs_enum_scenes(add_source_to_list, mask_source);

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


	obs_properties_t *mask_adjustments_group = obs_properties_create();

	p = obs_properties_add_bool(
		mask_adjustments_group, "brightness",
		obs_module_text("AdvancedMasks.Adjustments.Brightness"));

	obs_property_set_modified_callback(p,
					   setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_brightness_value",
		obs_module_text("AdvancedMasks.Adjustments.MinBrightness"),
		-1.0, 1.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_brightness_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxBrightness"),
		-1.0, 1.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "contrast",
		obs_module_text("AdvancedMasks.Adjustments.Contrast"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_contrast_value",
		obs_module_text("AdvancedMasks.Adjustments.MinContrast"),
		-4.0, 4.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_contrast_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxContrast"),
		-4.0, 4.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "saturation",
		obs_module_text("AdvancedMasks.Adjustments.Saturation"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_saturation_value",
		obs_module_text("AdvancedMasks.Adjustments.MinSaturation"),
		0.0, 5.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_saturation_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxSaturation"),
		0.0, 5.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "hue_shift",
		obs_module_text("AdvancedMasks.Adjustments.HueShift"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_hue_shift_value",
		obs_module_text("AdvancedMasks.Adjustments.MinHueShift"), -360.0,
		360.0, 0.1);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_hue_shift_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxHueShift"), -360.0,
		360.0, 0.1);

	obs_properties_add_group(
		props, "mask_adjustments_group",
		obs_module_text("AdvancedMasks.Adjustments.Label"),
		OBS_GROUP_NORMAL, mask_adjustments_group);

	obs_properties_t *mask_gradient_group = obs_properties_create();

	p = obs_properties_add_float_slider(
		mask_gradient_group, "mask_gradient_width",
		obs_module_text("AdvancedMasks.GradientMask.Width"), 0, 4000.0,
		1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_gradient_group, "mask_gradient_position",
		obs_module_text("AdvancedMasks.GradientMask.Position"), -6000.0,
		6000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_gradient_group, "mask_gradient_rotation",
		obs_module_text("AdvancedMasks.GradientMask.Rotation"), -360.0,
		360.0, 0.1);
	obs_property_float_set_suffix(p, "deg");

	obs_properties_add_bool(
		mask_gradient_group, "gradient_invert",
		obs_module_text("AdvancedMasks.GradientMask.Invert"));

	obs_properties_add_bool(
		mask_gradient_group, "mask_gradient_debug",
		obs_module_text("AdvancedMasks.GradientMask.DebugLines"));

	obs_properties_add_group(
		props, "mask_gradient_group",
		obs_module_text("AdvancedMasks.GradientMask.Label"),
		OBS_GROUP_NORMAL, mask_gradient_group);

	// START OF SOURCE STUFF
	obs_properties_t *mask_source_group = obs_properties_create();

	obs_property_t *mask_source_filter_list = obs_properties_add_list(
		mask_source_group, "mask_source_mask_properties_list",
		obs_module_text("AdvancedMasks.SourceMask.Properties"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		mask_source_filter_list,
		obs_module_text(MASK_SOURCE_FILTER_ALPHA_LABEL),
		MASK_SOURCE_FILTER_ALPHA);
	obs_property_list_add_int(
		mask_source_filter_list,
		obs_module_text(MASK_SOURCE_FILTER_GRAYSCALE_LABEL),
		MASK_SOURCE_FILTER_GRAYSCALE);
	obs_property_list_add_int(
		mask_source_filter_list,
		obs_module_text(MASK_SOURCE_FILTER_LUMINOSITY_LABEL),
		MASK_SOURCE_FILTER_LUMINOSITY);
	obs_property_list_add_int(
		mask_source_filter_list,
		obs_module_text(MASK_SOURCE_FILTER_SLIDERS_LABEL),
		MASK_SOURCE_FILTER_SLIDERS);

	obs_property_set_modified_callback(mask_source_filter_list,
					   setting_mask_source_filter_modified);

	obs_properties_add_float_slider(
		mask_source_group, "mask_source_filter_red",
		obs_module_text("AdvancedMasks.SourceMask.Channel.Red"),
		-100.01, 100.01, 0.01);

	obs_properties_add_float_slider(
		mask_source_group, "mask_source_filter_green",
		obs_module_text("AdvancedMasks.SourceMask.Channel.Green"),
		-100.01, 100.01, 0.01);

	obs_properties_add_float_slider(
		mask_source_group, "mask_source_filter_blue",
		obs_module_text("AdvancedMasks.SourceMask.Channel.Blue"),
		-100.01, 100.01, 0.01);

	obs_properties_add_float_slider(
		mask_source_group, "mask_source_filter_alpha",
		obs_module_text("AdvancedMasks.SourceMask.Channel.Alpha"),
		-100.01, 100.01, 0.01);

	obs_properties_add_float_slider(
		mask_source_group, "mask_source_filter_multiplier",
		obs_module_text("AdvancedMasks.SourceMask.Source.Multiplier"),
		-100.01, 100.01, 0.01);

	obs_properties_add_bool(
		mask_source_group, "source_invert",
		obs_module_text("AdvancedMasks.SourceMask.Invert"));

	obs_properties_add_group(
		props, "mask_source_group",
		obs_module_text("AdvancedMasks.SourceMask.SourceParameters"),
		OBS_GROUP_NORMAL, mask_source_group);

	obs_properties_t *mask_source_compression_group =
		obs_properties_create();

	obs_property_t *mask_source_compression_list = obs_properties_add_list(
		mask_source_compression_group, "mask_source_compression_list",
		obs_module_text("AdvancedMasks.SourceMask.CompressionTypes"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		mask_source_compression_list,
		obs_module_text(MASK_SOURCE_COMPRESSION_NONE_LABEL),
		MASK_SOURCE_COMPRESSION_NONE);

	obs_property_list_add_int(
		mask_source_compression_list,
		obs_module_text(MASK_SOURCE_COMPRESSION_THRESHOLD_LABEL),
		MASK_SOURCE_COMPRESSION_THRESHOLD);

	obs_property_list_add_int(
		mask_source_compression_list,
		obs_module_text(MASK_SOURCE_COMPRESSION_RANGE_LABEL),
		MASK_SOURCE_COMPRESSION_RANGE);

	obs_property_set_modified_callback(
		mask_source_compression_list,
		setting_mask_source_compression_modified);

	obs_properties_add_float_slider(
		mask_source_compression_group, "source_threshold_value",
		obs_module_text("AdvancedMasks.SourceMask.ThresholdValue"), 0.0,
		1.0, 0.01);

	obs_properties_add_float_slider(
		mask_source_compression_group, "source_range_min",
		obs_module_text("AdvancedMasks.SourceMask.RangeMin"), 0.0, 1.0,
		0.01);

	obs_properties_add_float_slider(
		mask_source_compression_group, "source_range_max",
		obs_module_text("AdvancedMasks.SourceMask.RangeMax"), 0.0, 1.0,
		0.01);

	obs_properties_add_group(
		props, "source_mask_compression_group",
		obs_module_text("AdvancedMasks.SourceMaskCompress"),
		OBS_GROUP_NORMAL, mask_source_compression_group);

	// START OF SHAPE STUFF


	// START OF SHAPE - RECTANGLE

	obs_properties_t *source_rect_mask_group = obs_properties_create();

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
	if (filter && filter->context) {
		obs_data_t *settings = obs_source_get_settings(filter->context);
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

	return props;
}

static bool setting_shape_type_modified(obs_properties_t *props,
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
	setting_visibility("source_zoom", effect_type == MASK_EFFECT_ALPHA, props);
	obs_property_t *group = obs_properties_get(props, "rectangle_source_group");
	const char *group_name =
		effect_type == MASK_EFFECT_ALPHA
			? obs_module_text("AdvancedMasks.Shape.Rectangle.SourceGroup")
			: obs_module_text("AdvancedMasks.Shape.Rectangle.GeometryGroup");
	obs_property_set_description(group, group_name);
	return true;
}

static bool setting_mask_source_compression_modified(obs_properties_t *props,
						     obs_property_t *p,
						     obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int filter_type =
		(int)obs_data_get_int(settings, "mask_source_compression_list");
	switch (filter_type) {
	case MASK_SOURCE_COMPRESSION_NONE:
		setting_visibility("source_threshold_value", false, props);
		setting_visibility("source_range_min", false, props);
		setting_visibility("source_range_max", false, props);
		break;
	case MASK_SOURCE_COMPRESSION_THRESHOLD:
		setting_visibility("source_threshold_value", true, props);
		setting_visibility("source_range_min", false, props);
		setting_visibility("source_range_max", false, props);
		break;
	case MASK_SOURCE_COMPRESSION_RANGE:
		setting_visibility("source_threshold_value", false, props);
		setting_visibility("source_range_min", true, props);
		setting_visibility("source_range_max", true, props);
		break;
	}

	return true;
}

static bool setting_mask_source_filter_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int filter_type = (int)obs_data_get_int(
		settings, "mask_source_mask_properties_list");
	switch (filter_type) {
	case MASK_SOURCE_FILTER_ALPHA:
	case MASK_SOURCE_FILTER_GRAYSCALE:
	case MASK_SOURCE_FILTER_LUMINOSITY:
		setting_visibility("mask_source_filter_red", false, props);
		setting_visibility("mask_source_filter_green", false, props);
		setting_visibility("mask_source_filter_blue", false, props);
		setting_visibility("mask_source_filter_alpha", false, props);
		break;
	case MASK_SOURCE_FILTER_SLIDERS:
		setting_visibility("mask_source_filter_red", true, props);
		setting_visibility("mask_source_filter_green", true, props);
		setting_visibility("mask_source_filter_blue", true, props);
		setting_visibility("mask_source_filter_alpha", true, props);
		break;
	}

	return true;
}

static bool setting_mask_adjustment_modified(obs_properties_t *props,
						obs_property_t *p,
						obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	bool brightness = obs_data_get_bool(settings, "brightness");
	bool contrast = obs_data_get_bool(settings, "contrast");
	bool saturation = obs_data_get_bool(settings, "saturation");
	bool hue_shift = obs_data_get_bool(settings, "hue_shift");
	setting_visibility("min_brightness_value", brightness, props);
	setting_visibility("max_brightness_value", brightness, props);
	setting_visibility("min_contrast_value", contrast, props);
	setting_visibility("max_contrast_value", contrast, props);
	setting_visibility("min_saturation_value", saturation, props);
	setting_visibility("max_saturation_value", saturation, props);
	setting_visibility("min_hue_shift_value", hue_shift, props);
	setting_visibility("max_hue_shift_value", hue_shift, props);
	return true;
}

static bool setting_mask_effect_modified(obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int mask_effect = (int)obs_data_get_int(settings, "mask_effect");
	switch (mask_effect) {
	case MASK_EFFECT_ADJUSTMENT:
		setting_visibility("mask_adjustments_group", true, props);
		break;
	case MASK_EFFECT_ALPHA:
		setting_visibility("mask_adjustments_group", false, props);
		break;
	}
	setting_mask_type_modified(props, p, settings);
	setting_shape_type_modified(props, p, settings);
	return true;
}

static bool setting_mask_type_modified(obs_properties_t *props,
				       obs_property_t *p, obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int mask_type = (int)obs_data_get_int(settings, "mask_type");
	int effect_type = (int)obs_data_get_int(settings, "mask_effect");
	switch (mask_type) {
	case MASK_TYPE_SHAPE:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_group", false, props);
		setting_visibility("source_mask_compression_group", false,
				   props);
		setting_visibility("shape_type", true, props);
		setting_visibility("rectangle_source_group", true, props);
		setting_visibility("rectangle_rounded_corners_group", true,
				   props);
		setting_visibility("scale_position_group",
				   effect_type == MASK_EFFECT_ALPHA, props);
		setting_visibility("mask_gradient_group", false, props);
		return true;
	case MASK_TYPE_SOURCE:
		setting_visibility("mask_source", true, props);
		setting_visibility("mask_source_group", true, props);
		setting_visibility("source_mask_compression_group", true,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("scale_position_group", false, props);
		setting_mask_source_filter_modified(props, p, settings);
		setting_visibility("mask_gradient_group", false, props);
		return true;
	case MASK_TYPE_GRADIENT:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_group", false, props);
		setting_visibility("source_mask_compression_group", false,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("scale_position_group", false, props);
		setting_visibility("mask_gradient_group", true, props);
		return true;
	}
	return false;
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
	advanced_masks_data_t *filter = data;
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

static void advanced_masks_video_tick(void *data, float seconds)
{
	UNUSED_PARAMETER(seconds);
	advanced_masks_data_t *filter = data;

	obs_source_t *target = obs_filter_get_target(filter->context);
	if (!target) {
		return;
	}
	filter->width = (uint32_t)obs_source_get_base_width(target);
	filter->height = (uint32_t)obs_source_get_base_height(target);

	filter->rendered = false;
	filter->input_texture_generated = false;
}

static void advanced_masks_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "mask_effect", MASK_EFFECT_ALPHA);
	obs_data_set_default_bool(settings, "brightness", false);
	obs_data_set_default_double(settings, "min_brightness_value", -1.0);
	obs_data_set_default_double(settings, "max_brightness_value", 1.0);
	obs_data_set_default_bool(settings, "contrast", false);
	obs_data_set_default_double(settings, "min_contrast_value", 0.0);
	obs_data_set_default_double(settings, "max_contrast_value", 1.0);
	obs_data_set_default_bool(settings, "saturation", false);
	obs_data_set_default_double(settings, "min_saturation_value", 0.0);
	obs_data_set_default_double(settings, "max_saturation_value", 4.0);
	obs_data_set_default_bool(settings, "hue_shift", false);
	obs_data_set_default_double(settings, "min_hue_shift_value", -180.0);
	obs_data_set_default_double(settings, "max_hue_shift_value", 180.0);

	obs_data_set_default_int(settings, "mask_type", MASK_TYPE_SHAPE);
	obs_data_set_default_int(settings, "shape_type", SHAPE_RECTANGLE);
	obs_data_set_default_double(settings, "shape_center_x", 960.0);
	obs_data_set_default_double(settings, "shape_center_y", 540.0);
	obs_data_set_default_double(settings, "rectangle_width", 250.0);
	obs_data_set_default_double(settings, "rectangle_height", 250.0);
	obs_data_set_default_double(settings, "position_x", 960.0);
	obs_data_set_default_double(settings, "position_y", 540.0);
	obs_data_set_default_double(settings, "position_scale", 100.0);
	obs_data_set_default_double(settings, "mask_source_filter_multiplier",
				    1.0);
	obs_data_set_default_double(settings, "source_zoom", 100.0);

	obs_data_set_default_bool(settings, "gradient_invert", false);
}

static void get_input_source(advanced_masks_data_t *filter)
{
	gs_effect_t *pass_through = obs_get_base_effect(OBS_EFFECT_DEFAULT);

	filter->input_texrender =
		create_or_reset_texrender(filter->input_texrender);
	if (obs_source_process_filter_begin(filter->context, GS_RGBA,
					    OBS_ALLOW_DIRECT_RENDERING) &&
	    gs_texrender_begin(filter->input_texrender, filter->width,
			       filter->height)) {

		set_blending_parameters();
		gs_ortho(0.0f, (float)filter->width, 0.0f,
			 (float)filter->height, -100.0f, 100.0f);
		obs_source_process_filter_end(filter->context, pass_through,
					      filter->width, filter->height);
		gs_texrender_end(filter->input_texrender);
		gs_blend_state_pop();
		filter->input_texture_generated = true;
	}
}

static void draw_output(advanced_masks_data_t *filter)
{
	gs_texture_t *texture =
		gs_texrender_get_texture(filter->output_texrender);
	gs_effect_t *pass_through = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	gs_eparam_t *param = gs_effect_get_param_by_name(pass_through, "image");
	gs_effect_set_texture(param, texture);
	uint32_t width = gs_texture_get_width(texture);
	uint32_t height = gs_texture_get_height(texture);
	while (gs_effect_loop(pass_through, "Draw")) {
		gs_draw_sprite(texture, 0, width, height);
	}
}

static void advanced_masks_render_filter(advanced_masks_data_t *filter)
{
	gs_texrender_t *tmp = filter->output_texrender;
	filter->output_texrender = filter->input_texrender;
	filter->input_texrender = tmp;
}

static void load_effect_files(advanced_masks_data_t *filter)
{
	shape_load_rectangle_effect(filter);
	load_source_mask_effect(filter);
	load_circle_mask_effect(filter);
	load_gradient_mask_effect(filter);
}

static void shape_load_rectangle_effect(advanced_masks_data_t *filter)
{
	const char *effect_file_path = "/shaders/rectangular-mask.effect";

	filter->effect_rectangle_mask = load_shader_effect(
		filter->effect_rectangle_mask, effect_file_path);
	if (filter->effect_rectangle_mask) {
		size_t effect_count =
			gs_effect_get_num_params(filter->effect_rectangle_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				filter->effect_rectangle_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				filter->param_rectangle_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				filter->param_rectangle_uv_size = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				filter->param_rectangle_mask_position = param;
			} else if (strcmp(info.name, "width") == 0) {
				filter->param_rectangle_width = param;
			} else if (strcmp(info.name, "height") == 0) {
				filter->param_rectangle_height = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				filter->param_global_position = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				filter->param_global_scale = param;
			} else if (strcmp(info.name, "corner_radius") == 0) {
				filter->param_corner_radius = param;
			} else if (strcmp(info.name, "max_corner_radius") == 0) {
				filter->param_max_corner_radius = param;
			} else if (strcmp(info.name, "aspect_ratio") == 0) {
				filter->param_rect_aspect_ratio = param;
			} else if (strcmp(info.name, "aa_scale") == 0) {
				filter->param_rectangle_aa_scale = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				filter->param_rectangle_zoom = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				filter->param_rectangle_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				filter->param_rectangle_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				filter->param_rectangle_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				filter->param_rectangle_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				filter->param_rectangle_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				filter->param_rectangle_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				filter->param_rectangle_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				filter->param_rectangle_max_hue_shift = param;
			}
		}
	}
}

static void render_rect_mask(advanced_masks_data_t *data)
{
	gs_effect_t *effect = data->effect_rectangle_mask;
	gs_texture_t *texture = gs_texrender_get_texture(data->input_texrender);
	if (!effect || !texture) {
		return;
	}

	data->output_texrender =
		create_or_reset_texrender(data->output_texrender);

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
		gs_effect_set_vec2(data->param_global_position,
				   &data->global_position);

	}

	if (data->param_global_scale) {

		gs_effect_set_float(data->param_global_scale, scale_factor);
	}

	if (data->param_corner_radius) {
		struct vec4 corner_radius;
		vec4_divf(&corner_radius, &data->rectangle_corner_radius,
				  scale_factor * (data->zoom / 100.0f));
		gs_effect_set_vec4(data->param_corner_radius, &corner_radius);
	}

	if (data->param_max_corner_radius) {
		float max_corner_radius =
			data->rectangle_max_corner_radius / scale_factor *
			(data->zoom / 100.0f);
		gs_effect_set_float(data->param_max_corner_radius,
				    max_corner_radius);
	}

	if (data->param_rect_aspect_ratio) {
		struct vec2 box_ar;
		box_ar.x =
			(float)data->width /
			(float)fmin((double)data->width, (double)data->height);
		box_ar.y =
			(float)data->height /
			(float)fmin((double)data->width, (double)data->height);

		gs_effect_set_vec2(data->param_rect_aspect_ratio, &box_ar);
	}

	if (data->param_rectangle_aa_scale) {
		float aa_scale = 5.0f / (float)data->height;
		gs_effect_set_float(data->param_rectangle_aa_scale, aa_scale);
	}

	if (data->param_rectangle_min_brightness) {
		const float min_brightness =
			data->adj_brightness ? data->min_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_brightness,
				    min_brightness);
	}

	if (data->param_rectangle_max_brightness) {
		const float max_brightness =
			data->adj_brightness ? data->max_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_brightness,
				    max_brightness);
	}

	if (data->param_rectangle_min_contrast) {
		const float min_contrast =
			data->adj_contrast ? data->min_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_contrast,
				    min_contrast);
	}

	if (data->param_rectangle_max_contrast) {
		const float max_contrast =
			data->adj_contrast ? data->max_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_contrast,
				    max_contrast);
	}

	if (data->param_rectangle_min_saturation) {
		const float min_saturation =
			data->adj_saturation ? data->min_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_min_saturation,
				    min_saturation);
	}

	if (data->param_rectangle_max_saturation) {
		const float max_saturation =
			data->adj_saturation ? data->max_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_saturation,
				    max_saturation);
	}

	if (data->param_rectangle_min_hue_shift) {
		const float min_hue_shift =
			data->adj_hue_shift ? data->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_rectangle_max_hue_shift) {
		const float max_hue_shift =
			data->adj_hue_shift ? data->max_hue_shift : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_rectangle_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)data->width;
		uv_size.y = (float)data->height;
		gs_effect_set_vec2(data->param_rectangle_uv_size, &uv_size);
	}

	set_blending_parameters();
	const char *technique = data->mask_effect == MASK_EFFECT_ALPHA
					? "Alpha"
					: "Adjustments";


	if (gs_texrender_begin(data->output_texrender, data->width,
			       data->height)) {
		gs_ortho(0.0f, (float)data->width, 0.0f, (float)data->height,
			 -100.0f, 100.0f);  
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, data->width, data->height);
		gs_texrender_end(data->output_texrender);
	}

	gs_blend_state_pop();
}

static void render_circle_mask(advanced_masks_data_t *data)
{
	gs_effect_t *effect = data->effect_rectangle_mask;
	gs_texture_t *texture = gs_texrender_get_texture(data->input_texrender);
	if (!effect || !texture) {
		return;
	}

	data->output_texrender =
		create_or_reset_texrender(data->output_texrender);

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
		gs_effect_set_vec2(data->param_global_position,
				   &data->global_position);
	}

	if (data->param_global_scale) {

		gs_effect_set_float(data->param_global_scale, scale_factor);
	}

	if (data->param_corner_radius) {
		struct vec4 corner_radius;
		float r = data->radius / (data->zoom / 100.0f);
		vec4_set(&corner_radius, r, r, r, r);
		gs_effect_set_vec4(data->param_corner_radius, &corner_radius);
	}

	if (data->param_max_corner_radius) {
		float max_corner_radius =
			data->radius / (data->zoom / 100.0f);
		gs_effect_set_float(data->param_max_corner_radius,
				    max_corner_radius);
	}

	if (data->param_rect_aspect_ratio) {
		struct vec2 box_ar;
		box_ar.x =
			(float)data->width /
			(float)fmin((double)data->width, (double)data->height);
		box_ar.y =
			(float)data->height /
			(float)fmin((double)data->width, (double)data->height);

		gs_effect_set_vec2(data->param_rect_aspect_ratio, &box_ar);
	}

	if (data->param_rectangle_aa_scale) {
		float aa_scale = 5.0f / (float)data->height;
		gs_effect_set_float(data->param_rectangle_aa_scale, aa_scale);
	}

	if (data->param_rectangle_min_brightness) {
		const float min_brightness =
			data->adj_brightness ? data->min_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_brightness,
				    min_brightness);
	}

	if (data->param_rectangle_max_brightness) {
		const float max_brightness =
			data->adj_brightness ? data->max_brightness : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_brightness,
				    max_brightness);
	}

	if (data->param_rectangle_min_contrast) {
		const float min_contrast =
			data->adj_contrast ? data->min_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_contrast,
				    min_contrast);
	}

	if (data->param_rectangle_max_contrast) {
		const float max_contrast =
			data->adj_contrast ? data->max_contrast : 0.0f;
		gs_effect_set_float(data->param_rectangle_max_contrast,
				    max_contrast);
	}

	if (data->param_rectangle_min_saturation) {
		const float min_saturation =
			data->adj_saturation ? data->min_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_min_saturation,
				    min_saturation);
	}

	if (data->param_rectangle_max_saturation) {
		const float max_saturation =
			data->adj_saturation ? data->max_saturation : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_saturation,
				    max_saturation);
	}

	if (data->param_rectangle_min_hue_shift) {
		const float min_hue_shift =
			data->adj_hue_shift ? data->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_rectangle_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_rectangle_max_hue_shift) {
		const float max_hue_shift =
			data->adj_hue_shift ? data->max_hue_shift : 1.0f;
		gs_effect_set_float(data->param_rectangle_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_rectangle_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)data->width;
		uv_size.y = (float)data->height;
		gs_effect_set_vec2(data->param_rectangle_uv_size, &uv_size);
	}

	set_blending_parameters();

	const char *technique = data->mask_effect == MASK_EFFECT_ALPHA
					? "Alpha"
					: "Adjustments";

	if (gs_texrender_begin(data->output_texrender, data->width,
			       data->height)) {
		gs_ortho(0.0f, (float)data->width, 0.0f, (float)data->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, data->width, data->height);
		gs_texrender_end(data->output_texrender);
	}

	gs_blend_state_pop();
}

// SOURCE MASK STUFF
static void load_source_mask_effect(advanced_masks_data_t *filter)
{
	const char *effect_file_path = "/shaders/source-mask.effect";

	filter->effect_source_mask = load_shader_effect(
		filter->effect_source_mask, effect_file_path);
	if (filter->effect_source_mask) {
		size_t effect_count =
			gs_effect_get_num_params(filter->effect_source_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				filter->effect_source_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				filter->param_source_mask_image = param;
			} else if (strcmp(info.name, "source_image") == 0) {
				filter->param_source_mask_source_image = param;
			} else if (strcmp(info.name, "invert") == 0) {
				filter->param_source_mask_invert = param;
			} else if (strcmp(info.name, "channel_multipliers") == 0) {
				filter->param_source_channel_multipliers = param;
			} else if (strcmp(info.name, "multiplier") == 0) {
				filter->param_source_multiplier = param;
			} else if (strcmp(info.name, "threshold_value") == 0) {
				filter->param_source_threshold_value = param;
			} else if (strcmp(info.name, "range_min") == 0) {
				filter->param_source_range_min = param;
			} else if (strcmp(info.name, "range_max") == 0) {
				filter->param_source_range_max = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				filter->param_source_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				filter->param_source_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				filter->param_source_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				filter->param_source_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				filter->param_source_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				filter->param_source_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				filter->param_source_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				filter->param_source_max_hue_shift = param;
			}
		}
	}
}

static void render_source_mask(advanced_masks_data_t *data)
{
	gs_effect_t *effect = data->effect_source_mask;
	gs_texture_t *texture = gs_texrender_get_texture(data->input_texrender);
	if (!effect || !texture) {
		return;
	}

	data->output_texrender =
		create_or_reset_texrender(data->output_texrender);

	if (data->param_source_mask_image) {
		gs_effect_set_texture(data->param_source_mask_image, texture);
	}

	if (data->param_source_mask_invert) {
		gs_effect_set_bool(data->param_source_mask_invert,
				   data->invert);
	}

	if (data->param_source_channel_multipliers) {
		gs_effect_set_vec4(data->param_source_channel_multipliers,
				   &data->channel_multipliers);
	}

	if (data->param_source_multiplier) {
		gs_effect_set_float(data->param_source_multiplier,
				    data->multiplier);
	}

	if (data->param_source_threshold_value) {
		gs_effect_set_float(data->param_source_threshold_value,
				    data->threshold_value);
	}

	if (data->param_source_range_min) {
		gs_effect_set_float(data->param_source_range_min,
				    data->range_min);
	}

	if (data->param_source_range_max) {
		gs_effect_set_float(data->param_source_range_max,
				    data->range_max);
	}

	if (data->param_source_min_brightness) {
		const float min_brightness =
			data->adj_brightness ? data->min_brightness : 0.0f;
		gs_effect_set_float(data->param_source_min_brightness,
				    min_brightness);
	}

	if (data->param_source_max_brightness) {
		const float max_brightness =
			data->adj_brightness ? data->max_brightness : 0.0f;
		gs_effect_set_float(data->param_source_max_brightness,
				    max_brightness);
	}

	if (data->param_source_min_contrast) {
		const float min_contrast =
			data->adj_contrast ? data->min_contrast : 0.0f;
		gs_effect_set_float(data->param_source_min_contrast,
				    min_contrast);
	}

	if (data->param_source_max_contrast) {
		const float max_contrast = data->adj_contrast ? data->max_contrast
							: 0.0f;
		gs_effect_set_float(data->param_source_max_contrast,
				    max_contrast);
	}

	if (data->param_source_min_saturation) {
		const float min_saturation = data->adj_saturation ? data->min_saturation
							: 1.0f;
		gs_effect_set_float(data->param_source_min_saturation,
				    min_saturation);
	}

	if (data->param_source_max_saturation) {
		const float max_saturation = data->adj_saturation ? data->max_saturation
							: 1.0f;
		gs_effect_set_float(data->param_source_max_saturation,
				    max_saturation);
	}

	if (data->param_source_min_hue_shift) {
		const float min_hue_shift =
			data->adj_hue_shift ? data->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_source_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_source_max_hue_shift) {
		const float max_hue_shift =
			data->adj_hue_shift ? data->max_hue_shift : 1.0f;
		gs_effect_set_float(data->param_source_max_hue_shift,
				    max_hue_shift);
	}

	gs_texrender_t *mask_source_render = NULL;
	obs_source_t *source =
		data->mask_source_source
			? obs_weak_source_get_source(data->mask_source_source)
			: NULL;
	if (!source) {
		return;
	}

	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};
	const enum gs_color_space space = obs_source_get_color_space(
		source, OBS_COUNTOF(preferred_spaces), preferred_spaces);
	const enum gs_color_format format = gs_get_format_from_space(space);

	// Set up a tex renderer for source
	mask_source_render = gs_texrender_create(format, GS_ZS_NONE);
	uint32_t base_width = obs_source_get_width(source);
	uint32_t base_height = obs_source_get_height(source);
	gs_blend_state_push();
	gs_blend_function(GS_BLEND_ONE, GS_BLEND_ZERO);
	if (gs_texrender_begin_with_color_space(mask_source_render, base_width,
						base_height, space)) {
		const float w = (float)base_width;
		const float h = (float)base_height;
		struct vec4 clear_color;

		vec4_zero(&clear_color);
		gs_clear(GS_CLEAR_COLOR, &clear_color, 0.0f, 0);
		gs_ortho(0.0f, w, 0.0f, h, -100.0f, 100.0f);
		obs_source_video_render(source);
		gs_texrender_end(mask_source_render);
	}
	gs_blend_state_pop();
	obs_source_release(source);
	gs_texture_t *source_texture =
		gs_texrender_get_texture(mask_source_render);

	if (data->param_source_mask_source_image) {
		gs_effect_set_texture(data->param_source_mask_source_image,
				      source_texture);
	}

	set_blending_parameters();

	char technique[32];
	strcpy(technique, data->mask_effect == MASK_EFFECT_ADJUSTMENT
				  ? "Adjustments"
				  : "Alpha");
	char *techniqueType =
		data->compression_type == MASK_SOURCE_COMPRESSION_THRESHOLD
				? "Threshold"
			: data->compression_type == MASK_SOURCE_COMPRESSION_RANGE
				? "Range"
			: "";
	strcat(technique, techniqueType);

	if (gs_texrender_begin(data->output_texrender, data->width,
			       data->height)) {
		gs_ortho(0.0f, (float)data->width, 0.0f, (float)data->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, data->width, data->height);
		gs_texrender_end(data->output_texrender);
	}
	gs_texrender_destroy(mask_source_render);
	gs_blend_state_pop();
}

// CIRCLE MASK STUFF
static void load_circle_mask_effect(advanced_masks_data_t *filter)
{
	const char *effect_file_path = "/shaders/circle-mask.effect";

	filter->effect_circle_mask = load_shader_effect(
		filter->effect_circle_mask, effect_file_path);
	if (filter->effect_circle_mask) {
		size_t effect_count =
			gs_effect_get_num_params(filter->effect_circle_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				filter->effect_circle_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				filter->param_circle_image = param;
			} else if (strcmp(info.name, "radius") == 0) {
				filter->param_circle_radius = param;
			} else if (strcmp(info.name, "zoom") == 0) {
				filter->param_circle_zoom = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				filter->param_circle_mask_position = param;
			} else if (strcmp(info.name, "global_position") == 0) {
				filter->param_circle_global_position = param;
			} else if (strcmp(info.name, "aspect_ratio") == 0) {
				filter->param_circle_aspect_ratio = param;
			} else if (strcmp(info.name, "global_scale") == 0) {
				filter->param_circle_global_scale = param;
			}
		}
	}
}

static void load_gradient_mask_effect(advanced_masks_data_t *filter)
{
	const char *effect_file_path = "/shaders/gradient-mask.effect";

	filter->effect_gradient_mask = load_shader_effect(
		filter->effect_gradient_mask, effect_file_path);
	if (filter->effect_gradient_mask) {
		size_t effect_count =
			gs_effect_get_num_params(filter->effect_gradient_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				filter->effect_gradient_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				filter->param_gradient_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				filter->param_gradient_uv_size = param;
			} else if (strcmp(info.name, "width") == 0) {
				filter->param_gradient_width = param;
			} else if (strcmp(info.name, "position") == 0) {
				filter->param_gradient_position = param;
			} else if (strcmp(info.name, "rotation") == 0) {
				filter->param_gradient_rotation = param;
			} else if (strcmp(info.name, "invert") == 0) {
				filter->param_gradient_invert = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				filter->param_gradient_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				filter->param_gradient_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				filter->param_gradient_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				filter->param_gradient_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				filter->param_gradient_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				filter->param_gradient_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				filter->param_gradient_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				filter->param_gradient_max_hue_shift = param;
			}
		}
	}
}

static void render_gradient_mask(advanced_masks_data_t *data)
{
	gs_effect_t *effect = data->effect_gradient_mask;
	gs_texture_t *texture = gs_texrender_get_texture(data->input_texrender);
	if (!effect || !texture) {
		return;
	}

	data->output_texrender =
		create_or_reset_texrender(data->output_texrender);

	if (data->param_gradient_image) {
		gs_effect_set_texture(data->param_gradient_image, texture);
	}

	if (data->param_gradient_width) {
		gs_effect_set_float(data->param_gradient_width,
				    data->gradient_width);
	}

	if (data->param_gradient_invert) {
		gs_effect_set_bool(data->param_gradient_invert, data->gradient_invert);
	}

	if (data->param_gradient_position) {
		const float position =
			data->gradient_position - (float)data->width / 2.0f;
		gs_effect_set_float(data->param_gradient_position, position);
	}

	if (data->param_gradient_rotation) {
		const float rotation = data->gradient_rotation * M_PI / 180.0f;
		gs_effect_set_float(data->param_gradient_rotation, rotation);
	}

	if (data->param_gradient_min_brightness) {
		const float min_brightness =
			data->adj_brightness ? data->min_brightness : 0.0f;
		gs_effect_set_float(data->param_gradient_min_brightness,
				    min_brightness);
	}

	if (data->param_gradient_max_brightness) {
		const float max_brightness =
			data->adj_brightness ? data->max_brightness : 0.0f;
		gs_effect_set_float(data->param_gradient_max_brightness,
				    max_brightness);
	}

	if (data->param_gradient_min_contrast) {
		const float min_contrast =
			data->adj_contrast ? data->min_contrast : 0.0f;
		gs_effect_set_float(data->param_gradient_min_contrast,
				    min_contrast);
	}

	if (data->param_gradient_max_contrast) {
		const float max_contrast =
			data->adj_contrast ? data->max_contrast : 0.0f;
		gs_effect_set_float(data->param_gradient_max_contrast,
				    max_contrast);
	}

	if (data->param_gradient_min_saturation) {
		const float min_saturation =
			data->adj_saturation ? data->min_saturation : 1.0f;
		gs_effect_set_float(data->param_gradient_min_saturation,
				    min_saturation);
	}

	if (data->param_gradient_max_saturation) {
		const float max_saturation =
			data->adj_saturation ? data->max_saturation : 1.0f;
		gs_effect_set_float(data->param_gradient_max_saturation,
				    max_saturation);
	}

	if (data->param_gradient_min_hue_shift) {
		const float min_hue_shift =
			data->adj_hue_shift ? data->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_gradient_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_gradient_max_hue_shift) {
		const float max_hue_shift =
			data->adj_hue_shift ? data->max_hue_shift : 1.0f;
		gs_effect_set_float(data->param_gradient_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_gradient_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)data->width;
		uv_size.y = (float)data->height;
		gs_effect_set_vec2(data->param_gradient_uv_size, &uv_size);
	}

	set_render_parameters();
	set_blending_parameters();
	char technique[32];
	strcpy(technique, data->gradient_debug ? "Debug" : "");
	strcat(technique, data->mask_effect == MASK_EFFECT_ALPHA
				  ? "Alpha"
				  : "Adjustments");

	if (gs_texrender_begin(data->output_texrender, data->width,
			       data->height)) {
		gs_ortho(0.0f, (float)data->width, 0.0f, (float)data->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, data->width, data->height);
		gs_texrender_end(data->output_texrender);
	}

	gs_blend_state_pop();
}
