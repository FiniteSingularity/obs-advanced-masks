#include "mask-feather.h"
#include "obs-utils.h"
#include "advanced-masks-filter.h"

mask_feather_data_t* mask_feather_create()
{
	mask_feather_data_t *data = bzalloc(sizeof(mask_feather_data_t));
	load_feather_effect_files(data);

	return data;
}

void mask_feather_destroy(mask_feather_data_t *data)
{
	obs_enter_graphics();
	if (data->effect_feather_mask) {
		gs_effect_destroy(data->effect_feather_mask);
	}
	if (data->effect_jump_flood_sdf) {
		gs_effect_destroy(data->effect_jump_flood_sdf);
	}

	if (data->buffer_a) {
		gs_texrender_destroy(data->buffer_a);
	}
	if (data->buffer_b) {
		gs_texrender_destroy(data->buffer_b);
	}

	obs_leave_graphics();

	bfree(data);
}

void mask_feather_update(mask_feather_data_t *data,
		       obs_data_t *settings)
{
	 data->featherSize =
	 	(float)obs_data_get_double(settings, "mask_feather_size");
	// data->gradient_width =
	// 	(float)obs_data_get_double(settings, "mask_gradient_width");
}

void mask_feather_defaults(obs_data_t *settings) {
	UNUSED_PARAMETER(settings);
	//obs_data_set_default_double(settings, "mask_gradient_position", -1.e9);
	//obs_data_set_default_double(settings, "mask_gradient_width", 500.0);
}

void feather_mask_properties(obs_properties_t *props)
{
	obs_properties_t *mask_feather_group = obs_properties_create();
	obs_property_t *p;

	p = obs_properties_add_float_slider(
		mask_feather_group, "mask_feather_size",
		obs_module_text("AdvancedMasks.FeatherMask.Size"), 0, 50.0,
		1.0);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "mask_feather_group",
		obs_module_text("AdvancedMasks.FeatherMask.Label"),
		OBS_GROUP_NORMAL, mask_feather_group);
}


static void load_feather_effect_files(mask_feather_data_t *data)
{
	load_feather_mask_effect(data);
	load_jump_flood_sdf_effect(data);
}

static void load_feather_mask_effect(mask_feather_data_t *data)
{
	const char *effect_file_path = "/shaders/feather-mask.effect";

	data->effect_feather_mask = load_shader_effect(
		data->effect_feather_mask, effect_file_path);
	if (data->effect_feather_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_feather_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
		     effect_index++) {
			gs_eparam_t *param = gs_effect_get_param_by_idx(
				data->effect_feather_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_feather_image = param;
			} else if (strcmp(info.name, "distance_field") == 0) {
				data->param_feather_distance_field = param;
			} else if (strcmp(info.name, "feather_size") == 0) {
				data->param_feather_size = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_feather_uv_size = param;
			}
		}
	}
}

static void load_jump_flood_sdf_effect(mask_feather_data_t* data)
{
	const char* effect_file_path = "/shaders/jump-flood.effect";

	data->effect_jump_flood_sdf = load_shader_effect(
		data->effect_jump_flood_sdf, effect_file_path);
	if (data->effect_jump_flood_sdf) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_jump_flood_sdf);
		for (size_t effect_index = 0; effect_index < effect_count;
			effect_index++) {
			gs_eparam_t* param = gs_effect_get_param_by_idx(
				data->effect_jump_flood_sdf, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "uv_size") == 0) {
				data->param_jf_uv_size = param;
			}
			else if (strcmp(info.name, "offset") == 0) {
				data->param_jf_offset = param;
			}
		}
	}
}

void render_feather_mask(mask_feather_data_t *data,
				 base_filter_data_t *base)
{
	get_input_source(base);
	gs_effect_t *effect = data->effect_feather_mask;

	gs_texture_t *texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture) {
		return;
	}

	render_jf_inner_threshold(data, base);
	float innerExtent = fmaxf(data->featherSize, 2.0f); // data->feather_size;
	render_jf_passes_inner(data, base, innerExtent);

	gs_texture_t* distance_field = gs_texrender_get_texture(data->buffer_a);

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
	} else {
		const char* technique = "Draw";
		const enum gs_color_format format = gs_get_format_from_space(source_space);
		if (obs_source_process_filter_begin_with_color_space(base->context, format, source_space,
			OBS_NO_DIRECT_RENDERING)) {
			gs_effect_set_float(data->param_feather_size, data->featherSize);
			gs_effect_set_texture(data->param_feather_distance_field, distance_field);
			if (data->param_feather_uv_size) {
				struct vec2 uv_size;
				uv_size.x = (float)base->width;
				uv_size.y = (float)base->height;
				gs_effect_set_vec2(data->param_feather_uv_size, &uv_size);
			}
			gs_blend_state_push();
			gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

			obs_source_process_filter_tech_end(base->context, effect, 0, 0, technique);

			gs_blend_state_pop();
		}
	}
}

static void render_jf_inner_threshold(mask_feather_data_t* data, base_filter_data_t* base)
{
	gs_effect_t* effect = data->effect_jump_flood_sdf;
	gs_texture_t* input_texture = gs_texrender_get_texture(base->input_texrender);

	if (!effect || !input_texture) {
		return;
	}

	data->buffer_a = create_or_reset_texrender_high(data->buffer_a);

	gs_eparam_t* image = gs_effect_get_param_by_name(effect, "image");
	gs_effect_set_texture(image, input_texture);

	if (data->param_jf_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_jf_uv_size, &uv_size);
	}

	set_blending_parameters();

	const char* technique = "DrawThresholdInner";

	if (gs_texrender_begin(data->buffer_a, base->width, base->height)) {
		gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
			-100.0f, 100.0f);
		while (gs_effect_loop(effect, technique))
			gs_draw_sprite(input_texture, 0, base->width,
				base->height);
		gs_texrender_end(data->buffer_a);
	}
	gs_blend_state_pop();
}


static void render_jf_passes_inner(mask_feather_data_t* data, base_filter_data_t* base, float maxExtent)
{
	gs_effect_t* effect = data->effect_jump_flood_sdf;

	//gs_texture_t* input_texture = gs_texrender_get_texture(data->stroke_mask);

	if (!effect) {
		return;
	}

	//gs_texrender_t* tmp = base->input_texrender;
	//base->input_texrender = data->buffer_a;
	//data->buffer_a = tmp;

	int iExtent = (int)maxExtent + 1;

	int maxPass = 1;
	while (maxPass < iExtent) {
		maxPass *= 2;
	}
	//maxPass = maxPass *= 2;
	int pass = maxPass;

	while (pass > 0) {
		gs_texture_t* input_texture = gs_texrender_get_texture(data->buffer_a);
		if (!input_texture) {
			return;
		}

		data->buffer_b = create_or_reset_texrender_high(data->buffer_b);

		gs_eparam_t* image = gs_effect_get_param_by_name(effect, "image");
		gs_effect_set_texture(image, input_texture);

		if (data->param_jf_offset) {
			//float fPass = pass != maxPass ? (float)pass : 1.0f;
			float fPass = (float)pass;
			gs_effect_set_float(data->param_jf_offset, fPass);
		}

		if (data->param_jf_uv_size) {
			struct vec2 uv_size;
			uv_size.x = (float)base->width;
			uv_size.y = (float)base->height;
			gs_effect_set_vec2(data->param_jf_uv_size, &uv_size);
		}

		set_blending_parameters();

		if (gs_texrender_begin(data->buffer_b, base->width, base->height)) {
			gs_ortho(0.0f, (float)base->width, 0.0f, (float)base->height,
				-100.0f, 100.0f);
			while (gs_effect_loop(effect, "DrawJumpFloodStep"))
				gs_draw_sprite(input_texture, 0, base->width,
					base->height);
			gs_texrender_end(data->buffer_b);
		}
		gs_blend_state_pop();

		gs_texrender_t* tmp = data->buffer_a;
		data->buffer_a = data->buffer_b;
		data->buffer_b = tmp;
		pass /= 2;
	}
}
