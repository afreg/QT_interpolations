#include "widget.h"
#include "ui_widget.h"
#include "Interpolation.h"

using interpolation::interp;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    GenerateRandomArray();
}

Widget::~Widget()
{
    //delete [] arr;
    delete ui;
}

void Widget::GenerateRandomArray() {
    for (int it = 0; it < len; ++it) {
        arr[it] = (max-min)*QRandomGenerator::global()->generateDouble()+min;
    }
};

void Widget::paintEvent(QPaintEvent *event)
{    
    double multY = (double)(this->height()-50)/(max - min);
    double multX = (double)(this->width()-50)/(len-1);

    QPainter p(this);

    // Points
    p.setPen(QPen(Qt::blue, 2));
    for (int it = 0; it<len; ++it){
        int coordX = 25+it*multX;
        int coordY = this->height()-25-multY*(arr[it]-min);
        p.drawEllipse(QPoint(coordX, coordY), 5, 5);
    }


    interp L;

    switch (interp_type){
    case 0:
        break;
    case 1:
        // Linear interpolation
        L.linear(arr, len);
        p.setPen(QPen(Qt::black, 2));
        break;
    case 2:
        // Quadric interpolation
        L.quadric(arr, len);
        p.setPen(QPen(Qt::green, 2));
        break;
    case 3:
        // Cubic interpolation
        L.cubic(arr, len);
        p.setPen(QPen(Qt::red, 2));
        break;
    }
    if (interp_type != 0){
        for (int it = 25; it <this->width()-24; ++it){
            int coordX = it;
            float x = (float)(it - 25)/multX;
            int coordY = this->height()-25-multY*(L(x)-min);
            p.drawPoint(QPoint(coordX, coordY));
        }
    }
}


void Widget::on_pushButton_clicked()
{
    GenerateRandomArray();
    update();
}

void Widget::on_radioButton_4_toggled(bool checked)
{
    if (checked){
        interp_type = 0;
        update();
    }
}

void Widget::on_radioButton_toggled(bool checked)
{
    if (checked){
        interp_type = 1;
        update();
    }
}


void Widget::on_radioButton_2_toggled(bool checked)
{
    if (checked){
        interp_type = 2;
        update();
    }
}


void Widget::on_radioButton_3_toggled(bool checked)
{
    if (checked){
        interp_type = 3;
        update();
    }
}

