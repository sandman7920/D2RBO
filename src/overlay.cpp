#include "overlay.h"
#include <QMouseEvent>
#include <QPainter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

Overlay::Overlay(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);

    calculateLayout(OverlayDirection::Horizontal);
    connect(&m_timer, &QTimer::timeout, this, [this] { update(); });
    setCursor(Qt::OpenHandCursor);
    setPreview(false);
}

Overlay &Overlay::the() {
    static Overlay overlay(nullptr);
    return overlay;
}

void Overlay::setTranslucent(bool enabled) {
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED;
    if (!enabled) {
        exStyle &= ~WS_EX_TRANSPARENT;
    }
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
#else
    setAttribute(Qt::WA_TransparentForMouseEvents); // Passthrough
    setAttribute(Qt::WA_AcceptTouchEvents, false);
#endif
}

void Overlay::setProfile(Profile *profile) {
    m_profile = profile;
    setPreview(m_preview);
    update();
}

void Overlay::start() {
    setPreview(false);
    if (m_profile != nullptr) {
        for (auto &item : m_profile->items) {
            item.expiry_at = 0;
        }
    }
    m_visible = true;
    update();
    m_timer.start(100);
}

void Overlay::stop() {
    m_visible = false;
    update();
    m_timer.stop();
}

void Overlay::setPreview(bool enabled) {
    if (m_profile) {
        for (auto &item : m_profile->items) {
            item.expiry_at = QDateTime::currentMSecsSinceEpoch() + 24 * 3600 * 10'000;
        }
    }
    m_preview = enabled;
    setMouseTracking(enabled);
    setTranslucent(!enabled);
    toggleVisible(enabled);
}

void Overlay::setPosition(const QPoint &p) {
    m_left = p.x();
    m_top = p.y();
    update();
}

void Overlay::mousePressEvent(QMouseEvent *event) {
    if (m_clip.contains(event->pos()) && event->button() == Qt::LeftButton) {
        m_last_pos = event->pos().toPointF();
        setCursor(Qt::ClosedHandCursor);
    }
}

void Overlay::mouseMoveEvent(QMouseEvent *event) {
    if (cursor() == Qt::ClosedHandCursor) {
        const auto d = event->pos() - m_last_pos;
        m_left += d.x();
        m_top += d.y();
        m_last_pos = event->pos();
        update();
    }
}

void Overlay::mouseReleaseEvent(QMouseEvent * /*event*/) {
    if (cursor() == Qt::ClosedHandCursor) {
        setCursor(Qt::OpenHandCursor);
        emit positionChanged(QPointF{m_left, m_top}.toPoint());
    }
}

void Overlay::toggleVisible(int set) {
    m_visible = set == -1 ? !m_visible : set;
    update();
}

void Overlay::paintEvent(QPaintEvent * /*event*/) {
    if (!m_visible || m_profile == nullptr) {
        return;
    }
    const double now = QDateTime::currentMSecsSinceEpoch();
    const auto &items = m_profile->items;
    int cnt = items.size();
    m_clip = {
        m_left,
        m_top,
        m_size + (cnt - 1) * m_offsetX,
        m_size + (cnt - 1) * m_offsetY + m_progress,
    };

    QPainter painter(this);
    painter.setClipRect(m_clip);
    if (m_preview) {
        painter.setOpacity(0.10);
        painter.setBrush(Qt::magenta);
        painter.drawRect(m_clip);
        painter.setBrush(Qt::NoBrush);
    }
    painter.translate(m_left, m_top);

    for (int i = 0; i < cnt; ++i) {
        QPointF top_left{m_offsetX * i, m_offsetY * i};
        QSizeF size{m_size, m_size};
        painter.setPen(Qt::NoPen);
        painter.setOpacity(m_opacity);
        auto &item = items.at(i);
        const auto delta = item.expiry_at - now;

        if (delta <= 0) {
            painter.setOpacity(0.50);
        }
        painter.drawPixmap({top_left.toPoint(), size.toSize()}, *item.image);
        if (m_warning > 0 && delta > 0 && delta <= m_warning) {
            painter.setOpacity(0.4);
            painter.setBrush(Qt::red);
            painter.drawRect({top_left, size});
            painter.setOpacity(m_opacity);
        }

        if (m_progress <= 0) {
            continue;
        }

        top_left.ry() += m_size;
        size.rheight() = m_progress;

        painter.setBrush(Qt::NoBrush);
        painter.setPen({Qt::white, .5});
        painter.drawRect({top_left, size});

        if (delta > 0) {
            painter.setPen(Qt::NoPen);
            const auto p = delta / item.time_to_live;
            size.rwidth() *= p;
            if (p >= .666) {
                painter.setBrush(Qt::green);
            } else if (p >= .333) {
                painter.setBrush(Qt::yellow);
            } else {
                painter.setBrush(Qt::red);
            }
            painter.drawRect({top_left, size});
        }
    }

    if (m_preview) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen({Qt::red, 2});
        painter.drawRect(0, 0, m_clip.width(), m_clip.height());
    }

    painter.end();
}

void Overlay::setProgressBar(int size) {
    m_progress = size;
    calculateLayout(m_direction);
}

void Overlay::setSize(int size) {
    m_size = size;
    calculateLayout(m_direction);
}

void Overlay::setWarning(int value) {
    m_warning = value * 1'000;
    update();
}

void Overlay::setSpace(int value) {
    m_space = value;
    calculateLayout(m_direction);
}

void Overlay::calculateLayout(OverlayDirection direction) {
    m_direction = direction;
    switch (direction) {
    case Verical:
        m_offsetX = 0;
        m_offsetY = m_size + m_space + m_progress;
        break;
    case Horizontal:
        m_offsetX = m_size + m_space;
        m_offsetY = 0;
        break;
    }
    update();
}
