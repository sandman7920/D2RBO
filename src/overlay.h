#ifndef OVERLAY_H
#define OVERLAY_H

#include "profile.h"

#include <QDateTime>
#include <QPixmap>
#include <QTimer>
#include <QVector>
#include <QWidget>

enum OverlayDirection {
    Horizontal,
    Verical,
};

class Overlay : public QWidget {
    Q_OBJECT

public:
    static Overlay &the();
    void calculateLayout(OverlayDirection direction);
    void start();
    void stop();
    void toggleVisible(int set = -1);
    void setPreview(bool enabled);
    void setProgressBar(int size);
    void setSize(int size);
    void setPosition(const QPoint &p);
    void setWarning(int value);
    void setSpace(int value);
    void setProfile(Profile *profile);
    void refresh(int code) {
        if (m_profile) {
            m_profile->refresh(code);
        }
    }

signals:
    void positionChanged(QPoint);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    explicit Overlay(QWidget *parent = nullptr);
    void setTranslucent(bool);

    QRectF m_clip;
    QPointF m_last_pos;
    QTimer m_timer;
    Profile *m_profile{nullptr};
    OverlayDirection m_direction;
    double m_left{80};
    double m_top{140};
    double m_offsetX{0};
    double m_offsetY{0};
    double m_size{64};
    double m_space{6};
    double m_progress{3};
    double m_opacity{0.90};
    Clock::duration m_warning{Chrono::seconds(10)};
    bool m_visible{false};
    bool m_preview{false};
};

#endif // OVERLAY_H
