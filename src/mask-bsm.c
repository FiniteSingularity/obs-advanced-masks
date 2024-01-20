#include "mask-bsm.h"
#include "obs-utils.h"

mask_bsm_data_t *mask_bsm_create()
{
	mask_bsm_data_t *data = bzalloc(sizeof(mask_bsm_data_t));

	data->bsm_mask_texrender =
		create_or_reset_texrender(data->bsm_mask_texrender);
	data->bsm_buffer_texrender =
		create_or_reset_texrender(data->bsm_buffer_texrender);

	data->effect_bsm_mask = NULL;
	data->mask_source_source = NULL;
	data->alpha_reduction = 0.0f;

	load_bsm_effect_files(data);

	return data;
}

void mask_bsm_destroy(mask_bsm_data_t *data)
{
	obs_enter_graphics();
	if (data->effect_bsm_mask) {
		gs_effect_destroy(data->effect_bsm_mask);
	}

	if (data->bsm_mask_texrender) {
		gs_texrender_destroy(data->bsm_mask_texrender);
	}
	if (data->bsm_buffer_texrender) {
		gs_texrender_destroy(data->bsm_buffer_texrender);
	}

	if (data->mask_source_source) {
		obs_weak_source_release(data->mask_source_source);
	}

	obs_leave_graphics();
	bfree(data);
}

void mask_bsm_update(mask_bsm_data_t *data, obs_data_t *settings)
{
	const char *mask_source_name =
		obs_data_get_string(settings, "bsm_mask_source");
	obs_source_t *mask_source =
		(mask_source_name && strlen(mask_source_name))
			? obs_get_source_by_name(mask_source_name)
			: NULL;

	if (mask_source) {
		obs_weak_source_release(data->mask_source_source);
		data->mask_source_source =
			obs_source_get_weak_source(mask_source);
		obs_source_release(mask_source);
	} else {
		data->mask_source_source = NULL;
	}

	data->fade_time = (float)obs_data_get_double(settings, "bsm_time");
}

void mask_bsm_defaults(obs_data_t *settings)
{
	UNUSED_PARAMETER(settings);
}

void bsm_mask_top_properties(obs_properties_t *props)
{
	obs_property_t *mask_source = obs_properties_add_list(
		props, "bsm_mask_source",
		obs_module_text("AdvancedMasks.SourceMask.Source"),
		OBS_COMBO_TYPE_EDITABLE, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(
		mask_source, obs_module_text("AdvancedMasks.Common.None"), "");
	obs_enum_sources(add_source_to_list, mask_source);
	obs_enum_scenes(add_source_to_list, mask_source);

	obs_property_t *p = obs_properties_add_float(
		props, "bsm_time",
		obs_module_text("AdvancedMasks.BSMMask.Time"), 0.0,
		100000.0, 1.0);

	obs_property_float_set_suffix(p, "ms");
}

void bsm_mask_bot_properties(obs_properties_t *props)
{ }

void bsm_mask_tick(mask_bsm_data_t* data, float seconds) {
	data->seconds = seconds;
}

void render_bsm_mask(mask_bsm_data_t *data, base_filter_data_t *base,
			color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_bsm_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	gs_texrender_t *tmp = data->bsm_buffer_texrender;
	data->bsm_buffer_texrender = base->output_texrender;
	base->output_texrender = tmp;

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	if (data->param_bsm_min_brightness) {
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_bsm_min_brightness,
				    min_brightness);
	}

	if (data->param_bsm_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_bsm_max_brightness,
				    max_brightness);
	}

	if (data->param_bsm_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_bsm_min_contrast, min_contrast);
	}

	if (data->param_bsm_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_bsm_max_contrast, max_contrast);
	}

	if (data->param_bsm_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_bsm_min_saturation,
				    min_saturation);
	}

	if (data->param_bsm_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_bsm_max_saturation,
				    max_saturation);
	}

	if (data->param_bsm_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_bsm_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_bsm_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_bsm_max_hue_shift,
				    max_hue_shift);
	}

	if (data->param_bsm_alpha_reduction) {
		float alpha_reduction = 0.0f;
		if(data->fade_time > 0.0001f) {
			data->alpha_reduction +=
				data->seconds / (data->fade_time / 1000.0f);
			if (data->alpha_reduction > 0.004) {
				alpha_reduction = data->alpha_reduction;
				data->alpha_reduction = 0.0f;
			}
		}
		gs_effect_set_float(data->param_bsm_alpha_reduction, alpha_reduction);
	}

	gs_texrender_t *mask_source_render = NULL;
	obs_source_t *source =
		data->mask_source_source
			? obs_weak_source_get_source(data->mask_source_source)
			: NULL;

	if (!source) {
		gs_texrender_t *tmp = base->output_texrender;
		base->output_texrender = base->input_texrender;
		base->input_texrender = tmp;
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

	gs_texture_t *buffer_texture =
		gs_texrender_get_texture(data->bsm_buffer_texrender);

	if (data->param_bsm_image) {
		gs_effect_set_texture(data->param_bsm_image,
				      texture);
	}

	if (data->param_bsm_current_input_mask) {
		gs_effect_set_texture(data->param_bsm_current_input_mask,
				      source_texture);
	}

	if (data->param_bsm_buffer) {
		gs_effect_set_texture(data->param_bsm_buffer,
				      buffer_texture);
	}

	set_blending_parameters();

	char technique[32];
	strcpy(technique, base->mask_effect == MASK_EFFECT_ADJUSTMENT
				  ? "Adjustments"
				  : "Alpha");

	if (gs_texrender_begin(base->output_texrender, base->width,
			       base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			 -100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(texture, 0, base->width, base->height);
		gs_texrender_end(base->output_texrender);
	}
	gs_texrender_destroy(mask_source_render);
	gs_blend_state_pop();
}


static void load_bsm_effect_files(mask_bsm_data_t *data)
{
	load_bsm_mask_effect(data);
}

static void load_bsm_mask_effect(mask_bsm_data_t *data)
{
	const char *effect_file_path = "/shaders/boom-so-much-mask.effect";

	data->effect_bsm_mask =
		load_shader_effect(data->effect_bsm_mask, effect_file_path);
	if (data->effect_bsm_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_bsm_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_bsm_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_bsm_image = param;
			} else if (strcmp(info.name, "buffer") == 0) {
				data->param_bsm_buffer = param;
			} else if (strcmp(info.name, "current_input_mask") == 0) {
				data->param_bsm_current_input_mask = param;
			} else if (strcmp(info.name, "alpha_reduction") == 0) {
				data->param_bsm_alpha_reduction = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_bsm_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_bsm_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_bsm_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_bsm_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_bsm_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_bsm_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_bsm_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_bsm_max_hue_shift = param;
			}
		}
	}
}
