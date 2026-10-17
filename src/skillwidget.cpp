#include "skillwidget.h"
#include "assets.h"

#include "ui_skillwidget.h"

#include <QIntValidator>
#include <QListWidgetItem>
#include <nlohmann/json.hpp>

namespace {

struct KeyCode {
    QString name;
    int code;
};

const QVector<KeyCode> codes = {
    {"None", 0}, //
    {"F1", 0x70},       {"F2", 0x71},      {"F3", 0x72},        {"F4", 0x73},       {"F5", 0x74},
    {"F6", 0x75},       {"F7", 0x76},      {"F8", 0x77},        {"F9", 0x78},       {"F10", 0x79},
    {"F11", 0x7A},      {"F12", 0x7B},

    {"A", 0x41},        {"B", 0x42},       {"C", 0x43},         {"D", 0x44},        {"E", 0x45},
    {"F", 0x46},        {"G", 0x47},       {"H", 0x48},         {"I", 0x49},        {"J", 0x4A},
    {"K", 0x4B},        {"L", 0x4C},       {"M", 0x4D},         {"N", 0x4E},        {"O", 0x4F},
    {"P", 0x50},        {"Q", 0x51},       {"R", 0x52},         {"S", 0x53},        {"T", 0x54},
    {"U", 0x55},        {"V", 0x56},       {"W", 0x57},         {"X", 0x58},        {"Y", 0x59},
    {"Z", 0x5A},

    {"0", 0x30},        {"1", 0x31},       {"2", 0x32},         {"3", 0x33},        {"4", 0x34},
    {"5", 0x35},        {"6", 0x36},       {"7", 0x37},         {"8", 0x38},        {"9", 0x39},

    {"NUMPAD0", 0x60},  {"NUMPAD1", 0x61}, {"NUMPAD2", 0x62},   {"NUMPAD3", 0x63},  {"NUMPAD4", 0x64},
    {"NUMPAD5", 0x65},  {"NUMPAD6", 0x66}, {"NUMPAD7", 0x67},   {"NUMPAD8", 0x68},  {"NUMPAD9", 0x69},
    {"MULTIPLY", 0x6A}, {"ADD", 0x6B},     {"SEPARATOR", 0x6C}, {"SUBTRACT", 0x6D}, {"DECIMAL", 0x6E},
    {"DIVIDE", 0x6F},
};

const QString &nameFromCode(int code) {
    for (auto it = codes.begin(); it != codes.end(); ++it) {
        if (it->code == code) {
            return it->name;
        }
    }
    return codes.front().name;
}
} // namespace

SkillWidget::SkillWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SkillWidget) {
    ui->setupUi(this);

    ui->lineDuration->setValidator(new QIntValidator(0, 3599, ui->lineDuration));

    for (const auto &img : Assets::the().images()) {
        ui->comboSkill->addItem(img.name);
    }

    for (const auto &code : codes) {
        ui->comboHotKey->addItem(code.name, code.code);
    }

    connect(ui->checkEnabled, &QCheckBox::checkStateChanged, this, &SkillWidget::changed);
    connect(ui->comboSkill, &QComboBox::currentIndexChanged, this, &SkillWidget::changed);
    connect(ui->comboHotKey, &QComboBox::currentIndexChanged, this, &SkillWidget::changed);
    connect(ui->lineDuration, &QLineEdit::textChanged, this, &SkillWidget::changed);
    // QMetaObject::invokeMethod(this, [this] { emit changed(); }, Qt::QueuedConnection);
}

using json = nlohmann::json;
void from_json(const json &j, SkillWidget *skill) {
    skill->blockSignals(true);
    skill->ui->comboSkill->setCurrentText(QString::fromStdString(j.value("name", "")));
    skill->ui->checkEnabled->setChecked(j.value("enabled", true));
    skill->ui->lineDuration->setText(QString::number(j.value("duration", 0)));
    skill->ui->comboHotKey->setCurrentText(nameFromCode(j.value("hotkey", 0)));
    skill->blockSignals(false);
}

void to_json(json &j, SkillWidget const *const skill) {
    auto *ui = skill->ui;
    j = {
        {"name", ui->comboSkill->currentText().toStdString()},
        {"enabled", ui->checkEnabled->isChecked()},
        {"duration", ui->lineDuration->text().toInt()},
        {"hotkey", ui->comboHotKey->currentData().toInt()},
    };
}

SkillWidget::~SkillWidget() {
    delete ui;
}

void SkillWidget::on_comboSkill_currentIndexChanged(int index) {
    const auto &item = Assets::the().images().at(index);
    ui->lblIcon->setPixmap(item.image);
    ui->lblIcon->setToolTip(item.name);
}

void SkillWidget::on_btnDel_clicked() {
    auto *item = property("listItem").value<QListWidgetItem *>();
    if (item == nullptr) {
        return;
    }
    ui->btnDel->setEnabled(false);
    delete item;
    emit changed();
}
