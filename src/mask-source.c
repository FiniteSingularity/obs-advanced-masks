#include "mask-source.h"
#include "obs-utils.h"

mask_source_data_t *mask_source_create()
{
	mask_source_data_t *data = bzalloc(sizeof(mask_source_data_t));

	data->mask_image_path = bzalloc(255 * sizeof(char));
	strcpy(data->mask_image_path, "");
	data->source_mask_texrender =
		create_or_reset_texrender(data->source_mask_texrender);
	data->effect_source_mask = NULL;
	data->mask_source_source = NULL;
	data->param_source_mask_image = NULL;
	data->param_source_mask_source_image = NULL;
	data->param_source_mask_invert = NULL;
	data->param_source_channel_multipliers = NULL;
	data->param_source_multiplier = NULL;
	data->param_source_threshold_value = NULL;
	data->param_source_range_min = NULL;
	data->param_source_range_max = NULL;
	data->param_source_min_brightness = NULL;
	data->param_source_max_brightness = NULL;
	data->param_source_min_contrast = NULL;
	data->param_source_max_contrast = NULL;
	data->param_source_min_saturation = NULL;
	data->param_source_max_saturation = NULL;
	data->param_source_min_hue_shift = NULL;
	data->param_source_max_hue_shift = NULL;

	load_source_effect_files(data);

	return data;
}

void mask_source_destroy(mask_source_data_t *data)
{
	obs_enter_graphics();
	if (data->effect_source_mask) {
		gs_effect_destroy(data->effect_source_mask);
	}
	if (data->source_mask_texrender) {
		gs_texrender_destroy(data->source_mask_texrender);
	}

	if (data->mask_source_source) {
		obs_weak_source_release(data->mask_source_source);
	}

	if (data->mask_image) {
		gs_image_file_free(data->mask_image);
		bfree(data->mask_image);
	}
	obs_leave_graphics();
	bfree(data->mask_image_path);
	bfree(data);
}

void mask_source_update(mask_source_data_t *data, obs_data_t *settings)
{
	const char *mask_source_name =
		obs_data_get_string(settings, "mask_source");
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

	const char *mask_image_file =
		obs_data_get_string(settings, "mask_source_image");
	if (strcmp(mask_image_file, data->mask_image_path) != 0) {
		strcpy(data->mask_image_path, mask_image_file);
		if (data->mask_image == NULL) {
			data->mask_image = bzalloc(sizeof(gs_image_file_t));
		} else {
			obs_enter_graphics();
			gs_image_file_free(data->mask_image);
			obs_leave_graphics();
		}
		if (strlen(mask_image_file)) {
			gs_image_file_init(data->mask_image, mask_image_file);
			obs_enter_graphics();
			gs_image_file_init_texture(data->mask_image);
			obs_leave_graphics();
		}
	}


	data->source_mask_filter_type = (uint32_t)obs_data_get_int(
		settings, "mask_source_mask_properties_list");

	switch (data->source_mask_filter_type) {
	case MASK_SOURCE_FILTER_ALPHA:
		vec4_set(&data->channel_multipliers, 0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case MASK_SOURCE_FILTER_GRAYSCALE:
		vec4_set(&data->channel_multipliers, 0.33333f, 0.33333f,
			 0.33334f, 0.0f);
		break;
	case MASK_SOURCE_FILTER_LUMINOSITY:
		vec4_set(&data->channel_multipliers, 0.299f, 0.587f, 0.114f,
			 0.0f);
		break;
	case MASK_SOURCE_FILTER_SLIDERS:
		vec4_set(&data->channel_multipliers,
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
	data->multiplier = (float)obs_data_get_double(
		settings, "mask_source_filter_multiplier");
	data->source_invert = obs_data_get_bool(settings, "source_invert");
	data->compression_type = (uint32_t)obs_data_get_int(
		settings, "mask_source_compression_list");
	data->threshold_value =
		(float)obs_data_get_double(settings, "source_threshold_value");
	data->range_min =
		(float)obs_data_get_double(settings, "source_range_min");
	data->range_max =
		(float)obs_data_get_double(settings, "source_range_max");
}

void mask_source_defaults(obs_data_t *settings)
{
	UNUSED_PARAMETER(settings);
}

void source_mask_top_properties(obs_properties_t *props)
{
	obs_property_t *mask_source = obs_properties_add_list(
		props, "mask_source",
		obs_module_text("AdvancedMasks.SourceMask.Source"),
		OBS_COMBO_TYPE_EDITABLE, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(
		mask_source, obs_module_text("AdvancedMasks.Common.None"), "");
	obs_enum_sources(add_source_to_list, mask_source);
	obs_enum_scenes(add_source_to_list, mask_source);

	obs_properties_add_path(
		props, "mask_source_image",
		obs_module_text("AdvancedMasks.SourceMask.File"), OBS_PATH_FILE,
		"Textures (*.bmp *.tga *.png *.jpeg *.jpg *.gif);;", NULL);
}

void source_mask_bot_properties(obs_properties_t *props)
{
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

bool setting_mask_source_filter_modified(obs_properties_t *props,
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


static void set_render_params(mask_source_data_t *data,
			      base_filter_data_t *base,
			      color_adjustments_data_t *color_adj,
			      gs_texture_t *texture)
{
	if (data->param_source_mask_image) {
		gs_effect_set_texture(data->param_source_mask_image, texture);
	}

	if (data->param_source_mask_invert) {
		gs_effect_set_bool(data->param_source_mask_invert,
				   data->source_invert);
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
		const float min_brightness = color_adj->adj_brightness
						     ? color_adj->min_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_source_min_brightness,
				    min_brightness);
	}

	if (data->param_source_max_brightness) {
		const float max_brightness = color_adj->adj_brightness
						     ? color_adj->max_brightness
						     : 0.0f;
		gs_effect_set_float(data->param_source_max_brightness,
				    max_brightness);
	}

	if (data->param_source_min_contrast) {
		const float min_contrast = color_adj->adj_contrast
						   ? color_adj->min_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_source_min_contrast,
				    min_contrast);
	}

	if (data->param_source_max_contrast) {
		const float max_contrast = color_adj->adj_contrast
						   ? color_adj->max_contrast
						   : 0.0f;
		gs_effect_set_float(data->param_source_max_contrast,
				    max_contrast);
	}

	if (data->param_source_min_saturation) {
		const float min_saturation = color_adj->adj_saturation
						     ? color_adj->min_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_source_min_saturation,
				    min_saturation);
	}

	if (data->param_source_max_saturation) {
		const float max_saturation = color_adj->adj_saturation
						     ? color_adj->max_saturation
						     : 1.0f;
		gs_effect_set_float(data->param_source_max_saturation,
				    max_saturation);
	}

	if (data->param_source_min_hue_shift) {
		const float min_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->min_hue_shift
						    : 0.0f;
		gs_effect_set_float(data->param_source_min_hue_shift,
				    min_hue_shift);
	}

	if (data->param_source_max_hue_shift) {
		const float max_hue_shift = color_adj->adj_hue_shift
						    ? color_adj->max_hue_shift
						    : 1.0f;
		gs_effect_set_float(data->param_source_max_hue_shift,
				    max_hue_shift);
	}
}

void render_source_mask(mask_source_data_t *data, base_filter_data_t *base,
			color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_source_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	set_render_params(data, base, color_adj, texture);

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

	if (data->param_source_mask_source_image) {
		gs_effect_set_texture(data->param_source_mask_source_image,
				      source_texture);
	}

	set_blending_parameters();

	char technique[32];
	strcpy(technique, base->mask_effect == MASK_EFFECT_ADJUSTMENT
				  ? "Adjustments"
				  : "Alpha");
	char *techniqueType =
		data->compression_type == MASK_SOURCE_COMPRESSION_THRESHOLD
			? "Threshold"
		: data->compression_type == MASK_SOURCE_COMPRESSION_RANGE
			? "Range"
			: "";
	strcat(technique, techniqueType);

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

void render_image_mask(mask_source_data_t *data, base_filter_data_t *base,
		       color_adjustments_data_t *color_adj)
{
	gs_effect_t *effect = data->effect_source_mask;
	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	set_render_params(data, base, color_adj, texture);

	gs_texture_t *source_texture = NULL;
	if (data->mask_image) {
		source_texture = data->mask_image->texture;
	}
	

	if (data->param_source_mask_source_image && source_texture) {
		gs_effect_set_texture(data->param_source_mask_source_image,
				      source_texture);
	} else {
		gs_texrender_t *tmp = base->output_texrender;
		base->output_texrender = base->input_texrender;
		base->input_texrender = tmp;
		return;
	}

	set_blending_parameters();

	char technique[32];
	strcpy(technique, base->mask_effect == MASK_EFFECT_ADJUSTMENT
				  ? "Adjustments"
				  : "Alpha");
	char *techniqueType =
		data->compression_type == MASK_SOURCE_COMPRESSION_THRESHOLD
			? "Threshold"
		: data->compression_type == MASK_SOURCE_COMPRESSION_RANGE
			? "Range"
			: "";
	strcat(technique, techniqueType);

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

static void load_source_effect_files(mask_source_data_t *data)
{
	load_source_mask_effect(data);
}

static void load_source_mask_effect(mask_source_data_t *data)
{
	const char *effect_file_path = "/shaders/source-mask.effect";

	data->effect_source_mask =
		load_shader_effect(data->effect_source_mask, effect_file_path);
	if (data->effect_source_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_source_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_source_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_source_mask_image = param;
			} else if (strcmp(info.name, "source_image") == 0) {
				data->param_source_mask_source_image = param;
			} else if (strcmp(info.name, "invert") == 0) {
				data->param_source_mask_invert = param;
			} else if (strcmp(info.name, "channel_multipliers") ==
				   0) {
				data->param_source_channel_multipliers = param;
			} else if (strcmp(info.name, "multiplier") == 0) {
				data->param_source_multiplier = param;
			} else if (strcmp(info.name, "threshold_value") == 0) {
				data->param_source_threshold_value = param;
			} else if (strcmp(info.name, "range_min") == 0) {
				data->param_source_range_min = param;
			} else if (strcmp(info.name, "range_max") == 0) {
				data->param_source_range_max = param;
			} else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_source_min_brightness = param;
			} else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_source_max_brightness = param;
			} else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_source_min_contrast = param;
			} else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_source_max_contrast = param;
			} else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_source_min_saturation = param;
			} else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_source_max_saturation = param;
			} else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_source_min_hue_shift = param;
			} else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_source_max_hue_shift = param;
			}
		}
	}
}
