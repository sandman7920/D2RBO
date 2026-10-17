#ifndef PROFILES_H
#define PROFILES_H

#include "profile.h"

#include <QWidget>
#include <nlohmann/json.hpp>

class SkillWidget;

namespace Ui {
class Profiles;
}

class Profiles : public QWidget {
    Q_OBJECT

public:
    explicit Profiles(QWidget *parent = nullptr);
    ~Profiles();

signals:
    void profileChanged();

private slots:
    void on_btnAddProfile_clicked();

    void on_comboSelectProfile_currentIndexChanged(int);

    void on_lineProfileName_textChanged(const QString &);

    void on_btnlAddSkill_clicked();

    void onProfileWidgetChanged();

    void on_btnDeleteProfile_clicked();

private:
    Ui::Profiles *ui;
    nlohmann::json m_settings;
    Profile m_profile;
    void loadSettings();
    void loadProfileFromIndex(int idx);
    void saveProfileFromIndex(int idx);
    void refreshDrawProfile();
    int lastProfileIndex{-1};
    SkillWidget *addSkill();
};

#endif // PROFILES_H
