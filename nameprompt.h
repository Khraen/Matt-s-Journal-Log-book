#ifndef NAMEPROMPT_H
#define NAMEPROMPT_H

#include <QDialog>

namespace Ui {
class namePrompt;
}

class namePrompt : public QDialog
{
    Q_OBJECT

public:
    explicit namePrompt(QWidget *parent = nullptr);
    ~namePrompt();

    QString getName()const;



private slots:
    void on_ok_btn_clicked();
    void on_cancel_btn_clicked();

private:
    Ui::namePrompt *ui;
};

#endif // NAMEPROMPT_H
