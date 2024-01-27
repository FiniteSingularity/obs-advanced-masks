#include "advanced-masks-filter.h"
#include "advanced-masks.h"

struct obs_source_info advanced_masks_filter = {
	.id = "advanced_masks_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_SRGB |
			OBS_SOURCE_CAP_OBSOLETE,
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

struct obs_source_info advanced_masks_filter_v2 = {
	.id = "advanced_masks_filter",
	.version = 2,
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_SRGB,
	.get_name = advanced_masks_name,
	.create = advanced_masks_create,
	.destroy = advanced_masks_destroy,
	.update = advanced_masks_update_v2,
	.video_render = advanced_masks_video_render,
	.video_tick = advanced_masks_video_tick,
	.get_width = advanced_masks_width,
	.get_height = advanced_masks_height,
	.get_properties = advanced_masks_properties,
	.get_defaults = advanced_masks_defaults_v2};

static const char *advanced_masks_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	//return obs_module_text("AdvancedMasks");
	return "Advanced Mask";
}

static void *advanced_masks_create(obs_data_t *settings, obs_source_t *source)
{
	advanced_masks_data_t *filter = bzalloc(sizeof(advanced_masks_data_t));

	filter->context = source;

	filter->source_data = mask_source_create(settings);
	filter->shape_data = mask_shape_create();
	filter->gradient_data = mask_gradient_create();
	filter->bsm_data = mask_bsm_create();

	filter->base = bzalloc(sizeof(base_filter_data_t));
	filter->base->input_texrender =
		create_or_reset_texrender(filter->base->input_texrender);
	filter->base->output_texrender =
		create_or_reset_texrender(filter->base->output_texrender);
	filter->base->param_output_image = NULL;
	filter->base->rendered = false;
	filter->base->rendering = false;

	filter->color_adj_data = bzalloc(sizeof(color_adjustments_data_t));

	load_output_effect(filter);
	obs_source_update(source, settings);

	return filter;
}

static void advanced_masks_destroy(void *data)
{
	// This function should clear up all memory the plugin uses.
	advanced_masks_data_t *filter = data;

	mask_source_destroy(filter->source_data);
	mask_shape_destroy(filter->shape_data);
	mask_gradient_destroy(filter->gradient_data);
	mask_bsm_destroy(filter->bsm_data);

	obs_enter_graphics();
	if (filter->base->input_texrender) {
		gs_texrender_destroy(filter->base->input_texrender);
	}
	if (filter->base->output_texrender) {
		gs_texrender_destroy(filter->base->output_texrender);
	}
	if (filter->base->output_effect) {
		gs_effect_destroy(filter->base->output_effect);
	}
	obs_leave_graphics();

	bfree(filter->base);
	bfree(filter->color_adj_data);
	bfree(filter);
}

static uint32_t advanced_masks_width(void *data)
{
	advanced_masks_data_t *filter = data;
	return filter->base->width;
}

static uint32_t advanced_masks_height(void *data)
{
	advanced_masks_data_t *filter = data;
	return filter->base->height;
}

static void advanced_masks_update(void *data, obs_data_t *settings)
{
	// Called after UI is updated, should assign new UI values to
	// data structure pointers/values/etc..
	advanced_masks_data_t *filter = data;
	if (filter->base->width > 0 &&
	    (float)obs_data_get_double(settings, "shape_center_x") < -1.e8) {
		double width = (double)obs_source_get_width(filter->context);
		double height = (double)obs_source_get_height(filter->context);
		obs_data_set_double(settings, "shape_center_x", width / 2.0);
		obs_data_set_double(settings, "position_x", width / 2.0);
		obs_data_set_double(settings, "shape_center_y", height / 2.0);
		obs_data_set_double(settings, "position_y", height / 2.0);
	}
	if (filter->base->width > 0 &&
	    (float)obs_data_get_double(settings, "mask_gradient_position") <
		    -1.e8) {
		double width = (double)obs_source_get_width(filter->context);
		obs_data_set_double(settings, "mask_gradient_position",
				    width / 2.0);
	}
	filter->base->mask_effect =
		(uint32_t)obs_data_get_int(settings, "mask_effect");
	filter->base->mask_type =
		(uint32_t)obs_data_get_int(settings, "mask_type");

	color_adjustments_update(filter->color_adj_data, settings);

	mask_shape_update(filter->shape_data, filter->base, settings, 1);
	mask_source_update(filter->source_data, settings);
	mask_gradient_update(filter->gradient_data, settings);
	mask_bsm_update(filter->bsm_data, settings);
}

static void advanced_masks_update_v2(void *data, obs_data_t *settings)
{
	// Called after UI is updated, should assign new UI values to
	// data structure pointers/values/etc..
	advanced_masks_data_t *filter = data;
	if (filter->base->width > 0 &&
	    (float)obs_data_get_double(settings, "shape_center_x") < -1.e8) {
		double width = (double)obs_source_get_width(filter->context);
		double height = (double)obs_source_get_height(filter->context);
		obs_data_set_double(settings, "shape_center_x", width / 2.0);
		obs_data_set_double(settings, "position_x", width / 2.0);
		obs_data_set_double(settings, "shape_center_y", height / 2.0);
		obs_data_set_double(settings, "position_y", height / 2.0);
	}
	if (filter->base->width > 0 &&
	    (float)obs_data_get_double(settings, "mask_gradient_position") <
		    -1.e8) {
		double width = (double)obs_source_get_width(filter->context);
		obs_data_set_double(settings, "mask_gradient_position",
				    width / 2.0);
	}
	filter->base->mask_effect =
		(uint32_t)obs_data_get_int(settings, "mask_effect");
	filter->base->mask_type =
		(uint32_t)obs_data_get_int(settings, "mask_type");

	color_adjustments_update(filter->color_adj_data, settings);

	mask_shape_update(filter->shape_data, filter->base, settings, 2);
	mask_source_update(filter->source_data, settings);
	mask_gradient_update(filter->gradient_data, settings);
	mask_bsm_update(filter->bsm_data, settings);
}

static void advanced_masks_video_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);
	advanced_masks_data_t *filter = data;
	if (filter->base->rendered) {
		draw_output(filter);
		return;
	}

	filter->base->rendering = true;

	// 1. Get the input source as a texture renderer
	//    accessed as filter->input_texrender after call
	get_input_source(filter);
	if (!filter->base->input_texture_generated) {
		filter->base->rendering = false;
		obs_source_skip_video_filter(filter->context);
		return;
	}

	// 3. Create Stroke Mask
	// Call a rendering functioner, e.g.:
	render_mask(filter);

	//gs_texrender_t *tmp = filter->base->output_texrender;
	//filter->base->output_texrender = filter->base->input_texrender;
	//filter->base->input_texrender = tmp;

	// 3. Draw result (filter->output_texrender) to source
	draw_output(filter);
	filter->base->rendered = true;
	filter->base->rendering = false;
}

static void render_mask(advanced_masks_data_t *filter)
{
	switch (filter->base->mask_type) {
	case MASK_TYPE_SHAPE:
		render_shape_mask(filter->shape_data, filter->base,
				  filter->color_adj_data);
		break;
	case MASK_TYPE_SOURCE:
		render_source_mask(filter->source_data, filter->base,
				   filter->color_adj_data);
		break;
	case MASK_TYPE_IMAGE:
		render_image_mask(filter->source_data, filter->base,
				  filter->color_adj_data);
		break;
	case MASK_TYPE_GRADIENT:
		render_gradient_mask(filter->gradient_data, filter->base,
				     filter->color_adj_data);
		break;
	case MASK_TYPE_BSM:
		render_bsm_mask(filter->bsm_data, filter->base,
				filter->color_adj_data);
		break;
	}
}

static obs_properties_t *advanced_masks_properties(void *data)
{
	advanced_masks_data_t *filter = data;

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

	obs_property_set_modified_callback2(mask_effect_list,
					    setting_mask_effect_modified, data);

	obs_property_t *mask_type_list = obs_properties_add_list(
		props, "mask_type", obs_module_text("AdvancedMasks.Type"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_SHAPE_LABEL),
				  MASK_TYPE_SHAPE);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_SOURCE_LABEL),
				  MASK_TYPE_SOURCE);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_IMAGE_LABEL),
				  MASK_TYPE_IMAGE);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_GRADIENT_LABEL),
				  MASK_TYPE_GRADIENT);
	obs_property_list_add_int(mask_type_list,
				  obs_module_text(MASK_TYPE_BSM_LABEL),
				  MASK_TYPE_BSM);

	obs_property_set_modified_callback2(mask_type_list,
					    setting_mask_type_modified, data);

	source_mask_top_properties(props, filter->source_data);
	shape_mask_top_properties(props);
	bsm_mask_top_properties(props);

	color_adjustments_properties(props);

	source_mask_bot_properties(props, filter->source_data);
	shape_mask_bot_properties(props, filter->context, filter->shape_data);
	gradient_mask_properties(props);

	obs_properties_add_text(props, "plugin_info", PLUGIN_INFO,
				OBS_TEXT_INFO);

	return props;
}

static bool setting_mask_effect_modified(void *data, obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings)
{
	int mask_effect = (int)obs_data_get_int(settings, "mask_effect");
	setting_visibility("mask_adjustments_group", mask_effect == MASK_EFFECT_ADJUSTMENT, props);

	setting_mask_type_modified(data, props, p, settings);
	return true;
}

static bool setting_mask_type_modified(void *data, obs_properties_t *props,
				       obs_property_t *p, obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int mask_type = (int)obs_data_get_int(settings, "mask_type");
	int effect_type = (int)obs_data_get_int(settings, "mask_effect");
	advanced_masks_data_t *filter = data;

	switch (mask_type) {
	case MASK_TYPE_SHAPE:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", false, props);
		setting_visibility("mask_source_scaling_type", false, props);
		setting_visibility("mask_source_scaling_group", false, props);
		setting_visibility("mask_source_group", false, props);
		setting_visibility("source_mask_compression_group", false,
				   props);
		setting_visibility("shape_type", true, props);
		setting_visibility("shape_relative", true, props);
		setting_visibility("shape_frame_check",
				   effect_type == MASK_EFFECT_ALPHA, props);
		setting_visibility("rectangle_source_group", true, props);
		setting_visibility("rectangle_rounded_corners_group", true,
				   props);
		setting_visibility("shape_feather_group", true, props);
		setting_visibility("scale_position_group",
				   effect_type == MASK_EFFECT_ALPHA, props);
		setting_visibility("mask_gradient_group", false, props);
		set_shape_settings_visibility(filter->shape_data, props, p,
					      settings);
		setting_visibility("bsm_mask_source", false, props);
		setting_visibility("bsm_time", false, props);
		setting_visibility("bsm_freeze", false, props);
		return true;
	case MASK_TYPE_SOURCE:
		setting_visibility("mask_source", true, props);
		setting_visibility("mask_source_image", false, props);
		setting_visibility("mask_source_scaling_type", true, props);
		setting_visibility("mask_source_scaling_group", true, props);

		setting_visibility("mask_source_group", true, props);
		setting_visibility("source_mask_compression_group", true,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("shape_relative", false, props);
		setting_visibility("shape_frame_check", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("shape_feather_group", false, props);
		setting_visibility("scale_position_group", false, props);
		setting_mask_source_filter_modified(props, p, settings);
		setting_visibility("mask_gradient_group", false, props);
		setting_visibility("bsm_mask_source", false, props);
		setting_visibility("bsm_time", false, props);
		setting_visibility("bsm_freeze", false, props);
		return true;
	case MASK_TYPE_IMAGE:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", true, props);
		setting_visibility("mask_source_scaling_type", true, props);
		setting_visibility("mask_source_scaling_group", true, props);

		setting_visibility("mask_source_group", true, props);
		setting_visibility("source_mask_compression_group", true,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("shape_relative", false, props);
		setting_visibility("shape_frame_check", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("shape_feather_group", false, props);
		setting_visibility("scale_position_group", false, props);
		setting_mask_source_filter_modified(props, p, settings);
		setting_visibility("mask_gradient_group", false, props);
		setting_visibility("bsm_mask_source", false, props);
		setting_visibility("bsm_time", false, props);
		setting_visibility("bsm_freeze", false, props);
		return true;
	case MASK_TYPE_GRADIENT:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", false, props);
		setting_visibility("mask_source_scaling_type", false, props);
		setting_visibility("mask_source_scaling_group", false, props);
		setting_visibility("mask_source_group", false, props);
		setting_visibility("source_mask_compression_group", false,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("shape_relative", false, props);
		setting_visibility("shape_frame_check", false, props);
		setting_visibility("shape_feather_group", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("scale_position_group", false, props);
		setting_visibility("mask_gradient_group", true, props);
		setting_visibility("bsm_mask_source", false, props);
		setting_visibility("bsm_time", false, props);
		setting_visibility("bsm_freeze", false, props);
		return true;
	case MASK_TYPE_BSM:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", false, props);
		setting_visibility("mask_source_group", false, props);
		setting_visibility("mask_source_scaling_type", false, props);
		setting_visibility("mask_source_scaling_group", false, props);
		setting_visibility("source_mask_compression_group", false,
				   props);
		setting_visibility("shape_type", false, props);
		setting_visibility("shape_relative", false, props);
		setting_visibility("shape_frame_check", false, props);
		setting_visibility("shape_feather_group", false, props);
		setting_visibility("rectangle_source_group", false, props);
		setting_visibility("rectangle_rounded_corners_group", false,
				   props);
		setting_visibility("scale_position_group", false, props);
		setting_visibility("mask_gradient_group", false, props);

		setting_visibility("bsm_mask_source", true, props);
		setting_visibility("bsm_time", true, props);
		setting_visibility("bsm_freeze", effect_type == MASK_EFFECT_ALPHA, props);
		return true;
	}
	return false;
}

static void advanced_masks_video_tick(void *data, float seconds)
{
	advanced_masks_data_t *filter = data;

	obs_source_t *target = obs_filter_get_target(filter->context);
	if (!target) {
		return;
	}
	filter->base->width = (uint32_t)obs_source_get_base_width(target);
	filter->base->height = (uint32_t)obs_source_get_base_height(target);

	filter->base->rendered = false;
	filter->base->input_texture_generated = false;
	bsm_mask_tick(filter->bsm_data, seconds);
}

static void advanced_masks_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "mask_effect", MASK_EFFECT_ALPHA);
	obs_data_set_default_int(settings, "mask_type", MASK_TYPE_SHAPE);

	color_adjustments_defaults(settings);
	mask_shape_defaults(settings, 1);
	mask_gradient_defaults(settings);
	mask_source_defaults(settings);
	mask_bsm_defaults(settings);
}

static void advanced_masks_defaults_v2(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "mask_effect", MASK_EFFECT_ALPHA);
	obs_data_set_default_int(settings, "mask_type", MASK_TYPE_SHAPE);

	color_adjustments_defaults(settings);
	mask_shape_defaults(settings, 2);
	mask_gradient_defaults(settings);
	mask_source_defaults(settings);
}

static void get_input_source(advanced_masks_data_t *filter)
{
	// Use the OBS default effect file as our effect.
	gs_effect_t *pass_through = obs_get_base_effect(OBS_EFFECT_DEFAULT);

	// Set up our color space info.
	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(filter->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);

	const enum gs_color_format format =
		gs_get_format_from_space(source_space);

	// Set up our input_texrender to catch the output texture.
	filter->base->input_texrender =
		create_or_reset_texrender(filter->base->input_texrender);

	// Start the rendering process with our correct color space params,
	// And set up your texrender to recieve the created texture.
	if (obs_source_process_filter_begin_with_color_space(
		    filter->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING) &&
	    gs_texrender_begin(filter->base->input_texrender,
			       filter->base->width, filter->base->height)) {

		set_blending_parameters();
		gs_ortho(0.0f, (float)filter->base->width, 0.0f,
			 (float)filter->base->height, -100.0f, 100.0f);
		// The incoming source is pre-multiplied alpha, so use the
		// OBS default effect "DrawAlphaDivide" technique to convert
		// the colors back into non-pre-multiplied space.
		obs_source_process_filter_tech_end(
			filter->context, pass_through, filter->base->width,
			filter->base->height, "DrawAlphaDivide");
		gs_texrender_end(filter->base->input_texrender);
		gs_blend_state_pop();
		filter->base->input_texture_generated = true;
	}
}

static void draw_output(advanced_masks_data_t *filter)
{
	const enum gs_color_space preferred_spaces[] = {
		GS_CS_SRGB,
		GS_CS_SRGB_16F,
		GS_CS_709_EXTENDED,
	};

	const enum gs_color_space source_space = obs_source_get_color_space(
		obs_filter_get_target(filter->context),
		OBS_COUNTOF(preferred_spaces), preferred_spaces);

	const enum gs_color_format format =
		gs_get_format_from_space(source_space);

	if (!obs_source_process_filter_begin_with_color_space(
		    filter->context, format, source_space,
		    OBS_NO_DIRECT_RENDERING)) {
		return;
	}

	gs_texture_t *texture =
		gs_texrender_get_texture(filter->base->output_texrender);
	gs_effect_t *pass_through = filter->base->output_effect;

	if (filter->base->param_output_image) {
		gs_effect_set_texture(filter->base->param_output_image,
				      texture);
	}

	obs_source_process_filter_end(filter->context, pass_through,
				      filter->base->width,
				      filter->base->height);
}

static void advanced_masks_render_filter(advanced_masks_data_t *filter)
{
	gs_texrender_t *tmp = filter->base->output_texrender;
	filter->base->output_texrender = filter->base->input_texrender;
	filter->base->input_texrender = tmp;
}

static void load_output_effect(advanced_masks_data_t *filter)
{
	if (filter->base->output_effect != NULL) {
		obs_enter_graphics();
		gs_effect_destroy(filter->base->output_effect);
		filter->base->output_effect = NULL;
		obs_leave_graphics();
	}

	char *shader_text = NULL;
	struct dstr filename = {0};
	dstr_cat(&filename, obs_get_module_data_path(obs_current_module()));
	dstr_cat(&filename, "/shaders/render_output.effect");
	shader_text = load_shader_from_file(filename.array);
	char *errors = NULL;
	dstr_free(&filename);

	obs_enter_graphics();
	filter->base->output_effect =
		gs_effect_create(shader_text, NULL, &errors);
	obs_leave_graphics();

	bfree(shader_text);
	if (filter->base->output_effect == NULL) {
		blog(LOG_WARNING,
		     "[obs-composite-blur] Unable to load output.effect file.  Errors:\n%s",
		     (errors == NULL || strlen(errors) == 0 ? "(None)"
							    : errors));
		bfree(errors);
	} else {
		size_t effect_count =
			gs_effect_get_num_params(filter->base->output_effect);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				filter->base->output_effect, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "output_image") == 0) {
				filter->base->param_output_image = param;
			}
		}
	}
}
