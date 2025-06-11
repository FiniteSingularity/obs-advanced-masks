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

gs_texture_t* gs_texture_from_svg_path(const char* path, int width, int height, int scale_by)
{
	//std::string svgPath(path);
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
	obs_enter_graphics();
	gs_texture_t* tex = gs_texture_create(cx, cy, format, 1, (const uint8_t**)&ptr, 0);
	obs_leave_graphics();
	return tex;
}

gs_texture_t* gs_texture_from_svg(const char* svg, int width, int height, int scale_by)
{
	//std::string svgPath(path);
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
	obs_enter_graphics();
	gs_texture_t* tex = gs_texture_create(cx, cy, format, 1, (const uint8_t**)&ptr, 0);
	obs_leave_graphics();
	return tex;
}
