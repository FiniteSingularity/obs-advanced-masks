#include "mask-svg.h"
#include "obs-utils.h"
#include "utils.h"
#include "advanced-masks-filter.h"
#include <math.h>

gs_texture_t* gs_texture_from_svg_path(const char* path, int width, int height, int scale_by);
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

mask_svg_data_t* mask_svg_create(obs_data_t* settings, base_filter_data_t* base)
{
	mask_svg_data_t* data = bzalloc(sizeof(mask_svg_data_t));
	dstr_init_copy(&data->svg_image_path, "!");
	dstr_init_copy(&data->svg_text, "!");

	load_svg_effect_files(data);

	da_init(data->textures);

	mask_svg_update(data, base, settings);
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

void set_mask_anchor(mask_svg_data_t* data, int anchor)
{
	switch (anchor) {
	case SVG_ANCHOR_TOP_LEFT:
		data->anchor.x = 0.0f;
		data->anchor.y = 0.0f;
		break;
	case SVG_ANCHOR_TOP_CENTER:
		data->anchor.x = 0.5f;
		data->anchor.y = 0.0f;
		break;
	case SVG_ANCHOR_TOP_RIGHT:
		data->anchor.x = 1.0f;
		data->anchor.y = 0.0f;
		break;
	case SVG_ANCHOR_CENTER_LEFT:
		data->anchor.x = 0.0f;
		data->anchor.y = 0.5f;
		break;
	case SVG_ANCHOR_CENTER_CENTER:
		data->anchor.x = 0.5f;
		data->anchor.y = 0.5f;
		break;
	case SVG_ANCHOR_CENTER_RIGHT:
		data->anchor.x = 1.0f;
		data->anchor.y = 0.5f;
		break;
	case SVG_ANCHOR_BOTTOM_LEFT:
		data->anchor.x = 0.0f;
		data->anchor.y = 1.0f;
		break;
	case SVG_ANCHOR_BOTTOM_CENTER:
		data->anchor.x = 0.5f;
		data->anchor.y = 1.0f;
		break;
	case SVG_ANCHOR_BOTTOM_RIGHT:
		data->anchor.x = 1.0f;
		data->anchor.y = 1.0f;
		break;
	}
}

void mask_svg_update(mask_svg_data_t* data, base_filter_data_t* base,
	obs_data_t* settings)
{
	uint32_t w = obs_source_get_width(base->context);
	uint32_t h = obs_source_get_height(base->context);

	if (w > 0) {
		obs_data_set_int(settings, "mask_source_width", w);
	}
	else {
		w = (uint32_t)obs_data_get_int(settings, "mask_source_width");
	}

	if (h > 0) {
		obs_data_set_int(settings, "mask_source_height", h);
	}
	else {
		h = (uint32_t)obs_data_get_int(settings, "mask_source_height");
	}

	mask_svg_defaults(settings, w, h);

	if (obs_data_get_int(settings, "mask_type") != MASK_TYPE_SVG) {
		return;
	}

	const char* svg_image_path = obs_data_get_string(settings, "mask_svg_image");
	const char* svg_text = obs_data_get_string(settings, "mask_svg_text");

	int input_type = (int)obs_data_get_int(settings, "mask_svg_input_type");
	uint32_t width = (uint32_t)obs_data_get_int(settings, "mask_svg_width");
	uint32_t height = (uint32_t)obs_data_get_int(settings, "mask_svg_height");
	uint32_t scaleBy = (uint32_t)obs_data_get_int(settings, "mask_svg_scale_by");
	uint32_t max_texture_size = (uint32_t)obs_data_get_int(settings, "mask_svg_max_texture_size");

	bool image_path_changed = false;
	bool scaleByChanged = data->scale_by != scaleBy;
	bool maxTextureSizeChanged = data->max_texture_size != max_texture_size;
	bool input_type_changed = data->input_type != input_type;

	if (input_type == SVG_INPUT_TYPE_FILE) {
		bool cur_img_path_zero = data->svg_image_path.len == 0;
		if (!cur_img_path_zero) {
			image_path_changed = dstr_cmp(&data->svg_image_path, svg_image_path) != 0;
		} else {
			image_path_changed = strlen(svg_image_path) > 0;
		}
	} else {
		bool cur_text_zero = data->svg_text.len == 0;
		if (!cur_text_zero) {
			image_path_changed = dstr_cmp(&data->svg_text, svg_text) != 0;
		} else {
			image_path_changed = strlen(svg_text) > 0;
		}
	}


	bool regen_bitmap = image_path_changed || input_type_changed;

	dstr_copy(&data->svg_image_path, svg_image_path);
	dstr_copy(&data->svg_text, svg_text);

	data->input_type = input_type;

	data->target_width = width;
	data->target_height = height;
	data->scale_by = scaleBy;

	data->offset_x = (int)obs_data_get_int(settings, "mask_pos_x");
	data->offset_y = (int)obs_data_get_int(settings, "mask_pos_y");
	data->max_texture_size = max_texture_size;

	data->rotation = (float)obs_data_get_double(settings, "mask_svg_rotation");
	data->invert = obs_data_get_bool(settings, "mask_svg_invert");

	bool empty = (input_type == SVG_INPUT_TYPE_FILE && dstr_is_empty(&data->svg_image_path)) || (input_type == SVG_INPUT_TYPE_TEXT && dstr_is_empty(&data->svg_text));

	if ((regen_bitmap || scaleByChanged || maxTextureSizeChanged) && !empty) {
		render_svg_to_texture(data);
	}

	const int anchor = (int)obs_data_get_int(settings, "mask_svg_anchor");
	if (anchor == SVG_ANCHOR_MANUAL) {
		data->anchor.x = (float)obs_data_get_double(settings, "mask_svg_anchor_x");
		data->anchor.y = (float)obs_data_get_double(settings, "mask_svg_anchor_y");
	} else {
		set_mask_anchor(data, anchor);
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

	float theta = data->rotation * M_PI / 180.0f;
	float x = data->anchor.x * data->svg_render_width;
	float y = data->anchor.y * data->svg_render_height;

	vec4_zero(&(data->rotation_matrix.x));
	vec4_zero(&data->rotation_matrix.y);
	vec4_zero(&data->rotation_matrix.z);
	vec4_zero(&data->rotation_matrix.t);
	data->rotation_matrix.x.x = cosf(theta);
	data->rotation_matrix.x.y = sinf(theta);
	data->rotation_matrix.y.x = -sinf(theta);
	data->rotation_matrix.y.y = cosf(theta);
	data->rotation_matrix.z.x = x - x * cosf(theta) + y * sinf(theta);
	data->rotation_matrix.z.y = y - x * sinf(theta) - y * cosf(theta);
	data->rotation_matrix.z.z = 1.0;
	data->rotation_matrix.t.x = 1.0;
	data->rotation_matrix.t.y = 1.0;
	data->rotation_matrix.t.w = 1.0;

	//data->rotation_matrix = {
	//	{                         cos(theta),                          sin(theta), 0.0, 0.0},
	//	{                        -sin(theta),                          cos(theta), 0.0, 0.0},
	//	{x - x * cos(theta) + y * sin(theta), y - x * sin(theta) - y * cos(theta), 1.0, 0.0},
	//	{                                1.0,                                 1.0, 0.0, 1.0}
	//};
}

void mask_svg_defaults(obs_data_t* settings, uint32_t width, uint32_t height)
{
	obs_data_set_default_int(settings, "mask_svg_width", 512);
	obs_data_set_default_int(settings, "mask_svg_height", 512);
	obs_data_set_default_int(settings, "mask_pos_x", width / 2);
	obs_data_set_default_int(settings, "mask_pos_y", height / 2);
	obs_data_set_default_int(settings, "mask_svg_anchor", SVG_ANCHOR_CENTER_CENTER);
	obs_data_set_default_bool(settings, "mask_svg_invert", false);
	obs_data_set_default_double(settings, "mask_svg_anchor_x", 0.5);
	obs_data_set_default_double(settings, "mask_svg_anchor_y", 0.5);
	obs_data_set_default_int(settings, "mask_svg_max_texture_size", max(width, height));
	obs_data_set_default_int(settings, "mask_svg_input_type", SVG_INPUT_TYPE_FILE);
}

void mask_svg_properties(obs_properties_t* props, mask_svg_data_t* data)
{
	obs_properties_t* mask_svg_group = obs_properties_create();
	obs_properties_t* mask_svg_advanced_group = obs_properties_create();
	obs_property_t* p;

	obs_property_t* input_type = obs_properties_add_list(
		mask_svg_group,
		"mask_svg_input_type",
		obs_module_text("AdvancedMasks.SvgMask.InputType"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT
	);

	obs_property_list_add_int(
		input_type,
		obs_module_text(SVG_INPUT_TYPE_FILE_LABEL),
		SVG_INPUT_TYPE_FILE);

	obs_property_list_add_int(
		input_type,
		obs_module_text(SVG_INPUT_TYPE_TEXT_LABEL),
		SVG_INPUT_TYPE_TEXT);

	obs_property_set_modified_callback(input_type, input_type_changed);

	obs_property_t* file_path = obs_properties_add_path(
		mask_svg_group, "mask_svg_image",
		obs_module_text("AdvancedMasks.SvgMask.File"), OBS_PATH_FILE,
		"Textures (*.svg);;", NULL);

	obs_property_t* text_input = obs_properties_add_text(
		mask_svg_group,
		"mask_svg_text",
		obs_module_text("AdvancedMasks.SvgMask.Text"),
		OBS_TEXT_MULTILINE
	);

	obs_property_t* anchor = obs_properties_add_list(mask_svg_group, "mask_svg_anchor",
		obs_module_text("AdvancedMasks.SvgMask.Anchor"), OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_TOP_LEFT_LABEL),
		SVG_ANCHOR_TOP_LEFT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_TOP_CENTER_LABEL),
		SVG_ANCHOR_TOP_CENTER);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_TOP_RIGHT_LABEL),
		SVG_ANCHOR_TOP_RIGHT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_CENTER_LEFT_LABEL),
		SVG_ANCHOR_CENTER_LEFT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_CENTER_CENTER_LABEL),
		SVG_ANCHOR_CENTER_CENTER);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_CENTER_RIGHT_LABEL),
		SVG_ANCHOR_CENTER_RIGHT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_BOTTOM_LEFT_LABEL),
		SVG_ANCHOR_BOTTOM_LEFT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_BOTTOM_CENTER_LABEL),
		SVG_ANCHOR_BOTTOM_CENTER);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_BOTTOM_RIGHT_LABEL),
		SVG_ANCHOR_BOTTOM_RIGHT);

	obs_property_list_add_int(
		anchor,
		obs_module_text(SVG_ANCHOR_MANUAL_LABEL),
		SVG_ANCHOR_MANUAL);

	obs_property_set_modified_callback(anchor, svg_anchor_changed);

	obs_properties_add_float_slider(
		mask_svg_group, "mask_svg_anchor_x",
		obs_module_text("AdvancedMasks.SvgMask.AnchorX"),
		0.0f, 1.0f, 0.01f
	);

	obs_properties_add_float_slider(
		mask_svg_group, "mask_svg_anchor_y",
		obs_module_text("AdvancedMasks.SvgMask.AnchorY"),
		0.0f, 1.0f, 0.01f
	);

	obs_properties_add_bool(mask_svg_group, "mask_svg_invert", "Invert?");

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

	obs_property_set_modified_callback(scale_by, scale_by_changed);

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

	p = obs_properties_add_float_slider(
		mask_svg_group, "mask_svg_rotation",
		obs_module_text("AdvancedMasks.SvgMask.Rotation"), -360.0, 360.0, 1.0);
	obs_property_float_set_suffix(p, "deg");

	obs_properties_add_group(
		props, "mask_svg_group",
		obs_module_text("AdvancedMasks.SvgMask.Label"),
		OBS_GROUP_NORMAL, mask_svg_group);

	p = obs_properties_add_int(
		mask_svg_advanced_group, "mask_svg_max_texture_size",
		obs_module_text("AdvancedMasks.SvgMask.MaxTextureSize"), 8, 8196, 1);

	obs_properties_add_group(
		props, "mask_svg_advanced_group",
		obs_module_text("AdvancedMasks.SvgMask.Advanced"),
		OBS_GROUP_NORMAL, mask_svg_advanced_group);
}

bool svg_anchor_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings)
{
	bool manual = obs_data_get_int(settings, "mask_svg_anchor") == SVG_ANCHOR_MANUAL;
	setting_visibility("mask_svg_anchor_x", manual, props);
	setting_visibility("mask_svg_anchor_y", manual, props);
	return true;
}

static void render_svg_to_texture(mask_svg_data_t* filter)
{
	gs_texture_t* (*gen_texture)(const char*, int, int, int);

	char* svg_data;

	if (filter->input_type == SVG_INPUT_TYPE_FILE) {
		gen_texture = &gs_texture_from_svg_path;
		svg_data = filter->svg_image_path.array;
	} else {
		gen_texture = &gs_texture_from_svg;
		svg_data = filter->svg_text.array;
	}

	for (size_t i = 0; i < filter->textures.num; i++)
	{
		gs_texture_destroy(filter->textures.array[i]);
	}
	da_clear(filter->textures);

	const int max_size = (int)filter->max_texture_size;
	int lastTexture = 1000000;

	for (int i = 8; i <= max_size; i *= 2) {
		gs_texture_t* tex = gen_texture(
			svg_data,
			i,
			i,
			filter->scale_by
		);
		da_push_back(filter->textures, &tex);
		obs_enter_graphics();
		filter->texture_width = gs_texture_get_width(tex);
		filter->texture_height = gs_texture_get_height(tex);
		obs_leave_graphics();
		lastTexture = i;
	}
	if (lastTexture < max_size) {
		gs_texture_t* tex = gen_texture(
			svg_data,
			max_size,
			max_size,
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
			}
			else if (strcmp(info.name, "svg_image") == 0) {
				data->param_svg_image = param;
			}
			else if (strcmp(info.name, "uv_size") == 0) {
				data->param_uv_size = param;
			}
			else if (strcmp(info.name, "svg_uv_size") == 0) {
				data->param_svg_uv_size = param;
			}
			else if (strcmp(info.name, "offset") == 0) {
				data->param_offset = param;
			}
			else if (strcmp(info.name, "primary_alpha") == 0) {
				data->param_primary_alpha = param;
			}
			else if (strcmp(info.name, "secondary_alpha") == 0) {
				data->param_secondary_alpha = param;
			}
			else if (strcmp(info.name, "invert") == 0) {
				data->param_invert = param;
			}
			else if (strcmp(info.name, "anchor") == 0) {
				data->param_anchor = param;
			}
			else if (strcmp(info.name, "rotation_matrix") == 0) {
				data->param_rotation_matrix = param;
			}
			else if (strcmp(info.name, "min_brightness") == 0) {
				data->param_min_brightness = param;
			}
			else if (strcmp(info.name, "max_brightness") == 0) {
				data->param_max_brightness = param;
			}
			else if (strcmp(info.name, "min_contrast") == 0) {
				data->param_min_contrast = param;
			}
			else if (strcmp(info.name, "max_contrast") == 0) {
				data->param_max_contrast = param;
			}
			else if (strcmp(info.name, "min_saturation") == 0) {
				data->param_min_saturation = param;
			}
			else if (strcmp(info.name, "max_saturation") == 0) {
				data->param_max_saturation = param;
			}
			else if (strcmp(info.name, "min_hue_shift") == 0) {
				data->param_min_hue_shift = param;
			}
			else if (strcmp(info.name, "max_hue_shift") == 0) {
				data->param_max_hue_shift = param;
			}
		}
	}
}

void render_mask_svg(mask_svg_data_t* data,
	base_filter_data_t* base, color_adjustments_data_t* color_adj)
{
	if (data->textures.num == 0) {
		obs_source_skip_video_filter(base->context);
		return;
	}
	gs_texture_t* svg_texture = data->textures.array[data->textureIndex];
	if (!svg_texture) {
		obs_source_skip_video_filter(base->context);
		return;
	}

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
		const char* technique = base->mask_effect == MASK_EFFECT_ALPHA
			? "DrawFA"
			: "DrawFAAdjustments";
		const enum gs_color_format format = gs_get_format_from_space(source_space);
		if (obs_source_process_filter_begin_with_color_space(base->context, format, source_space,
			OBS_ALLOW_DIRECT_RENDERING)) {
			gs_effect_set_texture(data->param_svg_image, svg_texture);
			struct vec2 uv_size;
			uv_size.x = (float)base->width;
			uv_size.y = (float)base->height;
			gs_effect_set_vec2(data->param_uv_size, &uv_size);

			struct vec2 svg_uv_size;
			svg_uv_size.x = (float)data->svg_render_width;
			svg_uv_size.y = (float)data->svg_render_height;
			gs_effect_set_vec2(data->param_svg_uv_size, &svg_uv_size);

			struct vec2 offset;
			offset.x = (float)data->offset_x;
			offset.y = (float)data->offset_y;
			gs_effect_set_vec2(data->param_offset, &offset);

			gs_effect_set_float(data->param_primary_alpha, 1.0f);
			gs_effect_set_float(data->param_secondary_alpha, 1.0f);
			gs_effect_set_float(data->param_invert, data->invert ? 1.0f : 0.0f);
			gs_effect_set_vec2(data->param_anchor, &data->anchor);
			gs_effect_set_matrix4(data->param_rotation_matrix, &data->rotation_matrix);

			if (base->mask_effect == MASK_EFFECT_ADJUSTMENT)
			{
				const float min_brightness = color_adj->adj_brightness
					? color_adj->min_brightness
					: 0.0f;
				gs_effect_set_float(data->param_min_brightness, min_brightness);
				const float max_brightness = color_adj->adj_brightness
					? color_adj->max_brightness
					: 0.0f;
				gs_effect_set_float(data->param_max_brightness, max_brightness);

				const float min_contrast = color_adj->adj_contrast
					? color_adj->min_contrast
					: 0.0f;
				gs_effect_set_float(data->param_min_contrast, min_contrast);
				const float max_contrast = color_adj->adj_contrast
					? color_adj->max_contrast
					: 0.0f;
				gs_effect_set_float(data->param_max_contrast, max_contrast);

				const float min_saturation = color_adj->adj_saturation
					? color_adj->min_saturation
					: 1.0f;
				gs_effect_set_float(data->param_min_saturation, min_saturation);
				const float max_saturation = color_adj->adj_saturation
					? color_adj->max_saturation
					: 1.0f;
				gs_effect_set_float(data->param_max_saturation, max_saturation);

				const float min_hue_shift = color_adj->adj_hue_shift
					? color_adj->min_hue_shift
					: 0.0f;
				gs_effect_set_float(data->param_min_hue_shift, min_hue_shift);

				const float max_hue_shift = color_adj->adj_hue_shift
					? color_adj->max_hue_shift
					: 1.0f;
				gs_effect_set_float(data->param_max_hue_shift, max_hue_shift);
			}

			gs_blend_state_push();
			gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

			obs_source_process_filter_tech_end(base->context, data->effect_svg_mask, 0, 0, technique);
			gs_blend_state_pop();
		}
	}
}

bool scale_by_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings)
{
	int scale_by = (int)obs_data_get_int(settings, "mask_svg_scale_by");
	switch (scale_by) {
	case SVG_SCALE_WIDTH:
		setting_visibility("mask_svg_width", true, props);
		setting_visibility("mask_svg_height", false, props);
		break;
	case SVG_SCALE_HEIGHT:
		setting_visibility("mask_svg_width", false, props);
		setting_visibility("mask_svg_height", true, props);
		break;
	case SVG_SCALE_BOTH:
		setting_visibility("mask_svg_width", true, props);
		setting_visibility("mask_svg_height", true, props);
		break;
	}
	return true;
}

bool input_type_changed(obs_properties_t* props,
	obs_property_t* property, obs_data_t* settings)
{
	int input_type = (int)obs_data_get_int(settings, "mask_svg_input_type");
	switch (input_type) {
	case SVG_INPUT_TYPE_FILE:
		setting_visibility("mask_svg_image", true, props);
		setting_visibility("mask_svg_text", false, props);
		break;
	case SVG_INPUT_TYPE_TEXT:
		setting_visibility("mask_svg_image", false, props);
		setting_visibility("mask_svg_text", true, props);
		break;
	}
	return true;
}
