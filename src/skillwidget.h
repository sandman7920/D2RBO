#ifndef SKILLWIDGET_H
#define SKILLWIDGET_H

#include "profileitem.h"

#include <QWidget>
#include <nlohmann/json_fwd.hpp>

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QWidget {
    Q_OBJECT

public:
    explicit SkillWidget(QWidget *parent = nullptr);
    ~SkillWidget();
    friend void from_json(const nlohmann::json &, SkillWidget *);
    friend void to_json(nlohmann::json &, SkillWidget const *const);
    ProfileItem profileItem();

signals:
    void changed();

private slots:
    void on_comboSkill_currentIndexChanged(int index);

    void on_btnDel_clicked();

private:
    Ui::SkillWidget *ui;
};

#endif // SKILLWIDGET_H
