#include "dateselectdialog.h"

DateSelectDialog::DateSelectDialog(QWidget *parent)
    : QDialog(parent)
{
    calendar = new QCalendarWidget(this);
    calendar->setSelectedDate(QDate::currentDate());

    QPushButton *okButton = new QPushButton("确定", this);
    QPushButton *cancelButton = new QPushButton("取消", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QDate DateSelectDialog::selectedDate() const {
    return calendar->selectedDate();
}
