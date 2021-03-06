#ifndef FORMTEST_H
#define FORMTEST_H

#include <QWidget>

namespace Ui {
class FormTest;
}

class FormTest : public QWidget
{
    Q_OBJECT

public:
    explicit FormTest(QWidget *parent = 0);
    ~FormTest();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormTest *ui;
};

#endif // FORMTEST_H
