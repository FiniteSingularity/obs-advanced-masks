#include "mask-source.h"
#include "obs-utils.h"

mask_source_data_t *mask_source_create(obs_data_t *settings)
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
	data->loading_effect = false;
	mask_source_update(data, settings);
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

void mask_source_update(mask_source_data_t *data,
			obs_data_t *settings)
{
	uint32_t mask_width = 0;
	uint32_t mask_height = 0;
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
		mask_width = obs_source_get_width(mask_source);
		mask_height = obs_source_get_height(mask_source);
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

	data->mask_source_scale_by =
		(uint32_t)obs_data_get_int(settings, "mask_source_scale_by");

	switch (data->mask_source_scale_by) {
	case MASK_SOURCE_SCALE_BY_PERCENT: {
		data->mask_scale_pct =
			(float)obs_data_get_double(settings, "mask_source_mask_pct") /
			100.0f;
		break;
	}
	case MASK_SOURCE_SCALE_BY_WIDTH: {
		data->mask_scale_width = (float)obs_data_get_double(
			settings, "mask_source_mask_width");
		break;
	}
	case MASK_SOURCE_SCALE_BY_HEIGHT: {
		data->mask_scale_height = (float)obs_data_get_double(
			settings, "mask_source_mask_height");
		break;
	}
	case MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT: {
		data->mask_scale_width = (float)obs_data_get_double(
			settings, "mask_source_mask_width");
		data->mask_scale_height = (float)obs_data_get_double(
			settings, "mask_source_mask_height");
		break;
	}
	}
	data->mask_source_scaling = (uint32_t)obs_data_get_int(settings, "mask_source_scaling_type");

	data->mask_offset.x = (float)obs_data_get_double(settings, "mask_source_mask_offset_x");
	data->mask_offset.y = (float)obs_data_get_double(settings, "mask_source_mask_offset_y");
	data->boundary_horizontal = (uint32_t)obs_data_get_int(settings, "mask_source_boundary_horiz");
	data->boundary_vertical = (uint32_t)obs_data_get_int(settings, "mask_source_boundary_vert");
	//data->positional_alignment = (uint32_t)obs_data_get_int(settings, "mask_source_positional_alignment");
	uint32_t pa = (uint32_t)obs_data_get_int(settings, "mask_source_positional_alignment");
	data->positional_alignment.x = get_position_width_factor(pa);
	data->positional_alignment.y = get_position_height_factor(pa);
	data->rotation = (float)(obs_data_get_double(settings, "mask_source_mask_rotation") * M_PI / 180.0);
	data->mask_position.x = (float)obs_data_get_double(settings, "mask_source_mask_position_x");
	data->mask_position.y = (float)obs_data_get_double(settings, "mask_source_mask_position_y");
}

void mask_source_defaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, "mask_source_mask_width", -1.0);
	obs_data_set_default_double(settings, "mask_source_mask_height", -1.0);
	obs_data_set_default_double(settings, "mask_source_mask_pct", 100.0);
}

void source_mask_top_properties(obs_properties_t *props,
				mask_source_data_t *data)
{
	obs_property_t *mask_source = obs_properties_add_list(
		props, "mask_source",
		obs_module_text("AdvancedMasks.SourceMask.Source"),
		OBS_COMBO_TYPE_EDITABLE, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(
		mask_source, obs_module_text("AdvancedMasks.Common.None"), "");
	obs_enum_sources(add_source_to_list, mask_source);
	obs_enum_scenes(add_source_to_list, mask_source);

	obs_property_set_modified_callback(mask_source,
					   setting_mask_source_source_modified);

	obs_properties_add_path(
		props, "mask_source_image",
		obs_module_text("AdvancedMasks.SourceMask.File"), OBS_PATH_FILE,
		"Textures (*.bmp *.tga *.png *.jpeg *.jpg *.gif);;", NULL);

	obs_property_t *source_scaling = obs_properties_add_list(
		props, "mask_source_scaling_type",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		source_scaling,
		obs_module_text(MASK_SOURCE_SCALING_STRETCH_LABEL),
		MASK_SOURCE_SCALING_STRETCH);
	obs_property_list_add_int(
		source_scaling,
		obs_module_text(MASK_SOURCE_SCALING_MANUAL_LABEL),
		MASK_SOURCE_SCALING_MANUAL);

	obs_property_set_modified_callback2(source_scaling,
					   setting_mask_source_scaling_modified, (void*)data);
}
//load_source_mask_effect(mask_source_data_t *data)
void source_mask_bot_properties(obs_properties_t *props,
				mask_source_data_t *data)
{
	obs_properties_t *mask_source_scaling_group = obs_properties_create();
	obs_property_t *p = NULL;

	obs_property_t *source_scale_by = obs_properties_add_list(
		mask_source_scaling_group, "mask_source_scale_by",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling.ScaleBy"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		source_scale_by,
		obs_module_text(MASK_SOURCE_SCALE_BY_PERCENT_LABEL),
		MASK_SOURCE_SCALE_BY_PERCENT);

	obs_property_list_add_int(
		source_scale_by,
		obs_module_text(MASK_SOURCE_SCALE_BY_WIDTH_LABEL),
		MASK_SOURCE_SCALE_BY_WIDTH);

	obs_property_list_add_int(
		source_scale_by,
		obs_module_text(MASK_SOURCE_SCALE_BY_HEIGHT_LABEL),
		MASK_SOURCE_SCALE_BY_HEIGHT);

	obs_property_list_add_int(
		source_scale_by,
		obs_module_text(MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT_LABEL),
		MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT);

	obs_property_set_modified_callback2(
		source_scale_by, setting_mask_source_scale_by_modified, data);

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_pct",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling.Percent"),
		0, 1000.0, 0.1);
	obs_property_float_set_suffix(p, "%");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_width",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling.Width"),
		0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_height",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling.Height"),
		0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_position_x",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.PositionX"),
		-5000.0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_position_y",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.PositionY"),
		-5000.0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_rotation",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.Rotation"),
		-360.0, 360.0, 0.1);
	obs_property_float_set_suffix(p, "°");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_offset_x",
		obs_module_text("AdvancedMasks.SourceMask.SourceScaling.OffsetX"),
		-5000.0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_float_slider(
		mask_source_scaling_group, "mask_source_mask_offset_y",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.OffsetY"),
		-5000.0, 5000.0, 1.0);
	obs_property_float_set_suffix(p, "px");

	obs_property_t *positional_alignment = obs_properties_add_list(
		mask_source_scaling_group, "mask_source_positional_alignment",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.PositionalAlignment"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_TL_LABEL),
		MASK_SOURCE_MASK_POSITION_TL);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_TC_LABEL),
		MASK_SOURCE_MASK_POSITION_TC);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_TR_LABEL),
		MASK_SOURCE_MASK_POSITION_TR);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_CL_LABEL),
		MASK_SOURCE_MASK_POSITION_CL);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_CC_LABEL),
		MASK_SOURCE_MASK_POSITION_CC);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_CR_LABEL),
		MASK_SOURCE_MASK_POSITION_CR);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_BL_LABEL),
		MASK_SOURCE_MASK_POSITION_BL);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_BC_LABEL),
		MASK_SOURCE_MASK_POSITION_BC);
	obs_property_list_add_int(
		positional_alignment,
		obs_module_text(MASK_SOURCE_MASK_POSITION_BR_LABEL),
		MASK_SOURCE_MASK_POSITION_BR);

	obs_property_t *source_boundary_horiz = obs_properties_add_list(
		mask_source_scaling_group, "mask_source_boundary_horiz",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.BoundaryHoriz"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		source_boundary_horiz,
		obs_module_text(MASK_SOURCE_BOUNDARY_NONE_LABEL),
		MASK_SOURCE_BOUNDARY_NONE);
	
	obs_property_list_add_int(
		source_boundary_horiz,
		obs_module_text(MASK_SOURCE_BOUNDARY_EXTEND_LABEL),
		MASK_SOURCE_BOUNDARY_EXTEND);

	obs_property_list_add_int(
		source_boundary_horiz,
		obs_module_text(MASK_SOURCE_BOUNDARY_TILE_LABEL),
		MASK_SOURCE_BOUNDARY_TILE);

	obs_property_list_add_int(
		source_boundary_horiz,
		obs_module_text(MASK_SOURCE_BOUNDARY_MIRROR_LABEL),
		MASK_SOURCE_BOUNDARY_MIRROR);

	obs_property_set_modified_callback2(
		source_boundary_horiz, setting_mask_source_boundary_modified, data);

	obs_property_t *source_boundary_vert = obs_properties_add_list(
		mask_source_scaling_group, "mask_source_boundary_vert",
		obs_module_text(
			"AdvancedMasks.SourceMask.SourceScaling.BoundaryVert"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		source_boundary_vert,
		obs_module_text(MASK_SOURCE_BOUNDARY_NONE_LABEL),
		MASK_SOURCE_BOUNDARY_NONE);

	obs_property_list_add_int(
		source_boundary_vert,
		obs_module_text(MASK_SOURCE_BOUNDARY_EXTEND_LABEL),
		MASK_SOURCE_BOUNDARY_EXTEND);

	obs_property_list_add_int(
		source_boundary_vert,
		obs_module_text(MASK_SOURCE_BOUNDARY_TILE_LABEL),
		MASK_SOURCE_BOUNDARY_TILE);

	obs_property_list_add_int(
		source_boundary_vert,
		obs_module_text(MASK_SOURCE_BOUNDARY_MIRROR_LABEL),
		MASK_SOURCE_BOUNDARY_MIRROR);

	obs_property_set_modified_callback2(
		source_boundary_vert, setting_mask_source_boundary_modified,
		data);

	obs_properties_add_group(
		props, "mask_source_scaling_group",
		obs_module_text("AdvancedMasks.SourceMask.ScalingParameters"),
		OBS_GROUP_NORMAL, mask_source_scaling_group);

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

static bool setting_mask_source_scale_by_modified(void *data,
						  obs_properties_t *props,
						  obs_property_t *p,
						  obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	mask_source_data_t *filter = data;
	uint32_t prior = filter->mask_source_scale_by;

	uint32_t scale_by = (uint32_t)obs_data_get_int(settings,
					    "mask_source_scale_by");

	if (prior != scale_by) {
		obs_source_t *mask =
			obs_weak_source_get_source(filter->mask_source_source);
		uint32_t mask_width = obs_source_get_width(mask);
		uint32_t mask_height = obs_source_get_height(mask);
		obs_source_release(mask);
		switch (prior) {
		case MASK_SOURCE_SCALE_BY_PERCENT: {
			float scale = filter->mask_scale_pct;
			obs_data_set_double(settings, "mask_source_mask_width",
					    (float)mask_width * scale);
			obs_data_set_double(settings, "mask_source_mask_height",
					    (float)mask_height * scale);
			break;
		}
		case MASK_SOURCE_SCALE_BY_WIDTH: {
			float scale = filter->mask_scale_width / mask_width;
			obs_data_set_double(settings, "mask_source_mask_pct",
					    scale * 100.0);
			obs_data_set_double(settings, "mask_source_mask_height",
					    (float)mask_height * scale);
			break;
		}
		case MASK_SOURCE_SCALE_BY_HEIGHT: {
			float scale = filter->mask_scale_height / mask_height;
			obs_data_set_double(settings, "mask_source_mask_pct",
					    scale * 100.0);
			obs_data_set_double(settings, "mask_source_mask_width",
					    (float)mask_width * scale);
			break;
		}
		case MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT: {
			float scale = filter->mask_scale_width / mask_width;
			obs_data_set_double(settings, "mask_source_mask_pct",
					    scale * 100.0);
			obs_data_set_double(settings, "mask_source_mask_height",
					    (float)mask_height * scale);
			break;
		}
		}
	}
	switch (scale_by) {
	case MASK_SOURCE_SCALE_BY_PERCENT:
		setting_visibility("mask_source_mask_pct", true, props);
		setting_visibility("mask_source_mask_width", false, props);
		setting_visibility("mask_source_mask_height", false, props);
		break;
	case MASK_SOURCE_SCALE_BY_WIDTH:
		setting_visibility("mask_source_mask_pct", false, props);
		setting_visibility("mask_source_mask_width", true, props);
		setting_visibility("mask_source_mask_height", false, props);
		break;
	case MASK_SOURCE_SCALE_BY_HEIGHT:
		setting_visibility("mask_source_mask_pct", false, props);
		setting_visibility("mask_source_mask_width", false, props);
		setting_visibility("mask_source_mask_height", true, props);
		break;
	case MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT:
		setting_visibility("mask_source_mask_pct", false, props);
		setting_visibility("mask_source_mask_width", true, props);
		setting_visibility("mask_source_mask_height", true, props);
		break;
	}
	return true;
}


static bool setting_mask_source_scaling_modified(void *data,
						 obs_properties_t *props,
						 obs_property_t *p,
						 obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	mask_source_data_t *filter = data;
	uint32_t prior = filter->mask_source_scaling;
	uint32_t current = (uint32_t)obs_data_get_int(
		settings, "mask_source_scaling_type");
	if (prior != current) {
		filter->mask_source_scaling = current;
		load_source_mask_effect(filter);
		setting_visibility("mask_source_scaling_group",
				   filter->mask_source_scaling == MASK_SOURCE_SCALING_MANUAL,
				   props);
		return true;
	}
	return false;
}

static bool setting_mask_source_boundary_modified(void *data,
						  obs_properties_t *props,
						  obs_property_t *p,
						  obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	mask_source_data_t *filter = data;
	uint32_t prior_horiz = filter->boundary_horizontal;
	uint32_t prior_vert = filter->boundary_vertical;
	uint32_t cur_horiz = (uint32_t)obs_data_get_int(
		settings, "mask_source_boundary_horiz");
	uint32_t cur_vert = (uint32_t)obs_data_get_int(
		settings, "mask_source_boundary_vert");

	if (prior_horiz != cur_horiz || prior_vert != cur_vert) {
		filter->boundary_horizontal = cur_horiz;
		filter->boundary_vertical = cur_vert;
		load_source_mask_effect(filter);
	}

	return false;
}

static bool setting_mask_source_source_modified(obs_properties_t* props,
	obs_property_t* p,
	obs_data_t* settings)
{
	UNUSED_PARAMETER(p);
	float width_setting =
		(float)obs_data_get_double(settings, "mask_source_mask_width");
	if (width_setting > -0.001) {
		return false;
	}
	const char *mask_source_name =
		obs_data_get_string(settings, "mask_source");
	obs_source_t *mask_source =
		(mask_source_name && strlen(mask_source_name))
			? obs_get_source_by_name(mask_source_name)
			: NULL;

	if (mask_source) {
		uint32_t width = obs_source_get_width(mask_source);
		uint32_t height = obs_source_get_height(mask_source);
		obs_data_set_double(settings, "mask_source_mask_width",
				    (float)width);
		obs_data_set_double(settings, "mask_source_mask_height",
				    (float)height);
		obs_source_release(mask_source);
		return true;
	}
	return false;
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
			      color_adjustments_data_t *color_adj,
			      gs_texture_t *texture)
{
	if (data->param_source_mask_image) {
		gs_effect_set_texture(data->param_source_mask_image, texture);
	}

	if (data->param_source_mask_positional_offset) {
		gs_effect_set_vec2(data->param_source_mask_positional_offset,
				   &data->positional_alignment);
	}

	if (data->param_source_mask_rotation) {
		gs_effect_set_float(data->param_source_mask_rotation,
				   data->rotation);
	}

	if (data->param_source_mask_position) {
		gs_effect_set_vec2(data->param_source_mask_position,
				   &data->mask_position);
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
	if (!effect || !texture || data->loading_effect) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	set_render_params(data, color_adj, texture);

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
	data->source_size.x = (float)base->width;
	data->source_size.y = (float)base->height;

	switch (data->mask_source_scale_by) {
	case MASK_SOURCE_SCALE_BY_PERCENT:
		data->mask_source_size.x = (float)base_width * data->mask_scale_pct;
		data->mask_source_size.y = (float)base_height * data->mask_scale_pct;
		break;
	case MASK_SOURCE_SCALE_BY_WIDTH:
		data->mask_source_size.x = data->mask_scale_width;
		data->mask_source_size.y = (float)base_height *
					   data->mask_scale_width /
					   (float)base_width;
		break;
	case MASK_SOURCE_SCALE_BY_HEIGHT:
		data->mask_source_size.y = data->mask_scale_height;
		data->mask_source_size.x = (float)base_width *
					   data->mask_scale_height /
					   (float)base_height;
		break;
	case MASK_SOURCE_SCALE_BY_WIDTH_HEIGHT:
		data->mask_source_size.x = data->mask_scale_width;
		data->mask_source_size.y = data->mask_scale_height;
		break;
	}


	if (data->param_source_source_image_size) {
		gs_effect_set_vec2(data->param_source_source_image_size,
				   &data->source_size);
	}

	if (data->param_source_mask_image_size) {
		gs_effect_set_vec2(data->param_source_mask_image_size,
				   &data->mask_source_size);
	}

	if (data->param_source_mask_offset) {
		gs_effect_set_vec2(data->param_source_mask_offset,
				   &data->mask_offset);
	}

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

	set_render_params(data, color_adj, texture);

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

static const char* get_addr_mode(uint32_t index) {
	switch (index) {
	case MASK_SOURCE_BOUNDARY_EXTEND:
		return "Clamp";
	case MASK_SOURCE_BOUNDARY_TILE:
		return "Wrap";
	case MASK_SOURCE_BOUNDARY_MIRROR:
		return "Mirror";
	}
	return "Border";
}

// Loads the shader file at `effect_file_path` into *effect
gs_effect_t *load_source_mask_file(gs_effect_t *effect,
				   const char *effect_file_path,
				   mask_source_data_t *data)
{
	if (effect != NULL) {
		obs_enter_graphics();
		gs_effect_destroy(effect);
		effect = NULL;
		obs_leave_graphics();
	}
	struct dstr shader_text = {0};
	struct dstr filename = {0};

	// Determine what defines we need at the top of our shader.


	dstr_cat(&filename, obs_get_module_data_path(obs_current_module()));
	dstr_cat(&filename, effect_file_path);
	char *errors = NULL;

	const char *defines =
		(data->mask_source_scaling == MASK_SOURCE_SCALING_MANUAL)
			? "#define MANUAL_SCALING 1"
			: "";

	dstr_cat(&shader_text, defines);
	dstr_cat(&shader_text, load_shader_from_file(filename.array));

	dstr_replace(&shader_text, "<ADDR_U>", get_addr_mode(data->boundary_horizontal));
	dstr_replace(&shader_text, "<ADDR_V>", get_addr_mode(data->boundary_vertical));

	obs_enter_graphics();
	effect = gs_effect_create(shader_text.array, NULL, &errors);
	obs_leave_graphics();

	if (effect == NULL) {
		blog(LOG_WARNING,
		     "[obs-composite-blur] Unable to load .effect file.  Errors:\n%s",
		     (errors == NULL || strlen(errors) == 0 ? "(None)"
							    : errors));
		bfree(errors);
	}

	dstr_free(&filename);
	dstr_free(&shader_text);

	return effect;
}

static void load_source_mask_effect(mask_source_data_t *data)
{
	data->loading_effect = true;
	const char *effect_file_path = "/shaders/source-mask.effect";

	data->effect_source_mask = load_source_mask_file(
		data->effect_source_mask, effect_file_path, data);
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
			} else if (strcmp(info.name, "source_image_size") == 0) {
				data->param_source_source_image_size = param;
			} else if (strcmp(info.name, "mask_image_size") == 0) {
				data->param_source_mask_image_size = param;
			} else if (strcmp(info.name, "mask_offset") == 0) {
				data->param_source_mask_offset = param;
			} else if (strcmp(info.name, "mask_rotation") == 0) {
				data->param_source_mask_rotation = param;
			} else if (strcmp(info.name, "positional_offset") == 0) {
				data->param_source_mask_positional_offset = param;
			} else if (strcmp(info.name, "mask_position") == 0) {
				data->param_source_mask_position = param;
			}  else if (strcmp(info.name, "invert") == 0) {
				data->param_source_mask_invert = param;
			} else if (strcmp(info.name, "channel_multipliers") == 0) {
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
	data->loading_effect = false;
}

static float get_position_width_factor(uint32_t pa)
{
	switch (pa) {
	case MASK_SOURCE_MASK_POSITION_TL:
	case MASK_SOURCE_MASK_POSITION_CL:
	case MASK_SOURCE_MASK_POSITION_BL:
		return 0.0f;
	case MASK_SOURCE_MASK_POSITION_TC:
	case MASK_SOURCE_MASK_POSITION_CC:
	case MASK_SOURCE_MASK_POSITION_BC:
		return 0.5f;
	case MASK_SOURCE_MASK_POSITION_TR:
	case MASK_SOURCE_MASK_POSITION_CR:
	case MASK_SOURCE_MASK_POSITION_BR:
		return 1.0f;
	}
	return 0.0f;
}

static float get_position_height_factor(uint32_t pa)
{
	switch (pa) {
	case MASK_SOURCE_MASK_POSITION_TL:
	case MASK_SOURCE_MASK_POSITION_TC:
	case MASK_SOURCE_MASK_POSITION_TR:
		return 0.0f;
	case MASK_SOURCE_MASK_POSITION_CL:
	case MASK_SOURCE_MASK_POSITION_CC:
	case MASK_SOURCE_MASK_POSITION_CR:
		return 0.5f;
	case MASK_SOURCE_MASK_POSITION_BL:
	case MASK_SOURCE_MASK_POSITION_BC:
	case MASK_SOURCE_MASK_POSITION_BR:
		return 1.0f;
	}
	return 0.0f;
}
