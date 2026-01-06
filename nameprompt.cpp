#include "nameprompt.h"
#include "ui_nameprompt.h"

#include <QGraphicsDropShadowEffect>


QGraphicsDropShadowEffect* CyanGloww(){
    auto *glow = new QGraphicsDropShadowEffect;
    glow->setBlurRadius(20);
    glow->setOffset(0);  // No shadow offset — centered glow
    glow->setColor(QColor(102, 204, 255));  // Cyan glow
    return glow;
}
namePrompt::namePrompt(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::namePrompt)
{
    ui->setupUi(this);
    connect(ui->name_field, &QLineEdit::textChanged, this, [=](const QString &text){
        if (!text.isEmpty()) {
            ui->name_field->setPlaceholderText("");
            ui->name_field->setStyleSheet("color: none; QLineEdit:focus{outline:none;}");  // or default color
        }
    });

    ui->cancel_btn->setGraphicsEffect(CyanGloww());
    ui->ok_btn->setGraphicsEffect(CyanGloww());
}

namePrompt::~namePrompt()
{
    delete ui;
}
QString namePrompt::getName() const{
    return ui->name_field->text();
}


void namePrompt::on_ok_btn_clicked(){
    QString input = ui->name_field->text();
    if(input.isEmpty()){
        ui->name_field->clear();
        ui->name_field->setPlaceholderText("Input Required");
        // Set red placeholder text
        ui->name_field->setStyleSheet(R"(
            QLineEdit {
                color:red;
            }
            QLineEdit:focus{
                outline:none;
            }
        )");
        return;
    }

    ui->name_field->setStyleSheet("");  // Reset to default
    accept();
}

void namePrompt::on_cancel_btn_clicked(){
    reject();
}
