#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT   //宏，使用Qt信号与槽机制必须添加

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:


    void on_btnStart_clicked();

private:
    Ui::Widget *ui; //Ui::Widget类型的一个指针，指向可视化的界面
};
#endif // WIDGET_H
