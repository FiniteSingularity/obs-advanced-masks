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
	gs_effect_t *effect = data->effect_gradient_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	if (data->param_gradient_image) {
		gs_effect_set_texture(data->param_gradient_image, texture);
	}

	if (data->param_gradient_width) {
		gs_effect_set_float(data->param_gradient_width,
				    data->gradient_width);
	}

	if (data->param_gradient_invert) {
		gs_effect_set_bool(data->param_gradient_invert,
				   data->gradient_invert);
	}

	if (data->param_gradient_position) {
		const float position =
			data->gradient_position - (float)base->width / 2.0f;
		gs_effect_set_float(data->param_gradient_position, position);
	}

	if (data->param_gradient_rotation) {
		const float rotation = data->gradient_rotation * M_PI / 180.0f;
		gs_effect_set_float(data->param_gradient_rotation, rotation);
	}

	if (data->param_gradient_min_brightness) {
		const float min_brightness =
			color_adj->adj_brightness ? color_adj->min_brightness : 0.0f;
		gs_effect_set_float(data->param_gradient_min_brightness,
				    min_brightness);
	}

	if (data->param_gradient_max_brightness) {
		const float max_brightness =
			color_adj->adj_brightness ? color_adj->max_brightness : 0.0f;
		gs_effect_set_float(data->param_gradient_max_brightness,
				    max_brightness);
	}

	if (data->param_gradient_min_contrast) {
		const float min_contrast =
			color_adj->adj_contrast ? color_adj->min_contrast : 0.0f;
		gs_effect_set_float(data->param_gradient_min_contrast,
				    min_contrast);
	}

	if (data->param_gradient_max_contrast) {
		const float max_contrast =
			color_adj->adj_contrast ? color_adj->max_contrast : 0.0f;
		gs_effect_set_float(data->param_gradient_max_contrast,
				    max_contrast);
	}

	if (data->param_gradient_min_saturation) {
		const float min_saturation =
			color_adj->adj_saturation ? color_adj->min_saturation : 1.0f;
		gs_effect_set_float(data->param_gradient_min_saturation,
				    min_saturation);
	}

	if (data->param_gradient_max_saturation) {
		const float max_saturation =
			color_adj->adj_saturation ? color_adj->max_saturation : 1.0f;
		gs_effect_set_float(data->param_gradient_max_saturation,
				    max_saturation);
	}

	if (data->param_gradient_min_hue_shift) {
		const float min_hue_shift =
			color_adj->adj_hue_shift ? color_adj->min_hue_shift : 0.0f;
		gs_effect_set_float(data->param_gradient_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_gradient_max_hue_shift) {
		const float max_hue_shift =
			color_adj->adj_hue_shift ? color_adj->max_hue_shift : 1.0f;
		gs_effect_set_float(data->param_gradient_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_gradient_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_gradient_uv_size, &uv_size);
	}

	set_render_parameters();
	set_blending_parameters();
	char technique[32];
	strcpy(technique, data->gradient_debug ? "Debug" : "");
	strcat(technique, base->mask_effect == MASK_EFFECT_ALPHA
				  ? "Alpha"
				  : "Adjustments");

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
