#include "profiles.h"
#include "assets.h"
#include "overlay.h"
#include "skillwidget.h"

#include "ui_profiles.h"
#include <QFile>
#include <QSettings>

using json = nlohmann::json;

namespace {
QString defaultProfile() {
    auto fp = QFile(":/resources/default_profile.json");
    if (fp.open(QIODevice::ReadOnly)) {
        return QString::fromLatin1(fp.readAll());
    }
    return "";
}
} // namespace

Profiles::Profiles(QWidget *parent) : QWidget(parent), ui(new Ui::Profiles) {
    ui->setupUi(this);
    Overlay::the().setProfile(&m_profile);
    m_settings = json::parse(QSettings().value("profiles", defaultProfile()).toString().toStdString());
    loadSettings();
    refreshDrawProfile();
}

Profiles::~Profiles() {
    QSettings().setValue("profiles", QString::fromStdString(m_settings.dump()));
    delete ui;
}

void Profiles::loadSettings() {
    ui->comboSelectProfile->blockSignals(true);
    for (const auto &item : m_settings["profiles"]) {
        ui->comboSelectProfile->addItem(QString::fromStdString(item.value("name", "")));
    }
    ui->comboSelectProfile->setCurrentIndex(-1);
    ui->comboSelectProfile->blockSignals(false);

    ui->comboSelectProfile->setCurrentIndex(m_settings.value("currentIndex", -1));
}

void Profiles::loadProfileFromIndex(int idx) {
    if (idx < 0) {
        return;
    }
    const auto &skills = m_settings["profiles"][idx]["skills"];
    for (auto &skill : skills) {
        auto *p = addSkill();
        skill.get_to(p);
    }
}

void Profiles::saveProfileFromIndex(int idx) {
    if (idx < 0) {
        return;
    }

    auto &profile = m_settings["profiles"][idx];
    profile["name"] = ui->comboSelectProfile->itemText(idx).toStdString();
    auto &skills = profile["skills"];
    skills.clear();

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        auto *widget = ui->listWidget->itemWidget(item);
        if (!widget) {
            continue;
        }
        auto *skill = qobject_cast<SkillWidget *>(widget);
        if (skill) {
            skills.emplace_back(skill);
        }
    }
    refreshDrawProfile();
}

void Profiles::refreshDrawProfile() {
    const auto &profile = m_settings["profiles"][ui->comboSelectProfile->currentIndex()];

    if (!profile.is_null()) {
        m_profile.name = QString::fromStdString(profile.value("name", ""));
        m_profile.items.clear();
        for (const auto &skill : profile["skills"]) {
            if (skill.value("enabled", false)) {
                ProfileItem item;
                item.time_to_live = skill.value("duration", 0) * 1000;
                item.hotkey = skill.value("hotkey", 0);
                item.name = QString::fromStdString(skill.value("name", ""));
                item.image = Assets::the().getImageByName(item.name);
                item.expiry_at = 0;
                m_profile.items.emplace_back(item);
            }
        }
    }
    emit profileChanged();
}

void Profiles::on_btnAddProfile_clicked() {
    int index = ui->comboSelectProfile->count();
    ui->comboSelectProfile->addItem(QStringLiteral("Profile ").append(QString::number(index + 1)));
    ui->comboSelectProfile->setCurrentIndex(index);
    saveProfileFromIndex(index);
}

void Profiles::on_comboSelectProfile_currentIndexChanged(int index) {
    saveProfileFromIndex(lastProfileIndex);
    ui->listWidget->clear();
    if (index < 0) {
        ui->lineProfileName->setText("");
        ui->lineProfileName->setEnabled(false);
        ui->btnlAddSkill->setEnabled(false);
        ui->btnDeleteProfile->setEnabled(false);
        return;
    }

    ui->lineProfileName->setText(ui->comboSelectProfile->itemText(index));
    ui->lineProfileName->setEnabled(true);
    ui->btnlAddSkill->setEnabled(true);
    ui->btnDeleteProfile->setEnabled(ui->comboSelectProfile->count() > 1);
    m_settings["currentIndex"] = index;
    loadProfileFromIndex(index);
    lastProfileIndex = index;
}

void Profiles::on_lineProfileName_textChanged(const QString &value) {
    auto idx = ui->comboSelectProfile->currentIndex();
    m_settings["profiles"][idx]["name"] = value.toStdString();
    ui->comboSelectProfile->setItemText(idx, value);
}

void Profiles::onProfileWidgetChanged() {
    auto *skill = qobject_cast<SkillWidget *>(sender());
    if (skill == nullptr) {
        return;
    }

    saveProfileFromIndex(ui->comboSelectProfile->currentIndex());
}

void Profiles::on_btnlAddSkill_clicked() {
    addSkill();
    saveProfileFromIndex(ui->comboSelectProfile->currentIndex());
}

SkillWidget *Profiles::addSkill() {
    auto *item = new QListWidgetItem(ui->listWidget);
    auto *skill = new SkillWidget();
    skill->setProperty("listItem", QVariant::fromValue(item));
    connect(skill, &SkillWidget::changed, this, &Profiles::onProfileWidgetChanged);

    item->setSizeHint(skill->sizeHint());
    ui->listWidget->setItemWidget(item, skill);
    return skill;
}

void Profiles::on_btnDeleteProfile_clicked() {
    auto idx = ui->comboSelectProfile->currentIndex();
    m_settings["profiles"].erase(idx);
    lastProfileIndex = -1;
    ui->comboSelectProfile->removeItem(idx);
    refreshDrawProfile();
}
