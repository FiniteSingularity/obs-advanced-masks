#pragma once

#include <obs-module.h>

#define PLUGIN_INFO                                                                                                 \
	"<a href=\"https://github.com/finitesingularity/obs-advanced-masks/\">Advanced Masks</a> (" PROJECT_VERSION \
	") by <a href=\"https://twitch.tv/finitesingularity\">FiniteSingularity</a>"

#define MASK_EFFECT_ALPHA 1
#define MASK_EFFECT_ALPHA_LABEL "AdvancedMasks.Effects.Alpha"
#define MASK_EFFECT_ADJUSTMENT 2
#define MASK_EFFECT_ADJUSTMENT_LABEL "AdvancedMasks.Effects.Adjustment"

#define MASK_TYPE_SHAPE 1
#define MASK_TYPE_SHAPE_LABEL "AdvancedMasks.Shape"
#define MASK_TYPE_SOURCE 2
#define MASK_TYPE_SOURCE_LABEL "AdvancedMasks.Source"
#define MASK_TYPE_IMAGE 3
#define MASK_TYPE_IMAGE_LABEL "AdvancedMasks.Image"
#define MASK_TYPE_GRADIENT 4
#define MASK_TYPE_GRADIENT_LABEL "AdvancedMasks.Gradient"
#define MASK_TYPE_BSM 5
#define MASK_TYPE_BSM_LABEL "AdvancedMasks.BSM"
#define MASK_TYPE_CHROMA_KEY 6
#define MASK_TYPE_CHROMA_KEY_LABEL "AdvancedMasks.ChromaKey"
#define MASK_TYPE_FEATHER 7
#define MASK_TYPE_FEATHER_LABEL "AdvancedMasks.FeatherMask"
#define MASK_TYPE_SVG 8
#define MASK_TYPE_SVG_LABEL "AdvancedMasks.SvgMask"

struct base_filter_data;
typedef struct base_filter_data base_filter_data_t;

struct base_filter_data {
	obs_source_t* context;

	bool input_texture_generated;
	gs_texrender_t *input_texrender;
	bool output_rendered;
	gs_texrender_t *output_texrender;
	gs_effect_t *output_effect;
	gs_eparam_t *param_output_image;

	bool rendered;
	bool rendering;

	uint32_t width;
	uint32_t height;

	uint32_t mask_effect;
	uint32_t mask_type;
};
