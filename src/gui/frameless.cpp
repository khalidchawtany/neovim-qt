#include "FrameLess.h"

FrameLess::FrameLess(QWidget *parent) :
_parent(parent),
_cursorchanged(false),
_leftButtonPressed(false),
_mousePress(None),
_mouseMove(None),
_borderWidth(5)
{
    _parent->setMouseTracking(true);
    _parent->setWindowFlags(Qt::FramelessWindowHint);
    _parent->setAttribute(Qt::WA_Hover);
    _parent->installEventFilter(this);
    _rubberband = new QRubberBand(QRubberBand::Rectangle);
}

FrameLess::~FrameLess() {}

bool FrameLess::eventFilter(QObject *o, QEvent*e) {
    if (e->type() == QEvent::MouseMove ||
        e->type() == QEvent::HoverMove ||
        e->type() == QEvent::Leave ||
        e->type() == QEvent::MouseButtonPress ||
        e->type() == QEvent::MouseButtonRelease) {

        switch (e->type())
        {
        case QEvent::MouseMove:
            mouseMove(static_cast<QMouseEvent*>(e));
            return true;
            break;
        case QEvent::HoverMove:
            mouseHover(static_cast<QHoverEvent*>(e));
            return true;
            break;
        case QEvent::Leave:
            mouseLeave(e);
            return true;
            break;
        case QEvent::MouseButtonPress:
            mousePress(static_cast<QMouseEvent*>(e));
            return true;
            break;
        case QEvent::MouseButtonRelease:
            mouseRealese(static_cast<QMouseEvent*>(e));
            return true;
            break;
        }
    } else {
        return false;
    }
}

void FrameLess::mouseHover(QHoverEvent *e) {
    updateCursorShape(_parent->mapToGlobal(e->pos()));

}

void FrameLess::mouseLeave(QEvent *e) {
    Q_UNUSED(e);
    if (!_leftButtonPressed) {
        _parent->unsetCursor();
    }
}

void FrameLess::mousePress(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        _leftButtonPressed = true;
        calculateCursorPosition(e->globalPos(), _parent->frameGeometry(), _mousePress);
        if (_mousePress != None) {
            _rubberband->setGeometry(_parent->frameGeometry());
        }
    }
}

void FrameLess::mouseRealese(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        _leftButtonPressed = false;
    }
}

void FrameLess::mouseMove(QMouseEvent *e) {
    if (_leftButtonPressed) {
        if (_mousePress != None) {
            int left = _rubberband->frameGeometry().left();
            int top = _rubberband->frameGeometry().top();
            int right = _rubberband->frameGeometry().right();
            int bottom = _rubberband->frameGeometry().bottom();
            if (_mousePress == Top) {
                top = e->globalPos().y();
            } else if (_mousePress == Bottom) {
                bottom = e->globalPos().y();
            } else if (_mousePress == Left) {
                left = e->globalPos().x();
            } else if (_mousePress == Right) {
                right = e->globalPos().x();
            } else if (_mousePress == TopLeft) {
                top = e->globalPos().y();
                left = e->globalPos().x();
            } else if (_mousePress == TopRight) {
                right = e->globalPos().x();
                top = e->globalPos().y();
            } else if (_mousePress == BottomLeft) {
                bottom = e->globalPos().y();
                left = e->globalPos().x();
            } else if (_mousePress == BottomRight) {
                bottom = e->globalPos().y();
                right = e->globalPos().x();
            }
            QRect newRect(QPoint(left, top), QPoint(right, bottom));
            if (newRect.width() < _parent->minimumWidth()) {
                left = _parent->frameGeometry().x();
            }
            else if (newRect.height() < _parent->minimumHeight()) {
                top = _parent->frameGeometry().y();
            }
            _parent->setGeometry(QRect(QPoint(left, top), QPoint(right, bottom)));
            _rubberband->setGeometry(QRect(QPoint(left, top), QPoint(right, bottom)));
        }
    } else {
        updateCursorShape(e->globalPos());
    }
}

void FrameLess::updateCursorShape(const QPoint &pos) {
    if (_parent->isFullScreen() || _parent->isMaximized()) {
        if (_cursorchanged) {
            _parent->unsetCursor();
        }
        return;
    }
    if (!_leftButtonPressed) {
        calculateCursorPosition(pos, _parent->frameGeometry(), _mouseMove);
        if (_mouseMove == Top || _mouseMove == Bottom) {
            _parent->setCursor(Qt::SizeVerCursor);
            _cursorchanged = true;
        } else if (_mouseMove == Left || _mouseMove == Right) {
            _parent->setCursor(Qt::SizeHorCursor);
            _cursorchanged = true;
        } else if (_mouseMove == TopLeft || _mouseMove == BottomRight) {
            _parent->setCursor(Qt::SizeFDiagCursor);
            _cursorchanged = true;
        } else if (_mouseMove == TopRight || _mouseMove == BottomLeft) {
            _parent->setCursor(Qt::SizeBDiagCursor);
            _cursorchanged = true;
        } else if (_cursorchanged) {
            _parent->unsetCursor();
            _cursorchanged = false;
        }
    }
}

void FrameLess::calculateCursorPosition(const QPoint &pos, const QRect &framerect, Edge &_edge) {
    bool onLeft = pos.x() >= framerect.x() - _borderWidth && pos.x() <= framerect.x() + _borderWidth &&
        pos.y() <= framerect.y() + framerect.height() - _borderWidth && pos.y() >= framerect.y() + _borderWidth;

    bool onRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
        pos.y() >= framerect.y() + _borderWidth && pos.y() <= framerect.y() + framerect.height() - _borderWidth;

    bool onBottom = pos.x() >= framerect.x() + _borderWidth && pos.x() <= framerect.x() + framerect.width() - _borderWidth  &&
        pos.y() >= framerect.y() + framerect.height() - _borderWidth && pos.y() <= framerect.y() + framerect.height();

    bool onTop = pos.x() >= framerect.x() + _borderWidth && pos.x() <= framerect.x() + framerect.width() - _borderWidth &&
        pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

    bool  onBottomLeft = pos.x() <= framerect.x() + _borderWidth && pos.x() >= framerect.x() &&
        pos.y() <= framerect.y() + framerect.height() && pos.y() >= framerect.y() + framerect.height() - _borderWidth;

    bool onBottomRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
        pos.y() >= framerect.y() + framerect.height() - _borderWidth && pos.y() <= framerect.y() + framerect.height();

    bool onTopRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
        pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

    bool onTopLeft = pos.x() >= framerect.x() && pos.x() <= framerect.x() + _borderWidth &&
        pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

    if (onLeft) {
        _edge = Left;
    } else if (onRight) {
        _edge = Right;
    } else if (onBottom) {
        _edge = Bottom;
    } else if (onTop) {
        _edge = Top;
    } else if (onBottomLeft) {
        _edge = BottomLeft;
    } else if (onBottomRight) {
        _edge = BottomRight;
    } else if (onTopRight) {
        _edge = TopRight;
    } else if (onTopLeft) {
        _edge = TopLeft;
    } else {
        _edge = None;
    }
}

void FrameLess::setBorderWidth(const qint16 &borderWidth) {
    _borderWidth = borderWidth;
}

qint16 FrameLess::borderWidth() const {
    return _borderWidth;
}
