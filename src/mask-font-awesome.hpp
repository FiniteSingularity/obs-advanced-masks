#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

#include <obs-module.h>

#include <QDialog>
#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QString>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QListView>
#include <QPushButton>

#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include "base-filter.h"
#include "color-adjustments.h"


#define FA_SCALE_WIDTH 1
#define FA_SCALE_WIDTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Width"
#define FA_SCALE_HEIGHT 2
#define FA_SCALE_HEIGHT_LABEL "AdvancedMasks.SvgMask.ScaleBy.Height"
#define FA_SCALE_BOTH 3
#define FA_SCALE_BOTH_LABEL "AdvancedMasks.SvgMask.ScaleBy.Both"


// C Call wrapper functions
extern "C" void* mask_font_awesome_create(base_filter_data_t* base);
extern "C" void mask_font_awesome_destroy(void* data);

extern "C" void mask_font_awesome_update(void* data, base_filter_data_t* base,
	obs_data_t* settings);
extern "C" void mask_font_awesome_defaults(void* data,  obs_data_t* settings);
extern "C" void mask_font_awesome_properties(void* data, obs_properties_t* props);
extern "C" void mask_font_awesome_render(void* data, base_filter_data_t* base,
	color_adjustments_data_t* color_adj);

struct FAIconData {
	QString name;
	QString family;
	QString style;
	QString svg;
};

Q_DECLARE_METATYPE(FAIconData)

class FontAwesomeIcon {
public:
	FontAwesomeIcon(std::string id, std::string label);
	inline std::string getId() const { return _id; }
	inline std::string getLabel() const { return _label; }
	void addIcon(std::string family, std::string style, std::string svg);
	inline const std::map<std::string, std::map<std::string, std::string>>& getIcons() const { return _icons; }

private:
	std::string _id;
	std::string _label;
	std::map<std::string, std::map<std::string, std::string>> _icons;
};

class MaskFontAwesomeFilter;

class FontAwesomeApi {
public:
	static FontAwesomeApi* getInstance();

	// Cant make any copies of FontAwesomeApi.
	FontAwesomeApi(FontAwesomeApi& other) = delete;
	void operator=(const FontAwesomeApi&) = delete;

	inline std::string getApiToken() const { return _apiToken; }
	bool setApiToken(std::string token);

	nlohmann::json search(std::string searchString);

protected:
	FontAwesomeApi();

	static FontAwesomeApi* _instance;

private:
	bool _getAccessToken(std::string apiToken);

	std::string _accessToken;
	std::string _apiToken;
};

class MaskFontAwesomeFilter {
public:
	MaskFontAwesomeFilter(base_filter_data_t* base);
	~MaskFontAwesomeFilter();

	void update(base_filter_data_t* base, obs_data_t* settings);
	void defaults(obs_data_t* settings, uint32_t width, uint32_t height);
	void properties(obs_properties_t* props);
	void render(base_filter_data_t* base, color_adjustments_data_t* color_adj);

	static bool choose_button_clicked(obs_properties_t* props,
		obs_property_t* property, void* data);
private:
	void _loadEffectFiles();
	void _loadSvgEffect();
	void _renderSvgToTextures();

	std::string _svgData;
	std::string _faId;
	FontAwesomeApi *_api;
	base_filter_data_t* _base;
	bool _defaultsSet = false;

	int _maxTextureSize = 4096;

	gs_effect_t* _effect_svg_mask;
	gs_eparam_t* _param_image;
	gs_eparam_t* _param_svg_image;
	gs_eparam_t* _param_uv_size;
	gs_eparam_t* _param_svg_uv_size;
	gs_eparam_t* _param_offset;
	gs_eparam_t* _param_primary_alpha;
	gs_eparam_t* _param_secondary_alpha;


	uint32_t _scale_by;
	uint32_t _target_width;
	uint32_t _target_height;

	uint32_t _texture_width;
	uint32_t _texture_height;

	uint32_t _svg_render_width;
	uint32_t _svg_render_height;

	int _offset_x;
	int _offset_y;

	float _primary_alpha;
	float _secondary_alpha;

	std::string _svg;

	uint32_t _textureIndex;
	std::vector<gs_texture_t*> _textures;
	bool _textureRegen = false;
};

class FontAwesomePicker : public QDialog {
	Q_OBJECT
public:
	explicit FontAwesomePicker(QWidget* parent = nullptr);
	FAIconData getSelectedIcon() const { return _selectedIcon; }

private:
	FAIconData _selectedIcon;
};

class FontAwesomeSettingsTab : public QWidget {
	Q_OBJECT
public:
	explicit FontAwesomeSettingsTab(QWidget* parent = nullptr);
public:
	FontAwesomeApi* _api = nullptr;
	std::string _apiToken;
	std::string _accessToken;
};

class FontAwesomeIconModel : public QAbstractListModel {
	Q_OBJECT

public:
	explicit FontAwesomeIconModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void setIcons(const std::vector<FAIconData>& newIcons);
	const FAIconData& iconAt(int row) const;

private:
	std::vector<FAIconData> _icons;
};

class FontAwesomeIconDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	explicit FontAwesomeIconDelegate(QObject* parent = nullptr);

	void setItemSize(const QSize& size);

	using QStyledItemDelegate::QStyledItemDelegate;

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const override;

private:
	QSize _itemSize;
};


class FontAwesomeIconGridWidget : public QWidget {
	Q_OBJECT
public:
	explicit FontAwesomeIconGridWidget(QWidget* parent = nullptr);

	void setIcons(const std::vector<FontAwesomeIcon>& icons);

signals:
	void iconSelectionChanged(std::optional<FAIconData> icon);

protected:
	void resizeEvent(QResizeEvent* event) override;
private:
	QListView* _view;
	FontAwesomeIconModel* _model;
	void _updateGridSize();
};


class FontAwesomeSearchTab : public QWidget {
	Q_OBJECT
public:
	explicit FontAwesomeSearchTab(QWidget* parent = nullptr);
signals:
	void iconSelected(FAIconData icon);
	void selectButtonClicked();
	void cancelButtonClicked();

private:
	QPushButton* _selectButton;
	FontAwesomeApi* _api;
	FontAwesomeIconGridWidget* _searchResults;
	std::vector<FontAwesomeIcon> _icons;
	FAIconData _selectedIcon;
};

std::string get_api_token();
void save_api_token(std::string token);

obs_data_t* load_module_config();
void save_module_config(obs_data_t* config);

QString preprocessSvg(const QString& rawSvg, const QString& primary, const QString& secondary);

std::string encryptString(std::string input);
std::string decryptString(std::string input);

std::string fetch_string_from_post(
	std::string url,
	std::string postdata,
	std::string token);

template<class T>
static size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	T& result = *static_cast<T*>(userdata);

	size_t end = result.size();
	result.resize(result.size() + size * nmemb);
	memcpy(&result[end], ptr, size * nmemb);

	return size * nmemb;
};
