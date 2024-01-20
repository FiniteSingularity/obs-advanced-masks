#pragma once

#include <obs-module.h>
#include "base-filter.h"
#include "color-adjustments.h"
#include "mask-source.h"
#include "mask-shape.h"
#include "mask-gradient.h"
#include "mask-bsm.h"

struct advanced_masks_data;
typedef struct advanced_masks_data advanced_masks_data_t;

struct advanced_masks_data {
	obs_source_t *context;

	base_filter_data_t *base;
	color_adjustments_data_t *color_adj_data;
	mask_source_data_t *source_data;
	mask_shape_data_t *shape_data;
	mask_gradient_data_t *gradient_data;
	mask_bsm_data_t *bsm_data;

	bool invert;
};
