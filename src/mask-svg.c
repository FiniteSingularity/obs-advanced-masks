#include "mask-svg.h"
#include "obs-utils.h"
#include "advanced-masks-filter.h"

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
	bool cur_img_path_zero = data->svg_image_path.len == 0;
	if (!cur_img_path_zero) {
		image_path_changed = !dstr_cmp(&data->svg_image_path, svg_image_path);
	} else {
		image_path_changed = strlen(svg_image_path) > 0;
	}

	bool regen_bitmap = width != data->target_width ||
			    height != data->target_height ||
			    scaleBy != data->scale_by ||
		            image_path_changed;

	dstr_copy(&data->svg_image_path, svg_image_path);
	data->target_width = width;
	data->target_height = height;
	data->scale_by = scaleBy;

	if (regen_bitmap && !dstr_is_empty(&data->svg_image_path)) {
		render_svg_to_texture(data);
	}
	switch (data->scale_by) {
	case SVG_SCALE_WIDTH:
		data->svg_render_width = data->target_width;
		data->svg_render_height = (uint32_t)((double)data->target_width * (double)data->texture_height / (double)data->texture_width);
		break;
	case SVG_SCALE_HEIGHT:
		data->svg_render_height = data->target_height;
		data->svg_render_width = (uint32_t)((double)data->target_height * (double)data->texture_width / (double)data->texture_height);
		break;
	case SVG_SCALE_BOTH:
		data->svg_render_width = data->target_width;
		data->svg_render_height = data->target_height;
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

	obs_properties_add_group(
		props, "mask_svg_group",
		obs_module_text("AdvancedMasks.SvgMask.Label"),
		OBS_GROUP_NORMAL, mask_svg_group);
}

static void render_svg_to_texture(mask_svg_data_t* filter)
{

	uint32_t new_width = next_power_of_2(filter->target_width);
	uint32_t new_height = next_power_of_2(filter->target_height);

	bool larger = (filter->scale_by == SVG_SCALE_WIDTH && new_width > filter->texture_width) || (filter->scale_by == SVG_SCALE_HEIGHT && new_height > filter->texture_height);
	bool smaller = (filter->scale_by == SVG_SCALE_WIDTH && new_width < filter->texture_width) || (filter->scale_by == SVG_SCALE_HEIGHT && new_height < filter->texture_height);

	if (filter->scale_by == SVG_SCALE_WIDTH && filter->texture_width == new_width) {
		return;
	} else if (filter->scale_by == SVG_SCALE_HEIGHT && filter->texture_height == new_height) {
		return;
	} else if (filter->scale_by == SVG_SCALE_BOTH && filter->texture_width == new_width && filter->texture_height == new_height) {
		return;
	}

	
	//if (filter->imageTexture) {
	//	obs_enter_graphics();
	//	gs_texture_destroy(filter->imageTexture);
	//	obs_leave_graphics();
	//}
	//obs_enter_graphics();
	//filter->imageTexture = gs_texture_from_svg(filter->svg_image_path.array, new_width, new_height, filter->scale_by);
	if (filter->current_tex == NULL) {
		update_svg_textures(
			filter->svg_image_path.array, new_width, new_height, filter->scale_by,
			SVG_GENERATE_ALL,
			&filter->next_size_smaller_tex,
			&filter->current_tex,
			&filter->next_size_greater_tex
		);
	} 
	else if (larger) {
		update_svg_textures(
			filter->svg_image_path.array, new_width, new_height, filter->scale_by,
			SVG_GENERATE_NEW_LARGE,
			&filter->next_size_smaller_tex,
			&filter->current_tex,
			&filter->next_size_greater_tex
		);
	} else if (smaller) {
		update_svg_textures(
			filter->svg_image_path.array, new_width, new_height, filter->scale_by,
			SVG_GENERATE_NEW_SMALL,
			&filter->next_size_smaller_tex,
			&filter->current_tex,
			&filter->next_size_greater_tex
		);
	}
	obs_enter_graphics();
	filter->texture_width = gs_texture_get_width(filter->current_tex);
	filter->texture_height = gs_texture_get_height(filter->current_tex);
	blog(LOG_INFO, "Regenerating Texture (%ix%i)", filter->texture_width, filter->texture_height);
	obs_leave_graphics();
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
			if (strcmp(info.name, "svg_image") == 0) {
				data->param_svg_image = param;
			} else if (strcmp(info.name, "uv_size") == 0) {
				data->param_uv_size = param;
			} else if (strcmp(info.name, "svg_uv_size") == 0) {
				data->param_svg_uv_size = param;
			}
		}
	}
}

void render_mask_svg(mask_svg_data_t* data,
	base_filter_data_t* base)
{
	gs_effect_t* effect = data->effect_svg_mask;
	gs_texture_t* texture = gs_texrender_get_texture(base->input_texrender);
	if (!effect || !texture || !data->current_tex) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	if (data->param_svg_image) {
		gs_effect_set_texture(data->param_svg_image, data->current_tex);
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
