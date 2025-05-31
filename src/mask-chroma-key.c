#include "mask-chroma-key.h"
#include "obs-utils.h"

static const float cb_vec[] = { -0.100644f, -0.338572f, 0.439216f, 0.501961f };
static const float cr_vec[] = { 0.439216f, -0.398942f, -0.040274f, 0.501961f };

mask_chroma_key_data_t* mask_chroma_key_create()
{
	mask_chroma_key_data_t* data = bzalloc(sizeof(mask_chroma_key_data_t));

	load_chroma_key_effect_files(data);

	return data;
}

void mask_chroma_key_destroy(mask_chroma_key_data_t* data)
{
	obs_enter_graphics();
	if (data->effect_super_key_mask) {
		gs_effect_destroy(data->effect_super_key_mask);
	}
	if (data->effect_advanced_key_mask) {
		gs_effect_destroy(data->effect_advanced_key_mask);
	}
	obs_leave_graphics();

	bfree(data);
}

void mask_chroma_key_update(mask_chroma_key_data_t* data,
	obs_data_t* settings)
{
	data->keyType = (uint32_t)obs_data_get_int(settings, "key_type");
	data->showMatte = obs_data_get_bool(settings, "show_matte");
	data->k = (float)obs_data_get_double(settings, "mask_super_key_k");
	data->k2 = (float)obs_data_get_double(settings, "mask_super_key_k2");
	data->veil = (float)obs_data_get_double(settings, "mask_super_key_veil");

	data->opacity = (float)obs_data_get_double(settings, "mask_advanced_key_opacity");

	double contrast = obs_data_get_double(settings, "mask_advanced_key_contrast");
	contrast = (contrast < 0.0) ? (1.0 / (-contrast + 1.0)) : (contrast + 1.0);
	data->contrast = (float)contrast;

	data->brightness = (float)obs_data_get_double(settings, "mask_advanced_key_brightness");

	double gamma = obs_data_get_double(settings, "mask_advanced_key_gamma");
	gamma = (gamma < 0.0) ? (-gamma + 1.0) : (1.0 / (gamma + 1.0));
	data->gamma = (float)gamma;

	int64_t similarity = obs_data_get_int(settings, "mask_advanced_key_similarity");
	int64_t smoothness = obs_data_get_int(settings, "mask_advanced_key_smoothness");
	int64_t spill = obs_data_get_int(settings, "mask_advanced_key_spill_reduction");
	uint32_t single_color = (uint32_t)obs_data_get_int(settings, "advanced_key_color_single");
	uint32_t light_color = (uint32_t)obs_data_get_int(settings, "advanced_key_color_double_light");
	uint32_t dark_color = (uint32_t)obs_data_get_int(settings, "advanced_key_color_double_dark");

	uint32_t key_color_type = (uint32_t)obs_data_get_int(settings, "advanced_color_type");
	struct vec4 key_rgb;
	struct vec4 key_single_rgb;
	struct vec4 key_double_light_rgb;
	struct vec4 key_double_dark_rgb;
	struct vec4 cb_v4;
	struct vec4 cr_v4;

	vec4_from_rgba(&key_single_rgb, single_color | 0xFF000000);
	vec4_from_rgba(&key_double_light_rgb, light_color | 0xFF000000);
	vec4_from_rgba(&key_double_dark_rgb, dark_color | 0xFF000000);

	switch (key_color_type) {
	case KEY_COLOR_TYPE_SINGLE:
		key_rgb = key_single_rgb;
		break;
	case KEY_COLOR_TYPE_DOUBLE:
		key_rgb.x = (key_double_light_rgb.x + key_double_dark_rgb.x) / 2.0f;
		key_rgb.y = (key_double_light_rgb.y + key_double_dark_rgb.y) / 2.0f;
		key_rgb.z = (key_double_light_rgb.z + key_double_dark_rgb.z) / 2.0f;
		key_rgb.w = (key_double_light_rgb.w + key_double_dark_rgb.w) / 2.0f;
		break;
	}

	memcpy(&cb_v4, cb_vec, sizeof(cb_v4));
	memcpy(&cr_v4, cr_vec, sizeof(cr_v4));
	data->chroma.x = vec4_dot(&key_rgb, &cb_v4);
	data->chroma.y = vec4_dot(&key_rgb, &cr_v4);

	data->similarity = (float)similarity / 1000.0f;
	data->smoothness = (float)smoothness / 1000.0f;
	data->spill = (float)spill / 1000.0f;
}

void mask_chroma_key_defaults(obs_data_t* settings) {
	obs_data_set_default_double(settings, "mask_super_key_k", 1.0);
	obs_data_set_default_double(settings, "mask_super_key_k2", 1.5);
	obs_data_set_default_double(settings, "mask_super_key_veil", 0.1);
	obs_data_set_default_bool(settings, "show_matte", false);
	obs_data_set_default_int(settings, "advanced_color_type", KEY_COLOR_TYPE_SINGLE);
	obs_data_set_default_int(settings, "mask_advanced_key_similarity", 65);
	obs_data_set_default_int(settings, "mask_advanced_key_smoothness", 35);
	obs_data_set_default_int(settings, "mask_advanced_key_spill_reduction", 40);
	obs_data_set_default_int(settings, "advanced_key_color_single", 0x28795a);
	obs_data_set_default_int(settings, "advanced_key_color_double_light", 0x3b8561);
	obs_data_set_default_int(settings, "advanced_key_color_double_dark", 0x0b301c);
	obs_data_set_default_double(settings, "mask_advanced_key_opacity", 1.0);
}

void mask_chroma_key_properties(obs_properties_t* props)
{
	obs_properties_t* mask_advanced_key_group = obs_properties_create();
	obs_properties_t* mask_super_key_group = obs_properties_create();

	obs_property_t* key_type_list = obs_properties_add_list(
		props, "key_type", obs_module_text("AdvancedMasks.ChromaKey.Type"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(key_type_list,
		obs_module_text(KEY_ADVANCED_LABEL),
		KEY_ADVANCED);
	obs_property_list_add_int(key_type_list,
		obs_module_text(KEY_SUPER_LABEL),
		KEY_SUPER);

	obs_properties_add_bool(props, "show_matte", obs_module_text("AdvancedMasks.ChromaKey.ShowMatte"));

	obs_property_t* advanced_color_list = obs_properties_add_list(
		mask_advanced_key_group, "advanced_color_type", obs_module_text("AdvancedMasks.ChromaKey.Advanced.ColorType"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(advanced_color_list,
		obs_module_text(KEY_COLOR_TYPE_SINGLE_LABEL),
		KEY_COLOR_TYPE_SINGLE);
	obs_property_list_add_int(advanced_color_list,
		obs_module_text(KEY_COLOR_TYPE_DOUBLE_LABEL),
		KEY_COLOR_TYPE_DOUBLE);

	obs_properties_add_color(
		mask_advanced_key_group, "advanced_key_color_single",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.ColorSingle"));

	obs_properties_add_color(
		mask_advanced_key_group, "advanced_key_color_double_light",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.ColorDoubleLight"));

	obs_properties_add_color(
		mask_advanced_key_group, "advanced_key_color_double_dark",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.ColorDoubleDark"));

	obs_properties_add_int_slider(
		mask_advanced_key_group, "mask_advanced_key_similarity",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Similarity"), 1, 1000,
		1);

	obs_properties_add_int_slider(
		mask_advanced_key_group, "mask_advanced_key_smoothness",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Smoothness"), 1, 1000,
		1);

	obs_properties_add_int_slider(
		mask_advanced_key_group, "mask_advanced_key_spill_reduction",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.SpillReduction"), 1, 1000,
		1);

	obs_properties_add_float_slider(
		mask_advanced_key_group, "mask_advanced_key_opacity",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Opacity"), 0.0, 1.0,
		0.0001);

	obs_properties_add_float_slider(
		mask_advanced_key_group, "mask_advanced_key_contrast",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Contrast"), -4.0, 4.0,
		0.01);

	obs_properties_add_float_slider(
		mask_advanced_key_group, "mask_advanced_key_brightness",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Brightness"), -1.0, 1.0,
		0.0001);

	obs_properties_add_float_slider(
		mask_advanced_key_group, "mask_advanced_key_gamma",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced.Gamma"), -1.0, 1.0,
		0.01);

	obs_properties_add_float_slider(
		mask_super_key_group, "mask_super_key_k",
		obs_module_text("AdvancedMasks.ChromaKey.K"), 0.0, 10.0,
		0.01);

	obs_properties_add_float_slider(
		mask_super_key_group, "mask_super_key_k2",
		obs_module_text("AdvancedMasks.ChromaKey.K2"), 0.0, 10.0,
		0.01);

	obs_properties_add_float_slider(
		mask_super_key_group, "mask_super_key_veil",
		obs_module_text("AdvancedMasks.ChromaKey.Veil"), 0.0, 100.0,
		0.01);

	obs_properties_add_group(props, "mask_advanced_key_group",
		obs_module_text("AdvancedMasks.ChromaKey.Advanced"),
		OBS_GROUP_NORMAL, mask_advanced_key_group);

	obs_properties_add_group(
		props, "mask_super_key_group",
		obs_module_text("AdvancedMasks.ChromaKey.Label"),
		OBS_GROUP_NORMAL, mask_super_key_group);

	obs_property_set_modified_callback(key_type_list,
		key_type_modified);

	obs_property_set_modified_callback(advanced_color_list,
		color_type_modified);
}

static void load_chroma_key_effect_files(mask_chroma_key_data_t* data)
{
	load_advanced_key_mask_effect(data);
	load_super_key_mask_effect(data);
}

static void load_super_key_mask_effect(mask_chroma_key_data_t* data)
{
	const char* effect_file_path = "/shaders/super-key.effect";

	data->effect_super_key_mask = load_shader_effect(
		data->effect_super_key_mask, effect_file_path);
	if (data->effect_super_key_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_super_key_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
			effect_index++) {
			gs_eparam_t* param = gs_effect_get_param_by_idx(
				data->effect_super_key_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_super_key_image = param;
			} else if (strcmp(info.name, "k") == 0) {
				data->param_super_key_k = param;
			} else if (strcmp(info.name, "k2") == 0) {
				data->param_super_key_k2 = param;
			} else if (strcmp(info.name, "veil") == 0) {
				data->param_super_key_veil = param;
			}
		}
	}
}

static void load_advanced_key_mask_effect(mask_chroma_key_data_t* data)
{
	const char* effect_file_path = "/shaders/chroma-key.effect";

	data->effect_advanced_key_mask = load_shader_effect(
		data->effect_advanced_key_mask, effect_file_path);
	if (data->effect_advanced_key_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_advanced_key_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
			effect_index++) {
			gs_eparam_t* param = gs_effect_get_param_by_idx(
				data->effect_advanced_key_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_advanced_key_image = param;
			} else if(strcmp(info.name, "opacity") == 0) {
				data->param_advanced_key_opacity = param;
			} else if(strcmp(info.name, "contrast") == 0) {
				data->param_advanced_key_contrast = param;
			} else if(strcmp(info.name, "brightness") == 0) {
				data->param_advanced_key_brightness = param;
			} else if(strcmp(info.name, "gamma") == 0) {
				data->param_advanced_key_gamma = param;
			} else if(strcmp(info.name, "chroma_key") == 0) {
				data->param_advanced_key_chroma_key = param;
			} else if(strcmp(info.name, "pixel_size") == 0) {
				data->param_advanced_key_pixel_size = param;
			} else if(strcmp(info.name, "similarity") == 0) {
				data->param_advanced_key_similarity = param;
			} else if(strcmp(info.name, "smoothness") == 0) {
				data->param_advanced_key_smoothness = param;
			} else if(strcmp(info.name, "spill") == 0) {
				data->param_advanced_key_spill = param;
			}
		}
	}
}

void render_chroma_key_mask(mask_chroma_key_data_t* data, base_filter_data_t* base)
{
	switch (data->keyType) {
	case KEY_ADVANCED:
		render_advanced_key_mask(data, base);
		break;
	case KEY_SUPER:
		render_super_key_mask(data, base);
		break;
	}
	
}

void render_super_key_mask(mask_chroma_key_data_t* data,
	base_filter_data_t* base)
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
		const char* technique = data->showMatte ? "DrawMatte" : "Draw";
		const enum gs_color_format format = gs_get_format_from_space(source_space);
		if (obs_source_process_filter_begin_with_color_space(base->context, format, source_space,
			OBS_ALLOW_DIRECT_RENDERING)) {
			gs_effect_set_float(data->param_super_key_k, data->k);
			gs_effect_set_float(data->param_super_key_k2, data->k2);
			gs_effect_set_float(data->param_super_key_veil, data->veil);

			gs_blend_state_push();
			//gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);
			gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

			obs_source_process_filter_tech_end(base->context, data->effect_super_key_mask, 0, 0, technique);

			gs_blend_state_pop();
		}
	}
}

void render_advanced_key_mask(mask_chroma_key_data_t* data,
	base_filter_data_t* base)
{
	obs_source_t* target = obs_filter_get_target(base->context);
	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	base->width = width;
	base->height = height;
	struct vec2 pixel_size;

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
		const char* technique = data->showMatte ? "DrawMatte" : "Draw";
		const enum gs_color_format format = gs_get_format_from_space(source_space);
		if (obs_source_process_filter_begin_with_color_space(base->context, format, source_space,
			OBS_ALLOW_DIRECT_RENDERING)) {
			vec2_set(&pixel_size, 1.0f / (float)width, 1.0f / (float)height);

			gs_effect_set_float(data->param_advanced_key_opacity, data->opacity);
			gs_effect_set_float(data->param_advanced_key_contrast, data->contrast);
			gs_effect_set_float(data->param_advanced_key_brightness, data->brightness);
			gs_effect_set_float(data->param_advanced_key_gamma, data->gamma);
			gs_effect_set_vec2(data->param_advanced_key_chroma_key, &data->chroma);
			gs_effect_set_vec2(data->param_advanced_key_pixel_size, &pixel_size);
			gs_effect_set_float(data->param_advanced_key_similarity, data->similarity);
			gs_effect_set_float(data->param_advanced_key_smoothness, data->smoothness);
			gs_effect_set_float(data->param_advanced_key_spill, data->spill);

			gs_blend_state_push();
			//gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);
			gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

			obs_source_process_filter_tech_end(base->context, data->effect_advanced_key_mask, 0, 0, technique);

			gs_blend_state_pop();
		}
	}
}

bool key_type_modified(obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings)
{
	UNUSED_PARAMETER(p);
	uint32_t key_type = (uint32_t)obs_data_get_int(settings, "key_type");
	switch (key_type) {
	case KEY_ADVANCED:
		setting_visibility("mask_advanced_key_group", true, props);
		setting_visibility("mask_super_key_group", false, props);
		break;
	case KEY_SUPER:
		setting_visibility("mask_advanced_key_group", false, props);
		setting_visibility("mask_super_key_group", true, props);
		break;
	}
	return true;
}


bool color_type_modified(obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings)
{
	UNUSED_PARAMETER(p);
	uint32_t color_type = (uint32_t)obs_data_get_int(settings, "advanced_color_type");
	switch (color_type) {
	case KEY_COLOR_TYPE_SINGLE:
		setting_visibility("advanced_key_color_single", true, props);
		setting_visibility("advanced_key_color_double_light", false, props);
		setting_visibility("advanced_key_color_double_dark", false, props);
		break;
	case KEY_COLOR_TYPE_DOUBLE:
		setting_visibility("advanced_key_color_single", false, props);
		setting_visibility("advanced_key_color_double_light", true, props);
		setting_visibility("advanced_key_color_double_dark", true, props);
		break;
	}
	return true;
}
