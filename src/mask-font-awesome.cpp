#include "mask-font-awesome.hpp"
#include "svg-utils.hpp"

extern "C" {
#include "obs-utils.h"
#include "utils.h"
}


#include <algorithm>

#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <util/platform.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QPainter>
#include <QSvgRenderer>
#include <QDomDocument>
#include <QDomElement>


#pragma comment(lib, "crypt32.lib")
#include <Windows.h>
#include <Wincrypt.h>
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

FontAwesomeApi* FontAwesomeApi::_instance = nullptr;

void* mask_font_awesome_create(base_filter_data_t* base)
{
	return new MaskFontAwesomeFilter(base);
}

void mask_font_awesome_destroy(void* data)
{
	MaskFontAwesomeFilter* obj = static_cast<MaskFontAwesomeFilter*>(data);
	delete obj;
}

void mask_font_awesome_update(void* data, base_filter_data_t* base,
	obs_data_t* settings)
{
	MaskFontAwesomeFilter& obj = *static_cast<MaskFontAwesomeFilter*>(data);
	obj.update(base, settings);
}

void mask_font_awesome_defaults(void* data, obs_data_t* settings)
{

}

void mask_font_awesome_properties(void* data, obs_properties_t* props)
{
	MaskFontAwesomeFilter& obj = *static_cast<MaskFontAwesomeFilter*>(data);
	obj.properties(props);
}

void mask_font_awesome_render(void* data, base_filter_data_t* base,
	color_adjustments_data_t* color_adj)
{
	MaskFontAwesomeFilter& obj = *static_cast<MaskFontAwesomeFilter*>(data);
	obj.render(base, color_adj);
}

FontAwesomeIcon::FontAwesomeIcon(std::string id, std::string label)
  : _id(id), _label(label)
{
}

void FontAwesomeIcon::addIcon(std::string family, std::string style, std::string svg)
{
	_icons[family][style] = svg;
}

FontAwesomeApi::FontAwesomeApi()
{
	// Do stuff here to get API key and access token.
	_apiToken = get_api_token();
	if (_apiToken == "") {
		return;
	}
	_getAccessToken(_apiToken);
}

FontAwesomeApi* FontAwesomeApi::getInstance()
{
	if (FontAwesomeApi::_instance == nullptr) {
		FontAwesomeApi::_instance = new FontAwesomeApi();
	}
	return _instance;
}

nlohmann::json FontAwesomeApi::search(std::string searchString)
{

	std::string query = "query{ search(version: \"6.x\", query: \"" + searchString + "\", first: 50) { id label svgs { html familyStyle {family style } } } }";
	nlohmann::json postJson;
	postJson["query"] = query;
	auto q = postJson.dump();
	std::string result = fetch_string_from_post("https://api.fontawesome.com/", postJson.dump(), _accessToken);
	if (result == "unauthorized") {
		return nlohmann::json::parse("{\"error\": \"unauthorized\"}");
	}
	return nlohmann::json::parse(result);
}

bool FontAwesomeApi::setApiToken(std::string token)
{
	bool success = _getAccessToken(token);

	if (!success) {
		return false;
	}
	_apiToken = token;
	save_api_token(_apiToken);
	return true;
}

bool FontAwesomeApi::_getAccessToken(std::string apiToken)
{
	std::string result = fetch_string_from_post("https://api.fontawesome.com/token", "", apiToken);
	if (result == "unauthorized") {
		return false;
	}
	// TODO- Error checking if we get bad json response.
	auto json = nlohmann::json::parse(result);
	_accessToken = json["access_token"];
	return true;
}

MaskFontAwesomeFilter::MaskFontAwesomeFilter(base_filter_data_t* base)
  :_api(FontAwesomeApi::getInstance()), _base(base)
{
	_effect_svg_mask = nullptr;
	_loadEffectFiles();
}

MaskFontAwesomeFilter::~MaskFontAwesomeFilter()
{
	obs_enter_graphics();
	if (_effect_svg_mask) {
		gs_effect_destroy(_effect_svg_mask);
	}

	for (auto& texture : _textures)
	{
		gs_texture_destroy(texture);
	}
	obs_leave_graphics();
}

void MaskFontAwesomeFilter::update(base_filter_data_t* base, obs_data_t* settings)
{
	uint32_t w = obs_source_get_width(base->context);
	uint32_t h = obs_source_get_height(base->context);

	defaults(settings, w, h);

	uint32_t width = (uint32_t)obs_data_get_int(settings, "mask_font_awesome_width");
	uint32_t height = (uint32_t)obs_data_get_int(settings, "mask_font_awesome_height");
	uint32_t scaleBy = (uint32_t)obs_data_get_int(settings, "mask_font_awesome_scale_by");
	std::string svg = obs_data_get_string(settings, "mask_font_awesome_svg");


	bool iconChanged = svg != _svg;
	bool scaleByChanged = scaleBy != _scale_by;

	bool regen_bitmap = iconChanged;

	_svg = svg;

	_target_width = width;
	_target_height = height;
	_scale_by = scaleBy;

	_offset_x = (int)obs_data_get_int(settings, "mask_font_awesome_pos_x");
	_offset_y = (int)obs_data_get_int(settings, "mask_font_awesome_pos_y");


	_primary_alpha = (float)obs_data_get_double(settings, "mask_font_awesome_primary");
	_secondary_alpha = (float)obs_data_get_double(settings, "mask_font_awesome_secondary");

	if ((regen_bitmap || scaleByChanged) && !_svg.empty()) {
		_renderSvgToTextures();
	}
	uint32_t npt;
	switch (_scale_by) {
	case FA_SCALE_WIDTH:
		_svg_render_width = _target_width;
		_svg_render_height = (uint32_t)((double)_target_width * (double)_texture_height / (double)_texture_width);
		npt = next_power_of_2(_target_width);
		_textureIndex = min((uint32_t)log2(npt) - 3, static_cast<uint32_t>(_textures.size()) - 1);
		break;
	case FA_SCALE_HEIGHT:
		_svg_render_height = _target_height;
		_svg_render_width = (uint32_t)((double)_target_height * (double)_texture_width / (double)_texture_height);
		npt = next_power_of_2(_target_height);
		_textureIndex = min((uint32_t)log2(npt) - 3, static_cast<uint32_t>(_textures.size()) - 1);
		break;
	case FA_SCALE_BOTH:
		_svg_render_width = _target_width;
		_svg_render_height = _target_height;
		npt = next_power_of_2(_target_width);
		_textureIndex = min((uint32_t)log2(npt) - 3, static_cast<uint32_t>(_textures.size()) - 1);
		break;
	}
}

void MaskFontAwesomeFilter::defaults(obs_data_t* settings, uint32_t width, uint32_t height)
{
	obs_data_set_default_int(settings, "mask_font_awesome_width", 256);
	obs_data_set_default_int(settings, "mask_font_awesome_height", 256);
	obs_data_set_default_int(settings, "mask_font_awesome_pos_x", width / 2);
	obs_data_set_default_int(settings, "mask_font_awesome_pos_y", height / 2);
	obs_data_set_default_double(settings, "mask_font_awesome_primary", 1.0);
	obs_data_set_default_double(settings, "mask_font_awesome_secondary", 0.7);
	_defaultsSet = true;
}

bool MaskFontAwesomeFilter::choose_button_clicked(obs_properties_t* props,
	obs_property_t* property, void* data)
{
	auto obj = static_cast<MaskFontAwesomeFilter*>(data);
	const auto mainWindow = static_cast<QMainWindow*>(
		obs_frontend_get_main_window());
	const QRect& hostRect = mainWindow->geometry();

	auto dialog = new FontAwesomePicker(mainWindow);
	auto resp = dialog->exec();
	if (resp == QDialog::Accepted) {
		auto selectedIcon = dialog->getSelectedIcon();

		obs_data_t* settings = obs_source_get_settings(obj->_base->context);
		obs_data_set_string(settings, "mask_font_awesome_name", selectedIcon.name.toStdString().c_str());
		obs_data_set_string(settings, "mask_font_awesome_family", selectedIcon.family.toStdString().c_str());
		obs_data_set_string(settings, "mask_font_awesome_style", selectedIcon.style.toStdString().c_str());
		obs_data_set_string(settings, "mask_font_awesome_svg", selectedIcon.svg.toStdString().c_str());

		obj->update(obj->_base, settings);

		obs_data_release(settings);
		obj->_renderSvgToTextures();

		return true;
	}
	return false;
}

void MaskFontAwesomeFilter::properties(obs_properties_t* props)
{
	auto mask_font_awesome_group = obs_properties_create();
	auto mask_font_awesome_selected_group = obs_properties_create();
	auto mask_font_awesome_mask_props_group = obs_properties_create();

	auto p = obs_properties_add_button2(
		mask_font_awesome_group, "mask_font_awesome_choose",
		obs_module_text("AdvancedMasks.FontAwesome.Choose"),
		MaskFontAwesomeFilter::choose_button_clicked,
		this);

	obs_properties_add_text(mask_font_awesome_selected_group, "mask_font_awesome_name", "    Name:", OBS_TEXT_INFO);
	obs_properties_add_text(mask_font_awesome_selected_group, "mask_font_awesome_family", "    Family:", OBS_TEXT_INFO);
	obs_properties_add_text(mask_font_awesome_selected_group, "mask_font_awesome_style", "    Style:", OBS_TEXT_INFO);
	p = obs_properties_add_text(mask_font_awesome_selected_group, "mask_font_awesome_svg", "    Svg:", OBS_TEXT_INFO);
	obs_property_set_visible(p, false);

	obs_properties_add_float_slider(mask_font_awesome_selected_group, "mask_font_awesome_primary", "    Primary Opacity:", 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(mask_font_awesome_selected_group, "mask_font_awesome_secondary", "    Secondary Opacity:", 0.0, 1.0, 0.01);

	obs_properties_add_group(
		mask_font_awesome_group, "mask_font_awesome_selected_group",
		obs_module_text("AdvancedMasks.FontAwesome.SelectedIcon"),
		OBS_GROUP_NORMAL, mask_font_awesome_selected_group
	);

	obs_properties_add_group(
		props, "mask_font_awesome_group",
		obs_module_text("AdvancedMasks.FontAwesome.Label"),
		OBS_GROUP_NORMAL, mask_font_awesome_group);



	obs_property_t* scale_by = obs_properties_add_list(
		mask_font_awesome_mask_props_group, "mask_font_awesome_scale_by",
		obs_module_text("AdvancedMasks.SvgMask.ScaleBy"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(FA_SCALE_WIDTH_LABEL),
		FA_SCALE_WIDTH);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(FA_SCALE_HEIGHT_LABEL),
		FA_SCALE_HEIGHT);

	obs_property_list_add_int(
		scale_by,
		obs_module_text(FA_SCALE_BOTH_LABEL),
		FA_SCALE_BOTH);

	p = obs_properties_add_int_slider(
		mask_font_awesome_mask_props_group, "mask_font_awesome_width",
		obs_module_text("AdvancedMasks.SvgMask.Width"), 0, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_font_awesome_mask_props_group, "mask_font_awesome_height",
		obs_module_text("AdvancedMasks.SvgMask.Height"), 0, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_font_awesome_mask_props_group, "mask_font_awesome_pos_x",
		obs_module_text("AdvancedMasks.SvgMask.PosX"), -4000, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	p = obs_properties_add_int_slider(
		mask_font_awesome_mask_props_group, "mask_font_awesome_pos_y",
		obs_module_text("AdvancedMasks.SvgMask.PosY"), -4000, 4000,
		1);
	obs_property_float_set_suffix(p, "px");

	obs_properties_add_group(
		props, "mask_font_awesome_mask_props_group",
		obs_module_text("AdvancedMasks.FontAwesome.Properties"),
		OBS_GROUP_NORMAL, mask_font_awesome_mask_props_group);


}

void MaskFontAwesomeFilter::render(base_filter_data_t* base, color_adjustments_data_t* color_adj)
{
	if (_textures.size() == 0 || _textureRegen) {
		return;
	}

	gs_effect_t* effect = _effect_svg_mask;
	gs_texture_t* texture = gs_texrender_get_texture(base->input_texrender);

	gs_texture_t* svg_texture = _textures[_textureIndex];
	if (!effect || !texture || !svg_texture) {
		return;
	}

	base->output_texrender =
		create_or_reset_texrender(base->output_texrender);

	if (_param_image) {
		gs_effect_set_texture(_param_image, texture);
	}

	if (_param_svg_image) {
		gs_effect_set_texture(_param_svg_image, svg_texture);
	}
	if (_param_uv_size) {
		struct vec2 uv_size;
		uv_size.x = (float)base->width;
		uv_size.y = (float)base->height;
		gs_effect_set_vec2(_param_uv_size, &uv_size);
	}
	if (_param_svg_uv_size) {
		struct vec2 svg_uv_size;
		svg_uv_size.x = (float)_svg_render_width;
		svg_uv_size.y = (float)_svg_render_height;
		gs_effect_set_vec2(_param_svg_uv_size, &svg_uv_size);
	}
	if (_param_offset) {
		struct vec2 offset;
		offset.x = (float)_offset_x;
		offset.y = (float)_offset_y;
		gs_effect_set_vec2(_param_offset, &offset);
	}
	if (_param_primary_alpha) {
		gs_effect_set_float(_param_primary_alpha, _primary_alpha);
	}
	if (_param_secondary_alpha) {
		gs_effect_set_float(_param_secondary_alpha, _secondary_alpha);
	}


	set_render_parameters();
	set_blending_parameters();
	const char* technique = "DrawFA";

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

void MaskFontAwesomeFilter::_loadEffectFiles()
{
	_loadSvgEffect();
}

void MaskFontAwesomeFilter::_loadSvgEffect()
{
	const char* effect_file_path = "/shaders/svg-mask.effect";

	_effect_svg_mask = load_shader_effect(
		_effect_svg_mask, effect_file_path);
	if (_effect_svg_mask) {
		size_t effect_count =
			gs_effect_get_num_params(_effect_svg_mask);
		for (size_t effect_index = 0; effect_index < effect_count;
			effect_index++) {
			gs_eparam_t* param = gs_effect_get_param_by_idx(
				_effect_svg_mask, effect_index);
			struct gs_effect_param_info info;
			gs_effect_get_param_info(param, &info);
			if (strcmp(info.name, "image") == 0) {
				_param_image = param;
			}
			else if (strcmp(info.name, "svg_image") == 0) {
				_param_svg_image = param;
			}
			else if (strcmp(info.name, "uv_size") == 0) {
				_param_uv_size = param;
			}
			else if (strcmp(info.name, "svg_uv_size") == 0) {
				_param_svg_uv_size = param;
			}
			else if (strcmp(info.name, "offset") == 0) {
				_param_offset = param;
			}
			else if (strcmp(info.name, "primary_alpha") == 0) {
				_param_primary_alpha = param;
			}
			else if (strcmp(info.name, "secondary_alpha") == 0) {
				_param_secondary_alpha = param;
			}
		}
	}
}


void MaskFontAwesomeFilter::_renderSvgToTextures()
{
	_textureRegen = true;
	for (auto &texture : _textures)
	{
		gs_texture_destroy(texture);
	}
	_textures.clear();
	QString primary = "#FFFFFF";
	QString secondary = "#000000";
	QString processedSvg = preprocessSvg(_svg.c_str(), primary, secondary);
	for (int i = 8; i <= _maxTextureSize; i *= 2) {
		gs_texture_t* tex = gs_texture_from_svg(
			processedSvg.toStdString().c_str(),
			i,
			i,
			_scale_by
		);
		_textures.push_back(tex);
		obs_enter_graphics();
		_texture_width = gs_texture_get_width(tex);
		_texture_height = gs_texture_get_height(tex);
		obs_leave_graphics();
	}
	_textureRegen = false;
}

FontAwesomePicker::FontAwesomePicker(QWidget* parent)
  : QDialog(parent)
{
	auto layout = new QVBoxLayout(this);
	QTabWidget* tabWidget = new QTabWidget(this);

	// Create instances of your existing widgets
	auto searchWidget = new FontAwesomeSearchTab;
	auto settingsWidget = new FontAwesomeSettingsTab;

	connect(searchWidget, &FontAwesomeSearchTab::selectButtonClicked, this, &QDialog::accept);
	connect(searchWidget, &FontAwesomeSearchTab::cancelButtonClicked, this, &QDialog::reject);
	connect(searchWidget, &FontAwesomeSearchTab::iconSelected, [&](FAIconData icon) {
		_selectedIcon = icon;
	});
	// Add the widgets to the tab widget
	tabWidget->addTab(searchWidget, "Icon Search");
	tabWidget->addTab(settingsWidget, "Account Settings");
	tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(tabWidget);
	resize(800, 600);
}

FontAwesomeSettingsTab::FontAwesomeSettingsTab(QWidget* parent)
  : QWidget(parent), _api(FontAwesomeApi::getInstance())
{
	_apiToken = _api->getApiToken();
	auto layout = new QVBoxLayout(this);
	auto header = new QLabel(obs_module_text("AdvancedMasks.FontAwesome.SettingsHeader"), this);
	auto subText = new QLabel(obs_module_text("AdvancedMasks.FontAwesome.SettingsSubText"), this);

	auto tokenLabel = new QLabel(obs_module_text("AdvancedMasks.FontAwesome.ApiToken"), this);
	auto tokenField = new QLineEdit(this);
	tokenField->setEchoMode(QLineEdit::Password);
	tokenField->setText(_apiToken.c_str());

	auto applyButton = new QPushButton(obs_module_text("AdvancedMasks.FontAwesome.Apply"), this);

	connect(applyButton, &QPushButton::clicked, [this, tokenField]() {
		_api->setApiToken(tokenField->text().toStdString());
	});

	auto tokenFieldLayout = new QHBoxLayout();
	tokenFieldLayout->addWidget(tokenField);
	tokenFieldLayout->addWidget(applyButton);

	layout->addWidget(header);
	layout->addWidget(subText);
	layout->addWidget(tokenLabel);
	layout->addLayout(tokenFieldLayout);
	layout->addStretch();
}

FontAwesomeSearchTab::FontAwesomeSearchTab(QWidget* parent)
 : QWidget(parent), _api(FontAwesomeApi::getInstance())
{
	auto layout = new QVBoxLayout(this);
	auto header = new QLabel(obs_module_text("AdvancedMasks.FontAwesome.IconSearch"), this);

	_searchResults = new FontAwesomeIconGridWidget(this);
	_searchResults->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	connect(_searchResults, &FontAwesomeIconGridWidget::iconSelectionChanged, [this](std::optional<FAIconData> icon) {
		if (icon == std::nullopt) {
			_selectButton->setDisabled(true);
			return;
		}
		_selectedIcon = icon.value();
		_selectButton->setDisabled(false);
		emit iconSelected(_selectedIcon);
	});

	auto searchField = new QLineEdit(this);

	auto searchButton = new QPushButton(obs_module_text("AdvancedMasks.FontAwesome.Search"), this);

	connect(searchButton, &QPushButton::clicked, [this, searchField]() {
		auto result = _api->search(searchField->text().toStdString());
		
		if (result.contains("data") && result["data"].contains("search")) {
			_icons.clear();
			for (auto it : result["data"]["search"]) {
				if (it["svgs"].size() == 0)
					continue;
				FontAwesomeIcon icon(it["id"], it["label"]);
				
				for (auto svg : it["svgs"]) {
					icon.addIcon(svg["familyStyle"]["family"], svg["familyStyle"]["style"], svg["html"]);
				}
				_icons.push_back(icon);
			}
			_searchResults->setIcons(_icons);
		}
	});

	auto searchFieldLayout = new QHBoxLayout();
	searchFieldLayout->addWidget(searchField);
	searchFieldLayout->addWidget(searchButton);

	auto buttons = new QHBoxLayout();
	auto cancelButton = new QPushButton("Cancel");
	_selectButton = new QPushButton("Select Icon");
	_selectButton->setDisabled(true);

	connect(_selectButton, &QPushButton::clicked, [this]() {
		emit selectButtonClicked();
	});

	connect(cancelButton, & QPushButton::clicked, [this]() {
		emit cancelButtonClicked();
	});

	buttons->addStretch();
	buttons->addWidget(cancelButton);
	buttons->addWidget(_selectButton);


	layout->addWidget(header);
	layout->addLayout(searchFieldLayout);
	layout->addWidget(_searchResults);
	layout->addLayout(buttons);


}

FontAwesomeIconModel::FontAwesomeIconModel(QObject* parent)
	: QAbstractListModel(parent) {
}

int FontAwesomeIconModel::rowCount(const QModelIndex&) const {
	return static_cast<int>(_icons.size());
}

QVariant FontAwesomeIconModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid() || index.row() >= _icons.size())
		return {};

	if (role == Qt::UserRole)
		return QVariant::fromValue(_icons[index.row()]);

	return {};
}

void FontAwesomeIconModel::setIcons(const std::vector<FAIconData>& newIcons) {
	beginResetModel();
	_icons = newIcons;
	endResetModel();
}

const FAIconData& FontAwesomeIconModel::iconAt(int row) const {
	return _icons[row];
}

FontAwesomeIconDelegate::FontAwesomeIconDelegate(QObject* parent)
	: QStyledItemDelegate(parent), _itemSize(120, 140) {
}

void FontAwesomeIconDelegate::setItemSize(const QSize& size) {
	_itemSize = size;
}

void FontAwesomeIconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const {
	painter->save();

	const FAIconData icon = index.data(Qt::UserRole).value<FAIconData>();

	// Draw background if selected
	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
	}

	QRect rect = option.rect.adjusted(5, 5, -5, -5);

	// Define SVG and text layout areas
	const int iconAreaHeight = rect.height() * 0.6;
	QRect svgRect(rect.left(), rect.top(), rect.width(), iconAreaHeight);

	// Preprocess SVG to override fill rules
	QString primary = "#FFFFFF";
	QString secondary = "#B3B3B3";
	QString processedSvg = preprocessSvg(icon.svg, primary, secondary);
	QSvgRenderer svgRenderer(processedSvg.toUtf8());

	QSizeF svgSize = svgRenderer.defaultSize();
	if (svgSize.isEmpty()) svgSize = QSizeF(24, 24);  // fallback if SVG has no default size

	// Scale the SVG to fit the available space while preserving aspect ratio
	QSizeF scaledSize = svgSize;
	scaledSize.scale(svgRect.size(), Qt::KeepAspectRatio);

	QPointF center = svgRect.center();
	QRectF renderRect(center.x() - scaledSize.width() / 2,
		center.y() - scaledSize.height() / 2,
		scaledSize.width(), scaledSize.height());

	svgRenderer.render(painter, renderRect);

	// Text below the icon
	QRect textRect(rect.left(), svgRect.bottom() + 5,
		rect.width(), rect.bottom() - svgRect.bottom() - 5);

	// Adjust font size based on item height
	QFont font = painter->font();
	int fontSize = (std::max)(8, rect.height() / 12);  // scale with item size
	font.setPointSize(fontSize);
	painter->setFont(font);
	painter->setPen(option.palette.text().color());

	painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop,
		QString("%1\n%2,%3").arg(icon.name, icon.family, icon.style));

	painter->restore();
}

QSize FontAwesomeIconDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
	return _itemSize;  // Adjust as needed
}

FontAwesomeIconGridWidget::FontAwesomeIconGridWidget(QWidget* parent)
	: QWidget(parent), _view(new QListView(this)), _model(new FontAwesomeIconModel(this))
{
	auto* layout = new QVBoxLayout(this);
	layout->addWidget(_view);

	_view->setModel(_model);
	_view->setItemDelegate(new FontAwesomeIconDelegate(this));
	_view->setViewMode(QListView::IconMode);
	_view->setResizeMode(QListView::Adjust);
	_view->setUniformItemSizes(true);
	_view->setSelectionMode(QAbstractItemView::SingleSelection);
	_view->setSpacing(10);
	_view->setWrapping(true);

	connect(_view->selectionModel(), &QItemSelectionModel::selectionChanged,
		this, [this](const QItemSelection& selected, const QItemSelection&) {
			if (!selected.indexes().isEmpty()) {
				QModelIndex index = selected.indexes().first();
				FAIconData icon = index.data(Qt::UserRole).value<FAIconData>();
				emit iconSelectionChanged(icon);
			}
			else {
				emit iconSelectionChanged(std::nullopt);
			}
		});
}

void FontAwesomeIconGridWidget::setIcons(const std::vector<FontAwesomeIcon>& icons) {
	std::vector<FAIconData> iconData;
	for (const auto& icon : icons) {
		for (const auto& [family, familyIcons] : icon.getIcons()) {
			for (const auto& [style, svg] : familyIcons) {
				iconData.push_back({
					icon.getLabel().c_str(),
					family.c_str(),
					style.c_str(),
					svg.c_str()
					});
			}
		}
	}

	_model->setIcons(iconData);
}

void FontAwesomeIconGridWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	_updateGridSize();  // Recalculate grid on resize
}

void FontAwesomeIconGridWidget::_updateGridSize() {
	if (!_view) return;

	const int maxItemWidth = 110;
	const int spacing = 10;  // Ensure spacing is manually respected
	_view->setSpacing(spacing);

	const int viewWidth = _view->viewport()->width();

	// Leave space for (columns - 1) spacings between items
	int columns = (std::max)(1, (viewWidth + spacing) / (maxItemWidth + spacing));

	// Adjusted width per item (including spacing)
	int totalSpacing = spacing * (columns - 1);
	int itemWidth = (viewWidth - totalSpacing) / columns;
	int itemHeight = itemWidth * 1.25;

	QSize newSize(itemWidth, itemHeight);
	_view->setGridSize(newSize);

	auto* delegate = qobject_cast<FontAwesomeIconDelegate*>(_view->itemDelegate());
	if (delegate) {
		delegate->setItemSize(newSize);
	}
}

std::string binaryToString(const BYTE* binaryData, DWORD dataLen, DWORD flags = CRYPT_STRING_BASE64) {
	DWORD stringLen = 0;
	// Get the required string length, not including the null terminator.
	if (!CryptBinaryToStringA(binaryData, dataLen, flags, nullptr, &stringLen)) {
		blog(LOG_ERROR, "Could not convert binary data to string.");
		return "";
	}

	// Allocate memory for the string, including the null terminator.
	std::string encodedString;
	encodedString.resize(stringLen);

	// Perform the actual conversion.
	if (!CryptBinaryToStringA(binaryData, dataLen, flags, encodedString.data(), &stringLen)) {
		blog(LOG_ERROR, "Could not convert binary data to string.");
		return "";
	}
	return encodedString;
}

DATA_BLOB stringToBinary(const std::string& input, DWORD flags = CRYPT_STRING_BASE64) {
	DWORD binarySize = 0;
	DATA_BLOB output;
	output.pbData = nullptr;
	output.cbData = 0;
	// Get the required size for the binary data
	if (!CryptStringToBinaryA(input.c_str(), (DWORD)input.length(), flags, nullptr, &binarySize, nullptr, nullptr)) {
		blog(LOG_INFO, "Could not string to binary. Could not determine needed binary data size.");
		return output;
	}

	// Allocate memory for the binary data
	output.pbData = (BYTE*)LocalAlloc(LMEM_FIXED, binarySize);
	if (output.pbData == nullptr) {
		blog(LOG_INFO, "Could not convert string to binary. Memory allocation error.");
		return output;
	}
	output.cbData = binarySize;
	// Perform the actual conversion
	if (!CryptStringToBinaryA(input.c_str(), (DWORD)input.length(), flags, output.pbData, &binarySize, nullptr, nullptr)) {
		blog(LOG_INFO, "Could not string to binary. Invalid conversion.");
		return output;
	}
	return output;
}


// Encrypts a string, and returns string of encrypted binary data
// as a formatted BASE 64 encoded string.
std::string encryptString(std::string input)
{
	DATA_BLOB DataIn;
	DATA_BLOB DataOut;
	BYTE* pbDataInput = (BYTE*)input.c_str();
	DWORD cbDataInput = DWORD(strlen((char*)pbDataInput) + 1);
	DataIn.pbData = pbDataInput;
	DataIn.cbData = cbDataInput;
	std::string encrypted = "";
	if (CryptProtectData(
		&DataIn,
		L"",
		NULL,
		NULL,
		NULL,
		0,
		&DataOut))
	{
		//convert binary to formatted base 64 encoded string.
		encrypted = binaryToString(DataOut.pbData, DataOut.cbData);
		LocalFree(DataOut.pbData);
	}
	else {
		blog(LOG_ERROR, "Could not encrypt string.");
	}
	return encrypted;
}

// Decrypts a formatted base64 encoded string.  First
// converts string to binary blob, then uses windows
// cryto API to decrypt the binary blob into a usable
// string
std::string decryptString(std::string input)
{
	DATA_BLOB ToDecrypt = stringToBinary(input);
	if (ToDecrypt.pbData == nullptr) {
		return "";
	}
	DATA_BLOB DataVerify;
	LPWSTR pDescrOut = NULL;

	std::string decrypted = "";

	if (CryptUnprotectData(
		&ToDecrypt,
		&pDescrOut,
		NULL,
		NULL,
		NULL,
		0,
		&DataVerify))
	{
		decrypted = std::string(reinterpret_cast<char*>(DataVerify.pbData), DataVerify.cbData);
		LocalFree(DataVerify.pbData);
		LocalFree(pDescrOut);
		if (ToDecrypt.pbData != nullptr) {
			LocalFree(ToDecrypt.pbData);
		}
	}
	else {
		blog(LOG_ERROR, "Could not decrypt string.");
	}
	return decrypted;
}

std::string get_api_token()
{
	obs_data_t* config = load_module_config();
	std::string encryptedToken = obs_data_get_string(config, "apiToken");
	obs_data_release(config);
	if (encryptedToken == "") {
		return "";
	}
	std::string token = decryptString(encryptedToken);
	return token;
}

void save_api_token(std::string token)
{
	obs_data_t* config = load_module_config();
	std::string encryptedToken = token == "" ? "" : encryptString(token);
	obs_data_set_string(config, "apiToken", encryptedToken.c_str());
	save_module_config(config);
	obs_data_release(config);
}


obs_data_t* load_module_config()
{
	const auto confPath = obs_module_config_path("config.json");
	obs_data_t* config =
		obs_data_create_from_json_file_safe(confPath, "bak");
	bfree(confPath);
	if (!config) {
		config = obs_data_create();
		blog(LOG_WARNING, "Configuration file not found");
	}
	else {
		blog(LOG_INFO, "Loaded configuration file");
	}

	obs_data_set_default_string(config, "apiToken", "");

	return config;
}

void save_module_config(obs_data_t* config)
{
	char* configPath = obs_module_config_path("config.json");
	if (!configPath)
		return;
	std::string path = configPath;
	bfree(configPath);

	size_t pos = path.rfind('/');
	if (pos == std::string::npos) {
		blog(LOG_ERROR, "Settings NOT saved.");
		return;
	}
	std::string configDir = path.substr(0, pos);

	os_mkdirs(configDir.c_str());
	if (obs_data_save_json_safe(config, path.c_str(), "tmp", "bak")) {
		blog(LOG_INFO, "Settings saved");
	}
	else {
		blog(LOG_ERROR, "Settings NOT saved.");
	}
}

std::string fetch_string_from_post(std::string url, std::string postdata, std::string token)
{
	std::string result;
	CURL* curl_instance = curl_easy_init();
	curl_easy_setopt(curl_instance, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_instance, CURLOPT_WRITEFUNCTION,
		write_data<std::string>);
	curl_easy_setopt(curl_instance, CURLOPT_WRITEDATA,
		static_cast<void*>(&result));
	if (token != "") {
		curl_easy_setopt(curl_instance, CURLOPT_XOAUTH2_BEARER,
			token.c_str());
		curl_easy_setopt(curl_instance, CURLOPT_HTTPAUTH,
			CURLAUTH_BEARER);
	}

	curl_easy_setopt(curl_instance, CURLOPT_POSTFIELDS, postdata.c_str());
	struct curl_slist* hs = NULL;
	hs = curl_slist_append(hs, "Content-Type: application/json");
	curl_easy_setopt(curl_instance, CURLOPT_HTTPHEADER, hs);
	

	CURLcode res = curl_easy_perform(curl_instance);

	curl_easy_cleanup(curl_instance);
	if (res == CURLE_OK) {
		return result;
	}
	return "{\"error\": \"Unspecified Error\"}";
}

QString preprocessSvg(const QString& rawSvg, const QString& primary, const QString& secondary) {
	QDomDocument doc;
	doc.setContent(rawSvg);

	QDomElement root = doc.documentElement();
	QDomNodeList elements = root.elementsByTagName("*");
	QDomNode child = root.firstChild();
	while (!child.isNull()) {
		QDomNode next = child.nextSibling();

		if (!child.isElement()) {
			child = next;
			continue;
		}

		QDomElement el = child.toElement();
		if (el.tagName() == "defs") {
			root.removeChild(child);  // Remove the <defs> element
			child = next;
			continue;
		}
		if (el.tagName() != "path" && el.tagName() != "rect" &&
			el.tagName() != "circle" && el.tagName() != "polygon") {
			child = next;
			continue;
		}
			
		QString classAttr = el.attribute("class");
		QString fillAttr = el.attribute("fill");

		if (classAttr.contains("fa-secondary", Qt::CaseInsensitive)) {
			el.setAttribute("fill", secondary);
		}
		else if (fillAttr.isEmpty() || classAttr.contains("fa-primary")) {
			el.setAttribute("fill", primary);
		}
		child = next;
	}

	return doc.toString();
}
