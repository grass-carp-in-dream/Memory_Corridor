#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = nullptr) : QLabel(parent) {}

signals:
    void clicked(Qt::MouseButton button);

protected:
    void mousePressEvent(QMouseEvent* event) override {
        emit clicked(event->button());
    }
};

#endif // CLICKABLELABEL_H
