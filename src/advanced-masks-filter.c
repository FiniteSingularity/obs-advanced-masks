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
	advanced_masks_data_t *filter = bzalloc(sizeof(advanced_masks_data_t));

	filter->context = source;

	filter->source_data = mask_source_create();
	filter->shape_data = mask_shape_create();
	filter->gradient_data = mask_gradient_create();

	filter->base = bzalloc(sizeof(base_filter_data_t));
	filter->base->input_texrender =
		create_or_reset_texrender(filter->base->input_texrender);
	filter->base->output_texrender =
		create_or_reset_texrender(filter->base->output_texrender);
	filter->base->rendered = false;
	filter->base->rendering = false;

	filter->color_adj_data = bzalloc(sizeof(color_adjustments_data_t));

	obs_source_update(source, settings);
	return filter;
}

static void advanced_masks_destroy(void *data)
{
	// This function should clear up all memory the plugin uses.
	advanced_masks_data_t *filter = data;

	obs_enter_graphics();
	if (filter->base->input_texrender) {
		gs_texrender_destroy(filter->base->input_texrender);
	}
	if (filter->base->output_texrender) {
		gs_texrender_destroy(filter->base->output_texrender);
	}
	obs_leave_graphics();

	mask_source_destroy(filter->source_data);
	mask_shape_destroy(filter->shape_data);
	mask_gradient_destroy(filter->gradient_data);

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

	filter->base->mask_effect =
		(uint32_t)obs_data_get_int(settings, "mask_effect");
	filter->base->mask_type =
		(uint32_t)obs_data_get_int(settings, "mask_type");

	color_adjustments_update(filter->color_adj_data, settings);

	mask_shape_update(filter->shape_data, filter->base, settings);
	mask_source_update(filter->source_data, settings);
	mask_gradient_update(filter->gradient_data, settings);
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
	obs_property_set_modified_callback2(mask_type_list,
					   setting_mask_type_modified, data);

	source_mask_top_properties(props);
	shape_mask_top_properties(props);

	color_adjustments_properties(props);

	source_mask_bot_properties(props);
	shape_mask_bot_properties(props, filter->context, filter->shape_data);
	gradient_mask_properties(props);

	obs_properties_add_text(props, "plugin_info", PLUGIN_INFO,
				OBS_TEXT_INFO);

	return props;
}

static bool setting_mask_effect_modified(void *data,
					 obs_properties_t *props,
					 obs_property_t *p,
					 obs_data_t *settings)
{
	int mask_effect = (int)obs_data_get_int(settings, "mask_effect");
	switch (mask_effect) {
	case MASK_EFFECT_ADJUSTMENT:
		setting_visibility("mask_adjustments_group", true, props);
		break;
	case MASK_EFFECT_ALPHA:
		setting_visibility("mask_adjustments_group", false, props);
		break;
	}
	setting_mask_type_modified(data, props, p, settings);
	return true;
}

static bool setting_mask_type_modified(void *data, obs_properties_t *props,
				       obs_property_t *p, obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	int mask_type = (int)obs_data_get_int(settings, "mask_type");
	int effect_type = (int)obs_data_get_int(settings, "mask_effect");
	switch (mask_type) {
	case MASK_TYPE_SHAPE:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", false, props);
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
		set_shape_settings_visibility(data, props, p, settings);
		return true;
	case MASK_TYPE_SOURCE:
		setting_visibility("mask_source", true, props);
		setting_visibility("mask_source_image", false, props);
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
		return true;
	case MASK_TYPE_IMAGE:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", true, props);
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
		return true;
	case MASK_TYPE_GRADIENT:
		setting_visibility("mask_source", false, props);
		setting_visibility("mask_source_image", false, props);
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
		return true;
	}
	return false;
}

static void advanced_masks_video_tick(void *data, float seconds)
{
	UNUSED_PARAMETER(seconds);
	advanced_masks_data_t *filter = data;

	obs_source_t *target = obs_filter_get_target(filter->context);
	if (!target) {
		return;
	}
	filter->base->width = (uint32_t)obs_source_get_base_width(target);
	filter->base->height = (uint32_t)obs_source_get_base_height(target);

	filter->base->rendered = false;
	filter->base->input_texture_generated = false;
}

static void advanced_masks_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "mask_effect", MASK_EFFECT_ALPHA);
	obs_data_set_default_int(settings, "mask_type", MASK_TYPE_SHAPE);

	color_adjustments_defaults(settings);
	mask_shape_defaults(settings);
	mask_gradient_defaults(settings);
	mask_source_defaults(settings);
}

static void get_input_source(advanced_masks_data_t *filter)
{
	gs_effect_t *pass_through = obs_get_base_effect(OBS_EFFECT_DEFAULT);

	filter->base->input_texrender =
		create_or_reset_texrender(filter->base->input_texrender);
	if (obs_source_process_filter_begin(filter->context, GS_RGBA,
					    OBS_ALLOW_DIRECT_RENDERING) &&
	    gs_texrender_begin(filter->base->input_texrender,
			       filter->base->width, filter->base->height)) {

		set_blending_parameters();
		gs_ortho(0.0f, (float)filter->base->width, 0.0f,
			 (float)filter->base->height, -100.0f, 100.0f);
		obs_source_process_filter_end(filter->context, pass_through,
					      filter->base->width,
					      filter->base->height);
		gs_texrender_end(filter->base->input_texrender);
		gs_blend_state_pop();
		filter->base->input_texture_generated = true;
	}
}

static void draw_output(advanced_masks_data_t *filter)
{
	gs_texture_t *texture =
		gs_texrender_get_texture(filter->base->output_texrender);
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
	gs_texrender_t *tmp = filter->base->output_texrender;
	filter->base->output_texrender = filter->base->input_texrender;
	filter->base->input_texrender = tmp;
}
