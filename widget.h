#ifndef WIDGET_H
#define WIDGET_H

#include <QPushButton>
#include <QWidget>
#include <QPainter>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    const int len = 20;
    const float min = -2.5;
    const float max = 2.5;
    float arr[20];
    int interp_type = 0;
    void GenerateRandomArray();

protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pushButton_clicked();
    void on_radioButton_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);
    void on_radioButton_3_toggled(bool checked);
    void on_radioButton_4_toggled(bool checked);
};
#endif // WIDGET_H
