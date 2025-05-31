#pragma once

#include <QDialog>
#include <QCalendarWidget>
#include <QPushButton>
#include <QVBoxLayout>

class DateSelectDialog : public QDialog {
    Q_OBJECT
public:
    DateSelectDialog(QWidget *parent = nullptr);

    QDate selectedDate() const;

private:
    QCalendarWidget *calendar;
};
