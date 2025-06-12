#include "mask-gradient.h"
#include "obs-utils.h"

mask_gradient_data_t *mask_gradient_create()
{
	mask_gradient_data_t *data = bzalloc(sizeof(mask_gradient_data_t));

	data->param_gradient_image = NULL;
	data->param_gradient_width = NULL;
	data->param_gradient_position = NULL;
	data->param_gradient_rotation = NULL;
	data->param_gradient_uv_size = NULL;
	data->param_gradient_invert = NULL;
	data->param_gradient_min_brightness = NULL;
	data->param_gradient_max_brightness = NULL;
	data->param_gradient_min_contrast = NULL;
	data->param_gradient_max_contrast = NULL;
	data->param_gradient_min_saturation = NULL;
	data->param_gradient_max_saturation = NULL;
	data->param_gradient_min_hue_shift = NULL;
	data->param_gradient_max_hue_shift = NULL;

	load_gradient_effect_files(data);

	return data;
}

void mask_gradient_destroy(mask_gradient_data_t *data)
{
	obs_enter_graphics();
	if (data->effect_gradient_mask) {
		gs_effect_destroy(data->effect_gradient_mask);
	}
	obs_leave_graphics();

	bfree(data);
}

void mask_gradient_update(mask_gradient_data_t *data,
		       obs_data_t *settings)
{

	data->gradient_position =
		(float)obs_data_get_double(settings, "mask_gradient_position");
	data->gradient_width =
		(float)obs_data_get_double(settings, "mask_gradient_width");
	data->gradient_rotation =
		(float)obs_data_get_double(settings, "mask_gradient_rotation");
	data->gradient_debug =
		obs_data_get_bool(settings, "mask_gradient_debug");
	data->gradient_invert =
		obs_data_get_bool(settings, "gradient_invert");
}

void mask_gradient_defaults(obs_data_t *settings) {
	obs_data_set_default_bool(settings, "gradient_invert", false);
	obs_data_set_default_double(settings, "mask_gradient_position", -1.e9);
	obs_data_set_default_double(settings, "mask_gradient_width", 500.0);
	obs_data_set_default_double(settings, "mask_gradient_rotation", 0.0);
	obs_data_set_default_bool(settings, "mask_gradient_debug", false);
}

void gradient_mask_properties(obs_properties_t *props)
{
	obs_properties_t *mask_gradient_group = obs_properties_create();
	obs_property_t *p;

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
}


static void load_gradient_effect_files(mask_gradient_data_t *data)
{
	load_gradient_mask_effect(data);
}

static void load_gradient_mask_effect(mask_gradient_data_t *data)
{
	const char *effect_file_path = "/shaders/gradient-mask.effect";

	data->effect_gradient_mask = load_shader_effect(
		data->effect_gradient_mask, effect_file_path);
	if (data->effect_gradient_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_gradient_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_gradient_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_gradient_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_gradient_uv_size = param;
			} else if (strcmp(info.name, "width") == 0) {
				data->param_gradient_width = param;
			} else if (strcmp(info.name, "position") == 0) {
				data->param_gradient_position = param;
			} else if (strcmp(info.name, "rotation") == 0) {
				data->param_gradient_rotation = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_gradient_invert = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_gradient_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_gradient_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_gradient_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_gradient_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_gradient_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_gradient_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_gradient_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_gradient_max_hue_shift = param;
			}
		}
	}
}

void render_gradient_mask(mask_gradient_data_t *data,
				 base_filter_data_t *base,
				 color_adjustments_data_t *color_adj)
{
	obs_source_t* target = obs_filter_get_target(base->context);
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
		obs_filter_get_target(base->context), OBS_COUNTOF(preferred_spaces), preferred_spaces);
	if (source_space == GS_CS_709_EXTENDED) {
		obs_source_skip_video_filter(base->context);
	}
	else {
		char technique[32];
		strcpy(technique, data->gradient_debug ? "Debug" : "");
		strcat(technique, base->mask_effect == MASK_EFFECT_ALPHA
			? "Alpha"
			: "Adjustments");

		const enum gs_color_format format = gs_get_format_from_space(source_space);
		if (obs_source_process_filter_begin_with_color_space(base->context, format, source_space,
			OBS_NO_DIRECT_RENDERING)) {

			gs_effect_set_float(data->param_gradient_width,
				data->gradient_width);
			gs_effect_set_bool(data->param_gradient_invert,
				data->gradient_invert);
			const float position = data->gradient_position - (float)base->width / 2.0f;
			gs_effect_set_float(data->param_gradient_position, position);
			const float rotation = data->gradient_rotation * M_PI / 180.0f;
			gs_effect_set_float(data->param_gradient_rotation, rotation);

			const float min_brightness =
				color_adj->adj_brightness ? color_adj->min_brightness : 0.0f;
			gs_effect_set_float(data->param_gradient_min_brightness,
				min_brightness);
			const float max_brightness =
				color_adj->adj_brightness ? color_adj->max_brightness : 0.0f;
			gs_effect_set_float(data->param_gradient_max_brightness,
				max_brightness);

			const float min_contrast =
				color_adj->adj_contrast ? color_adj->min_contrast : 0.0f;
			gs_effect_set_float(data->param_gradient_min_contrast,
				min_contrast);
			const float max_contrast =
				color_adj->adj_contrast ? color_adj->max_contrast : 0.0f;
			gs_effect_set_float(data->param_gradient_max_contrast,
				max_contrast);

			const float min_saturation =
				color_adj->adj_saturation ? color_adj->min_saturation : 1.0f;
			gs_effect_set_float(data->param_gradient_min_saturation,
				min_saturation);
			const float max_saturation =
				color_adj->adj_saturation ? color_adj->max_saturation : 1.0f;
			gs_effect_set_float(data->param_gradient_max_saturation,
				max_saturation);

			const float min_hue_shift =
				color_adj->adj_hue_shift ? color_adj->min_hue_shift : 0.0f;
			gs_effect_set_float(data->param_gradient_min_hue_shift,
				min_hue_shift);
			const float max_hue_shift =
				color_adj->adj_hue_shift ? color_adj->max_hue_shift : 1.0f;
			gs_effect_set_float(data->param_gradient_max_hue_shift,
				max_hue_shift);

			struct vec2 uv_size;
			uv_size.x = (float)base->width;
			uv_size.y = (float)base->height;
			gs_effect_set_vec2(data->param_gradient_uv_size, &uv_size);

			gs_blend_state_push();
			gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);
			obs_source_process_filter_tech_end(base->context, data->effect_gradient_mask, 0, 0, technique);
			gs_blend_state_pop();
		}
	}
}
