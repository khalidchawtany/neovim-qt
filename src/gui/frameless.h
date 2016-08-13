#include <QtWidgets/QWidget>
#include <QtWidgets/QRubberBand>
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QRect>
#include <QtCore/QPoint>
#include <QtCore/Qt>
#include <QtGui/QHoverEvent>
#include <QtGui/QMouseEvent>

class FrameLess : public QObject {
public:
    explicit FrameLess(QWidget *parent);
    ~FrameLess();
    void setBorderWidth(const qint16 &borderWidth);
    qint16 borderWidth() const;

private:
    QWidget *_parent = new QWidget();
    enum Edge {
        None = 0x00,
        Left = 0x01,
        Top = 0x02,
        Right = 0x03,
        Bottom = 0x04,
        TopLeft = 0x05,
        TopRight = 0x06,
        BottomLeft = 0x07,
        BottomRight = 0x08
    };
    QRubberBand *_rubberband;
    bool _cursorchanged;
    bool _leftButtonPressed;
    Edge _mousePress;
    Edge _mouseMove;
    qint16 _borderWidth;

private:
    void mouseHover(QHoverEvent *e);
    void mouseLeave(QEvent *e);
    void mousePress(QMouseEvent *e);
    void mouseRealese(QMouseEvent *e);
    void mouseMove(QMouseEvent *e);
    void updateCursorShape(const QPoint &pos);
    void calculateCursorPosition(const QPoint &pos, const QRect &framerect, Edge &_edge);

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;
};
