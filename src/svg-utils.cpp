#include "svg-utils.hpp"
extern "C" {
#include "mask-svg.h"
#include "utils.h"
}

#include <string>

#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QBuffer>
#include <QByteArray>
#include <QtConcurrent/QtConcurrent>

// update type- 1: smaller, 2: larger, 3: all
void update_svg_textures(
	const char* path,
	int width,
	int height,
	int scale_by,
	int update_type,
	gs_texture_t** smallTex,
	gs_texture_t** currentTex,
	gs_texture_t** largeTex
)
{
	uint32_t large_width = next_power_of_2(width + 1);
	uint32_t large_height = next_power_of_2(height + 1);
	uint32_t small_width = previous_power_of_2(width - 1);
	uint32_t small_height = previous_power_of_2(height - 1);
	gs_texture_t* to_delete;
	blog(LOG_INFO, "Updating Texture...");
	QFuture<void> future;
	switch (update_type) {
	case SVG_GENERATE_NEW_SMALL:
		to_delete = *largeTex;
		// *large resolves to the original pointer

		// dereferenced large gets assigned to dereferenced current.
		// i.e.- the pointer memory address that was at current
		//       is now assigned to large.
		*largeTex = *currentTex;
		*currentTex = *smallTex;

		// Todo: pop this in its own thread.
		future = QtConcurrent::run(
			[path, smallTex, small_width, small_height, scale_by, to_delete]() {
				*smallTex = gs_texture_from_svg_path(path, small_width, small_height, scale_by);
				obs_enter_graphics();
				gs_texture_destroy(to_delete);
				obs_leave_graphics();
		});

		break;
	case SVG_GENERATE_NEW_LARGE:
		to_delete = *smallTex;
		*smallTex = *currentTex;
		*currentTex = *largeTex;

		// TODO: Thead the following

		future = QtConcurrent::run(
			[path, largeTex, large_width, large_height, scale_by, to_delete]() {
				*largeTex = gs_texture_from_svg_path(path, large_width, large_height, scale_by);
				obs_enter_graphics();
				gs_texture_destroy(to_delete);
				obs_leave_graphics();
			});
		break;
	case SVG_GENERATE_ALL:
		*smallTex = gs_texture_from_svg_path(path, small_width, small_height, scale_by);
		*currentTex = gs_texture_from_svg_path(path, width, height, scale_by);
		*largeTex = gs_texture_from_svg_path(path, large_width, large_height, scale_by);
		break;
	}
}

gs_texture_t* gs_texture_from_svg_path(const char* path, int width, int height, int scale_by)
{
	//std::string svgPath(path);
	blog(LOG_INFO, "gs_texture_from_svg...");
	int large = 1000000;
	QSvgRenderer svgRenderer(QString(path), nullptr);
	if (!svgRenderer.isValid()) {
		return nullptr;
	}
	QSize defSize = svgRenderer.defaultSize();

	QSize imageSize;
	if (scale_by == 1) {
		imageSize = defSize.scaled(QSize(width, large), Qt::KeepAspectRatio);
	} else if (scale_by == 2) {
		imageSize = defSize.scaled(QSize(large, height), Qt::KeepAspectRatio);
	} else {
		imageSize.setWidth(width);
		imageSize.setHeight(height);
	}

	QImage image(imageSize, QImage::Format_RGBA8888);
	image.fill(Qt::transparent);

	QPainter painter(&image);
	svgRenderer.render(&painter);
	painter.end();

	int bytesPerLine = image.bytesPerLine();
	uint32_t cy = image.height();
	uint32_t cx = image.width();

	const uint8_t* ptr = image.constBits();

	std::vector<uint8_t> rawData(ptr, ptr + (bytesPerLine * cy));

	enum gs_color_format format = GS_RGBA;
	blog(LOG_INFO, "Generating texture...");
	obs_enter_graphics();
	gs_texture_t* tex = gs_texture_create(cx, cy, format, 1, (const uint8_t**)&ptr, 0);
	obs_leave_graphics();
	blog(LOG_INFO, "COMPLETE!");
	return tex;
}

gs_texture_t* gs_texture_from_svg(const char* svg, int width, int height, int scale_by)
{
	//std::string svgPath(path);
	blog(LOG_INFO, "gs_texture_from_svg...");
	int large = 1000000;
	QSvgRenderer svgRenderer(QString(svg).toUtf8());
	if (!svgRenderer.isValid()) {
		return nullptr;
	}
	QSize defSize = svgRenderer.defaultSize();

	QSize imageSize;
	if (scale_by == 1) {
		imageSize = defSize.scaled(QSize(width, large), Qt::KeepAspectRatio);
	}
	else if (scale_by == 2) {
		imageSize = defSize.scaled(QSize(large, height), Qt::KeepAspectRatio);
	}
	else {
		imageSize.setWidth(width);
		imageSize.setHeight(height);
	}

	QImage image(imageSize, QImage::Format_RGBA8888);
	image.fill(Qt::transparent);

	QPainter painter(&image);
	svgRenderer.render(&painter);
	painter.end();

	int bytesPerLine = image.bytesPerLine();
	uint32_t cy = image.height();
	uint32_t cx = image.width();

	const uint8_t* ptr = image.constBits();

	std::vector<uint8_t> rawData(ptr, ptr + (bytesPerLine * cy));

	enum gs_color_format format = GS_RGBA;
	blog(LOG_INFO, "Generating texture...");
	obs_enter_graphics();
	gs_texture_t* tex = gs_texture_create(cx, cy, format, 1, (const uint8_t**)&ptr, 0);
	obs_leave_graphics();
	blog(LOG_INFO, "COMPLETE!");
	return tex;
}
