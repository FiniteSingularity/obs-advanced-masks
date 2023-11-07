#include "color-adjustments.h"
#include "obs-utils.h"

void color_adjustments_update(color_adjustments_data_t *data, obs_data_t *settings)
{
	data->adj_brightness = obs_data_get_bool(settings, "brightness");
	data->min_brightness =
		(float)obs_data_get_double(settings, "min_brightness_value");
	data->max_brightness =
		(float)obs_data_get_double(settings, "max_brightness_value");

	data->adj_contrast = obs_data_get_bool(settings, "contrast");
	data->min_contrast =
		(float)obs_data_get_double(settings, "min_contrast_value");
	data->max_contrast =
		(float)obs_data_get_double(settings, "max_contrast_value");

	data->adj_saturation = obs_data_get_bool(settings, "saturation");
	data->min_saturation =
		(float)obs_data_get_double(settings, "min_saturation_value");
	data->max_saturation =
		(float)obs_data_get_double(settings, "max_saturation_value");

	data->adj_hue_shift = obs_data_get_bool(settings, "hue_shift");
	data->min_hue_shift =
		(float)obs_data_get_double(settings, "min_hue_shift_value");
	data->max_hue_shift =
		(float)obs_data_get_double(settings, "max_hue_shift_value");
}

void color_adjustments_properties(obs_properties_t *props)
{
	obs_properties_t *mask_adjustments_group = obs_properties_create();
	obs_property_t *p;

	p = obs_properties_add_bool(
		mask_adjustments_group, "brightness",
		obs_module_text("AdvancedMasks.Adjustments.Brightness"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_brightness_value",
		obs_module_text("AdvancedMasks.Adjustments.MinBrightness"),
		-1.0, 1.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_brightness_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxBrightness"),
		-1.0, 1.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "contrast",
		obs_module_text("AdvancedMasks.Adjustments.Contrast"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_contrast_value",
		obs_module_text("AdvancedMasks.Adjustments.MinContrast"), -4.0,
		4.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_contrast_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxContrast"), -4.0,
		4.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "saturation",
		obs_module_text("AdvancedMasks.Adjustments.Saturation"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_saturation_value",
		obs_module_text("AdvancedMasks.Adjustments.MinSaturation"), 0.0,
		5.0, 0.01);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_saturation_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxSaturation"), 0.0,
		5.0, 0.01);

	p = obs_properties_add_bool(
		mask_adjustments_group, "hue_shift",
		obs_module_text("AdvancedMasks.Adjustments.HueShift"));

	obs_property_set_modified_callback(p, setting_mask_adjustment_modified);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "min_hue_shift_value",
		obs_module_text("AdvancedMasks.Adjustments.MinHueShift"),
		-360.0, 360.0, 0.1);

	p = obs_properties_add_float_slider(
		mask_adjustments_group, "max_hue_shift_value",
		obs_module_text("AdvancedMasks.Adjustments.MaxHueShift"),
		-360.0, 360.0, 0.1);

	obs_properties_add_group(
		props, "mask_adjustments_group",
		obs_module_text("AdvancedMasks.Adjustments.Label"),
		OBS_GROUP_NORMAL, mask_adjustments_group);
}

static bool setting_mask_adjustment_modified(obs_properties_t *props,
					     obs_property_t *p,
					     obs_data_t *settings)
{
	UNUSED_PARAMETER(p);
	bool brightness = obs_data_get_bool(settings, "brightness");
	bool contrast = obs_data_get_bool(settings, "contrast");
	bool saturation = obs_data_get_bool(settings, "saturation");
	bool hue_shift = obs_data_get_bool(settings, "hue_shift");
	setting_visibility("min_brightness_value", brightness, props);
	setting_visibility("max_brightness_value", brightness, props);
	setting_visibility("min_contrast_value", contrast, props);
	setting_visibility("max_contrast_value", contrast, props);
	setting_visibility("min_saturation_value", saturation, props);
	setting_visibility("max_saturation_value", saturation, props);
	setting_visibility("min_hue_shift_value", hue_shift, props);
	setting_visibility("max_hue_shift_value", hue_shift, props);
	return true;
}


void color_adjustments_defaults(obs_data_t* settings) {
	obs_data_set_default_bool(settings, "brightness", false);
	obs_data_set_default_double(settings, "min_brightness_value", -1.0);
	obs_data_set_default_double(settings, "max_brightness_value", 1.0);
	obs_data_set_default_bool(settings, "contrast", false);
	obs_data_set_default_double(settings, "min_contrast_value", 0.0);
	obs_data_set_default_double(settings, "max_contrast_value", 1.0);
	obs_data_set_default_bool(settings, "saturation", false);
	obs_data_set_default_double(settings, "min_saturation_value", 0.0);
	obs_data_set_default_double(settings, "max_saturation_value", 4.0);
	obs_data_set_default_bool(settings, "hue_shift", false);
	obs_data_set_default_double(settings, "min_hue_shift_value", -180.0);
	obs_data_set_default_double(settings, "max_hue_shift_value", 180.0);
}
