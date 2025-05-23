#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{
}

void mousePressEvent(QMouseEvent* event) override {
    emit clicked(event->button());
}
