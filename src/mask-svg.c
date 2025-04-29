#include "mask-svg.h"
#include "obs-utils.h"
#include "advanced-masks-filter.h"
#include <math.h>

gs_texture_t* gs_texture_from_svg(const char* path, int width, int height, int scale_by);
void update_svg_textures(
	const char* path,
	int width,
	int height,
	int scale_by,
	int update_type,
	gs_texture_t** small,
	gs_texture_t** current,
	gs_texture_t** large
);

mask_svg_data_t* mask_svg_create(obs_data_t* settings)
{
	mask_svg_data_t* data = bzalloc(sizeof(mask_svg_data_t));
	dstr_init_copy(&data->svg_image_path, "!");
	load_svg_effect_files(data);

	da_init(data->textures);

	mask_svg_update(data, settings);
	return data;
}

void mask_svg_destroy(mask_svg_data_t* data)
{
	obs_enter_graphics();
	if (data->effect_svg_mask) {
		gs_effect_destroy(data->effect_svg_mask);
	}
	if (data->imageTexture) {
		gs_texture_destroy(data->imageTexture);
	}
	if (data->next_size_smaller_tex) {
		gs_texture_destroy(data->next_size_smaller_tex);
	}
	if (data->current_tex) {
		gs_texture_destroy(data->current_tex);
	}
	if (data->next_size_greater_tex) {
		gs_texture_destroy(data->next_size_greater_tex);
	}

	for (size_t i = 0; i < data->textures.num; i++)
	{
		gs_texture_destroy(data->textures.array[i]);
	}
	da_free(data->textures);

	dstr_free(&data->svg_image_path);
	obs_leave_graphics();

	bfree(data);
}

void mask_svg_update(mask_svg_data_t* data,
	obs_data_t* settings)
{
	if (obs_data_get_int(settings, "mask_type") != MASK_TYPE_SVG) {
		return;
	}

	const char* svg_image_path = obs_data_get_string(settings, "mask_svg_image");
	uint32_t width = (uint32_t)obs_data_get_int(settings, "mask_svg_width");
	uint32_t height = (uint32_t)obs_data_get_int(settings, "mask_svg_height");
	uint32_t scaleBy = (uint32_t)obs_data_get_int(settings, "mask_svg_scale_by");

	bool image_path_changed = false;
	bool scaleByChanged = data->scale_by != scaleBy;

	bool cur_img_path_zero = data->svg_image_path.len == 0;
	if (!cur_img_path_zero) {
		image_path_changed = dstr_cmp(&data->svg_image_path, svg_image_path) != 0;
	} else {
		image_path_changed = strlen(svg_image_path) > 0;
	}

	bool regen_bitmap = image_path_changed;

	dstr_copy(&data->svg_image_path, svg_image_path);
	data->target_width = width;
	data->target_height = height;
	data->scale_by = scaleBy;

	data->offset_x = (int)obs_data_get_int(settings, "mask_pos_x");
	data->offset_y = (int)obs_data_get_int(settings, "mask_pos_y");


	if ((regen_bitmap || scaleByChanged) && !dstr_is_empty(&data->svg_image_path)) {
		render_svg_to_texture(data);
	}
	uint32_t npt;
	switch (data->scale_by) {
	case SVG_SCALE_WIDTH:
		data->svg_render_width = data->target_width;
		data->svg_render_height = (uint32_t)((double)data->target_width * (double)data->texture_height / (double)data->texture_width);
		npt = next_power_of_2(data->target_width);
		data->textureIndex = min((uint32_t)log2(npt) - 3, data->textures.num - 1);
		break;
	case SVG_SCALE_HEIGHT:
		data->svg_render_height = data->target_height;
		data->svg_render_width = (uint32_t)((double)data->target_height * (double)data->texture_width / (double)data->texture_height);
		npt = next_power_of_2(data->target_height);
		data->textureIndex = min((uint32_t)log2(npt) - 3, data->textures.num - 1);
		break;
	case SVG_SCALE_BOTH:
		data->svg_render_width = data->target_width;
		data->svg_render_height = data->target_height;
		npt = next_power_of_2(data->target_width);
		data->textureIndex = min((uint32_t)log2(npt) - 3, data->textures.num - 1);
		break;
	}
}

void mask_svg_defaults(obs_data_t* settings) {
	//obs_data_set_default_double(settings, "mask_gradient_position", -1.e9);
	//obs_data_set_default_double(settings, "mask_gradient_width", 500.0);
}

void mask_svg_properties(obs_properties_t* props, mask_svg_data_t* data)
{
	obs_properties_t* mask_svg_group = obs_properties_create();
	obs_property_t* p;

	obs_property_t* file_path = obs_properties_add_path(
		mask_svg_group, "mask_svg_image",
		obs_module_text("AdvancedMasks.SvgMask.File"), OBS_PATH_FILE,
		"Textures (*.svg);;", NULL);

	//obs_property_set_modified_callback2(file_path,
	//	setting_file_path_modified, (void*)data);

	obs_property_t* scale_by = obs_properties_add_list(
		mask_svg_group, "mask_svg_scale_by",
		obs_module_text("AdvancedMasks.SvgMask.ScaleBy"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(SVG_SCALE_WIDTH_LABEL),
		SVG_SCALE_WIDTH);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(SVG_SCALE_HEIGHT_LABEL),
		SVG_SCALE_HEIGHT);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(SVG_SCALE_BOTH_LABEL),
		SVG_SCALE_BOTH);

	p = obs_properties_add_int_slider(
		mask_svg_group, "mask_svg_width",
		obs_module_text("AdvancedMasks.SvgMask.Width"), 0, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_svg_group, "mask_svg_height",
		obs_module_text("AdvancedMasks.SvgMask.Height"), 0, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_svg_group, "mask_pos_x",
		obs_module_text("AdvancedMasks.SvgMask.PosX"), -4000, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_svg_group, "mask_pos_y",
		obs_module_text("AdvancedMasks.SvgMask.PosY"), -4000, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "mask_svg_group",
		obs_module_text("AdvancedMasks.SvgMask.Label"),
		OBS_GROUP_NORMAL, mask_svg_group);
}

static void render_svg_to_texture(mask_svg_data_t* filter)
{
	for (size_t i = 0; i < filter->textures.num; i++)
	{
		gs_texture_destroy(filter->textures.array[i]);
	}
	da_clear(filter->textures);

	const int max_size = 4096;

	for (int i = 8; i <= max_size; i *= 2) {
		gs_texture_t* tex = gs_texture_from_svg(
			filter->svg_image_path.array,
			i,
			i,
			filter->scale_by
		);
		da_push_back(filter->textures, &tex);
		obs_enter_graphics();
		filter->texture_width = gs_texture_get_width(tex);
		filter->texture_height = gs_texture_get_height(tex);
		obs_leave_graphics();
	}
}

static bool setting_file_path_modified(void* data,
	obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings)
{
	mask_svg_data_t* filter = (mask_svg_data_t*)data;
	render_svg_to_texture(filter);
	return false;
}


static void load_svg_effect_files(mask_svg_data_t* data)
{
	load_mask_svg_effect(data);
}

static void load_mask_svg_effect(mask_svg_data_t* data)
{
	const char* effect_file_path = "/shaders/svg-mask.effect";

	data->effect_svg_mask = load_shader_effect(
		data->effect_svg_mask, effect_file_path);
	if (data->effect_svg_mask) {
		size_t effect_count =
			gs_effect_get_num_params(data->effect_svg_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
			effect_index++) {
			gs_eparam_t* param = gs_effect_get_param_by_idx(
				data->effect_svg_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				data->param_image = param;
			} else if (strcmp(info.name, "svg_image") == 0) {
				data->param_svg_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_uv_size = param;
			} else if (strcmp(info.name, "svg_uv_size") == 0) {
				data->param_svg_uv_size = param;
			} else if (strcmp(info.name, "offset") == 0) {
				data->param_offset = param;
			}
		}
	}
}

void render_mask_svg(mask_svg_data_t* data,
	base_filter_data_t* base)
{
	if (data->textures.num == 0) {
		return;
	}

	gs_effect_t* effect = data->effect_svg_mask;
	gs_texture_t* texture = gs_texrender_get_texture(base->input_texrender);

	gs_texture_t* svg_texture = data->textures.array[data->textureIndex];
	if (!effect || !texture || !svg_texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	if (data->param_image) {
		gs_effect_set_texture(data->param_image, texture);
	}

	if (data->param_svg_image) {
		gs_effect_set_texture(data->param_svg_image, svg_texture);
	}
	if (data->param_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(data->param_uv_size, &uv_size);
	}
	if (data->param_svg_uv_size) {
		struct vec2 svg_uv_size;
		svg_uv_size.x = (float)data->svg_render_width;
		svg_uv_size.y = (float)data->svg_render_height;
		gs_effect_set_vec2(data->param_svg_uv_size, &svg_uv_size);
	}
	if (data->param_offset) {
		struct vec2 offset;
		offset.x = (float)data->offset_x;
		offset.y = (float)data->offset_y;
		gs_effect_set_vec2(data->param_offset, &offset);
	}


	set_render_parameters();
	set_blending_parameters();
	const char* technique = "Draw";

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

uint32_t next_power_of_2(uint32_t n) {
	if (n == 0)
		return 1;
	// If already a power of 2, return n
	if ((n & (n - 1)) == 0)
		return n;

	// Else find the next power of 2
	uint32_t power = 1;
	while (power < n) {
		power <<= 1; // Same as power *= 2
	}
	return power;
}

uint32_t previous_power_of_2(uint32_t n) {
	if (n == 0)
		return 0;

	uint32_t power = 1;
	while (power << 1 <= n) {
		power <<= 1;
	}
	return power;
}
