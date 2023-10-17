#include <obs-module.h>

#include "version.h"

extern struct obs_source_info advanced_masks_filter;

OBS_DECLARE_MODULE();

OBS_MODULE_USE_DEFAULT_LOCALE("obs-advanced-masks", "en-US");

OBS_MODULE_AUTHOR("FiniteSingularity");

bool obs_module_load(void)
{
	blog(LOG_INFO, "[Advanced Masks] loaded version %s", PROJECT_VERSION);
	obs_register_source(&advanced_masks_filter);
	return true;
}

void obs_module_unload(void) {}
