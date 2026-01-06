#include "journal_system.h"
#include "./ui_journal_system.h"
#include "nameprompt.h"
#include <QTableWidgetItem>
#include <QPushButton>
#include<QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QRegularExpression>
#include <QGraphicsDropShadowEffect>
#include "borderpulseoverlay.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>



 QGraphicsDropShadowEffect* CyanGlow(){
     auto *glow = new QGraphicsDropShadowEffect;
     glow->setBlurRadius(20);
     glow->setOffset(0);  // No shadow offset — centered glow
     glow->setColor(QColor(0, 234, 255));  // Cyan glow
     return glow;
 }
 QGraphicsDropShadowEffect* YellowGlow(){
     auto *glow = new QGraphicsDropShadowEffect;
     glow->setBlurRadius(20);
     glow->setOffset(0);  // No shadow offset — centered glow
     glow->setColor(QColor(255, 166, 0));  // yellow glow
     return glow;
 }

JournalSystem::JournalSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JournalSystem)
{
    ui->setupUi(this); // Set up the UI from Qt Designer
    ui->stackedWidget->setCurrentIndex(0);
    this->populate_journal_table();
    connect(ui->journals_table, &QTableWidget::cellDoubleClicked,
            this, &JournalSystem::onJournalTableRowDoubleClicked);
    connect(ui->entry_table, &QTableWidget::cellDoubleClicked,
            this, &JournalSystem::onEntryTableRowDoubleClicked);

    QFont headerFont;
    headerFont.setFamily("Helvetica");
    headerFont.setPointSize(13);
    headerFont.setBold(true); // Optional

    // auto *glow = new QGraphicsDropShadowEffect;
    // glow->setBlurRadius(20);
    // glow->setOffset(0);  // No shadow offset — centered glow
    // glow->setColor(QColor(102, 204, 255));  // Cyan glow
    // glow effect block
    ui->passwordLineEdit->setGraphicsEffect(CyanGlow());
    ui->confirmPasswordLineEdit->setGraphicsEffect(CyanGlow());

    ui->add_journal_btn->setGraphicsEffect(CyanGlow());
    ui->delete_journal_btn->setGraphicsEffect(CyanGlow());
    ui->add_entry_btn->setGraphicsEffect(YellowGlow());
    ui->delete_entry_btn->setGraphicsEffect(YellowGlow());
    ui->back_btn1->setGraphicsEffect(YellowGlow());
    ui->user_entry->setGraphicsEffect(CyanGlow());
    ui->weather_entry->setGraphicsEffect(YellowGlow());
    ui->mood_entry->setGraphicsEffect(YellowGlow());
    ui->description_entry->setGraphicsEffect(YellowGlow());
    ui->entry_input2->setGraphicsEffect(CyanGlow());
    ui->entry_edit2->setGraphicsEffect(YellowGlow());
    ui->weather_label->setGraphicsEffect(YellowGlow());
    ui->weather_label2->setGraphicsEffect(CyanGlow());
    ui->mood_label->setGraphicsEffect(YellowGlow());
    ui->mood_label2->setGraphicsEffect(CyanGlow());
    ui->date_label->setGraphicsEffect(YellowGlow());
    ui->date_label2->setGraphicsEffect(CyanGlow());
    ui->time_label->setGraphicsEffect(YellowGlow());
    ui->time_label2->setGraphicsEffect(CyanGlow());
    ui->back_btn3->setGraphicsEffect(CyanGlow());
    ui->edit_btn->setGraphicsEffect(CyanGlow());
    ui->cancel_edit_btn->setGraphicsEffect(YellowGlow());
    ui->save_btn2->setGraphicsEffect(YellowGlow());
    ui->back_btn2->setGraphicsEffect(CyanGlow());
    ui->save_btn->setGraphicsEffect(CyanGlow());
    // glow effect block
    ui->journals_table->setGraphicsEffect(CyanGlow());

    ui->journals_table->horizontalHeader()->setFont(headerFont);
    ui->entry_table->horizontalHeader()->setFont(headerFont);

    //connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &JournalSystem::onPasswordEntered);

    bool firstTime = !QFile::exists(getJournalFilePath());
    //qDebug() << firstTime;
    //QString filePath = QDir::current().absoluteFilePath("journal.dat");
    //qDebug() << "Saving/loading journal file at:" << filePath;

    if (firstTime) {
        ui->password_label->setText("Create a new password. This password cannot be recovered:");
        ui->confirmPasswordLineEdit->setVisible(true);
        ui->confirmLabel->setVisible(true);
    } else {
        ui->password_label->setText("Enter your password:");
        ui->confirmPasswordLineEdit->setVisible(false);
        ui->confirmLabel->setVisible(false);
    }

    ui->passwordMsg->setVisible(false);
    ui->confirmMsg->setVisible(false);


}


JournalSystem::~JournalSystem()
{
    delete ui;
}
void JournalSystem::onJournalTableRowDoubleClicked(int row, int column)
{
    qDebug() << "Double-clicked row:" << row;

    // Optionally: store the selected journal or pass data to the next page
    current_journal_index_ = row;  // store if needed
    this->current_page_index_ = 2;
    ui->stackedWidget->setCurrentIndex(2);  // or whatever page index you want
    ui->journal_title->setText(QString::fromStdString(this->CurrentJournal().getName()));
    this->populate_entry_table();

}
void JournalSystem::onEntryTableRowDoubleClicked(int row, int column){
    qDebug() << "Double-clicked row:" << row;

    //this->journal_list_[current_journal_index_].setCurrentEntryIndex(row);
    // Switch to the desired page in the stacked widget
    current_page_index_ = 3;
    ui->stackedWidget->setCurrentIndex(current_page_index_);  // view page
    ui->journal_title3->setText(QString::fromStdString(this->CurrentJournal().getName()));
    Entry viewing_entry = this->CurrentJournal().GetEntries()[row];
    this->CurrentJournal().setCurrentEntryIndex(row);
    ui->time_label->setText(viewing_entry.getQTime().toString("hh:mm AP"));
    ui->date_label->setText(viewing_entry.getQDate().toString("🗓yyyy-MM-dd"));
    ui->mood_label->setText(QString::fromStdString(viewing_entry.getMood()));
    ui->weather_label->setText(QString::fromStdString(viewing_entry.getWeather()));
    ui->entry_input2->setPlainText(QString::fromStdString(viewing_entry.getText()));

    // populate view page

}
void JournalSystem::show_journals() const{
    for(int i =0; i<this->journal_list_.size();i++){
        qDebug() << "Journal "<< i+1 << ":" << journal_list_[i].getName();
    }
}
void JournalSystem::populate_journal_table(){
    ui->journals_table->setRowCount(journal_list_.size());
    ui->journals_table->setColumnCount(3);
    ui->journals_table->setColumnWidth(1,200);

    for(int row = 0; row < journal_list_.size();row++){
        //set dates column
        QString dateStr = journal_list_[row].getDate().toString("yyyy-MM-dd");
        QTableWidgetItem *date_item = new QTableWidgetItem(dateStr);
        ui->journals_table->setItem(row,0,date_item);
        //
        QPushButton *view_button = new QPushButton();

        //view_button->setStyleSheet("background-color: transparent;");

        view_button->setObjectName("view_btn");
        view_button->setStyleSheet("background-color: darkred; border-radius:6px; padding-left:22px; padding-right:22px; padding-top:6px; padding-bottom:6px;");

        // Create a container widget
        QWidget* container = new QWidget();

        container->setAutoFillBackground(false);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setStyleSheet("background-color: transparent;");

        // Create a layout with center alignment
        QHBoxLayout* layout = new QHBoxLayout(container);

        layout->addWidget(view_button);
        layout->setAlignment(Qt::AlignRight);
        layout->setContentsMargins(0, 0, 25, 0);  // Remove extra margins
        view_button->setText("view");

        // Capture the current row using a lambda
        connect(view_button, &QPushButton::clicked, this, [this, row]() {
            onJournalViewButtonClicked(row);
        });

        ui->journals_table->setCellWidget(row,2,container);
        //set names column
        QTableWidgetItem *name_item = new QTableWidgetItem(QString::fromStdString(journal_list_[row].getName()));
        ui->journals_table->setItem(row,1, name_item);

        // ✅ Add the animated border overlay here
        static BorderPulseOverlay *pulseEffect = nullptr;
        if (!pulseEffect) {
            pulseEffect = new BorderPulseOverlay(ui->journals_table, this);
            pulseEffect->show();
        }
    }
}
void JournalSystem::on_add_journal_btn_clicked()
{
    namePrompt dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        std::string journal_name = dialog.getName().toStdString();  // Retrieve user input
        qDebug() << "User entered:" << journal_name;
        Journal new_journal(journal_name);
        QDate currentDate = QDate::currentDate();
        new_journal.setDate(currentDate);
        qDebug() << "new journal made!";
        this->journal_list_.push_back(new_journal);
        this->populate_journal_table();
        this->show_journals();


    }

}
void JournalSystem::onJournalViewButtonClicked(int row){
    if(ui->stackedWidget->currentIndex() == 1){
        this->current_journal_index_ = row;
        this->current_page_index_ = 1;
    }
    this->current_page_index_ = 2;
    ui->stackedWidget->setCurrentIndex(2);

    ui->journal_title->setText(QString::fromStdString(this->CurrentJournal().getName()));
    this->populate_entry_table();

}
void JournalSystem::onEntryViewButtonClicked(int row){
    if(ui->stackedWidget->currentIndex() != 2){
        return;
    }
    this->current_page_index_ = 3;
    ui->stackedWidget->setCurrentIndex(current_page_index_);
    const Entry& viewing_entry = this->CurrentJournal().GetEntries()[row];
    this->CurrentJournal().setCurrentEntryIndex(row);
    ui->journal_title3->setText(QString::fromStdString(this->CurrentJournal().getName()));
    ui->time_label->setText(viewing_entry.getQTime().toString("hh:mm AP"));
    ui->date_label->setText(viewing_entry.getQDate().toString("🗓yyyy-MM-dd"));
    ui->mood_label->setText(QString::fromStdString(viewing_entry.getMood()));
    ui->weather_label->setText(QString::fromStdString(viewing_entry.getWeather()));
    qDebug() << viewing_entry.getText();
    ui->entry_input2->setPlainText(QString::fromStdString(viewing_entry.getText()));

}


void JournalSystem:: on_delete_journal_btn_clicked(){
    // Get all selected items (cells)
    QList<QTableWidgetItem*> selectedItems = ui->journals_table->selectedItems();

    // Use a set to keep unique row numbers
    QSet<int> rowsToDelete;
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

    // Convert to list and sort descending to delete rows safely
    QList<int> sortedRows = rowsToDelete.values();
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    for (int row : sortedRows) {
        qDebug() << "Journal deleted: " << journal_list_[row].getName();
        journal_list_.erase(journal_list_.begin() + row);

        ui->journals_table->removeRow(row);
    }

}
void JournalSystem:: on_delete_entry_btn_clicked(){
    std::vector<Entry> entry_list = journal_list_[current_journal_index_].GetEntries();
    if(entry_list.size() == 0){
        return;
    }
    // Get all selected items (cells)
    QList<QTableWidgetItem*> selectedItems = ui->entry_table->selectedItems();

    // Use a set to keep unique row numbers
    QSet<int> rowsToDelete;
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

    // Convert to list and sort descending to delete rows safely
    QList<int> sortedRows = rowsToDelete.values();
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    for (int row : sortedRows) {
        qDebug() << "entry deleted: " << entry_list[row].getDescription();
        this->CurrentJournal().GetEntries().erase(this->CurrentJournal().GetEntries().begin() + row);
        ui->entry_table->removeRow(row);
    }
}
void JournalSystem::on_back_btn1_clicked(){
    this->current_page_index_=1;
    ui->stackedWidget->setCurrentIndex(this->current_page_index_);
}
void JournalSystem::on_back_btn2_clicked(){
    this->current_page_index_ = 2;
    ui->stackedWidget->setCurrentIndex(this->current_page_index_);
}
void JournalSystem::on_back_btn3_clicked(){
    this->current_page_index_ = 2;
    ui->stackedWidget->setCurrentIndex(this->current_page_index_);
}
void JournalSystem::populate_entry_table(){

    std::vector<Entry> entry_list = this->CurrentJournal().GetEntries();

    ui->entry_table->setRowCount(entry_list.size());
    ui->entry_table->setColumnCount(4);
    ui->entry_table->setColumnWidth(2,200);
    qDebug() << "inside entry table";
    for(int row = 0; row < entry_list.size();row++){
        qDebug() << "in for loop";
        //set dates column

        QDate date = entry_list[row].getQDate();
        QString dateStr = date.toString("yyyy-MM-dd");

        QTableWidgetItem *date_item = new QTableWidgetItem(dateStr);
        ui->entry_table->setItem(row,0,date_item);
        qDebug() << "Created date for entry";
        //set time column
        QTableWidgetItem *time_item = new QTableWidgetItem(entry_list[row].getQTime().toString("hh:mm AP"));
        ui->entry_table->setItem(row,1,time_item);


        QPushButton *view_button = new QPushButton();
        view_button->setObjectName("entry_view_btn");
        view_button->setStyleSheet("background-color: darkred; border-radius:6px; padding-left:22px; padding-right:22px; padding-top:6px; padding-bottom:6px;");
        // Create a container widget
        QWidget* container = new QWidget();

        container->setAutoFillBackground(false);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setStyleSheet("background-color: transparent;");


        // Create a layout with center alignment
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->addWidget(view_button);
        layout->setAlignment(Qt::AlignRight);
        layout->setContentsMargins(0, 0, 25, 0);  // Remove extra margins
        view_button->setText("view");

        // Capture the current row using a lambda
        connect(view_button, &QPushButton::clicked, this, [this, row]() {
            onEntryViewButtonClicked(row);
        });
        ui->entry_table->setCellWidget(row,3,container);

        //set names column
        QTableWidgetItem *description_item = new QTableWidgetItem(QString::fromStdString(entry_list[row].getDescription()));
        ui->entry_table->setItem(row,2, description_item);
        // ✅ Add the animated border overlay here
        static BorderPulseOverlay *pulseEffect = nullptr;

        if (!pulseEffect) {
            pulseEffect = new BorderPulseOverlay(ui->entry_table, this);
            pulseEffect->setPulseColor(QColor(255, 166, 0)); // Tron orangish-yellow
            pulseEffect->show();
        }
    }

}
void JournalSystem::on_add_entry_btn_clicked(){
    ui->stackedWidget->setCurrentIndex(4);
    this->current_page_index_ = 4;
    ui->weather_entry->clear();
    ui->mood_entry->clear();
    ui->description_entry->clear();
    ui->user_entry->clear();

}
void JournalSystem::on_edit_btn_clicked(){
    ui->stackedWidget->setCurrentIndex(5);
    ui->journal_title4->setText(QString::fromStdString(this->CurrentJournal().getName()));
    Entry viewing_entry = this->CurrentJournal().GetEntries()[this->CurrentJournal().getCurrentEntryIndex()];
    ui->time_label2->setText(viewing_entry.getQTime().toString("hh:mm AP"));
    ui->date_label2->setText(viewing_entry.getQDate().toString("🗓yyyy-MM-dd"));
    ui->mood_label2->setText(QString::fromStdString(viewing_entry.getMood()));
    ui->weather_label2->setText(QString::fromStdString(viewing_entry.getWeather()));
    ui->entry_edit2->setPlainText(QString::fromStdString(viewing_entry.getText()));

}
void JournalSystem::on_cancel_edit_btn_clicked(){
    ui->time_label2->clear();
    ui->date_label2->clear();
    ui->mood_label2->clear();
    ui->weather_label2->clear();
    ui->entry_edit2->clear();
    ui->stackedWidget->setCurrentIndex(2);
}
void JournalSystem::on_save_btn2_clicked(){
    QString userInput = ui->entry_edit2->toPlainText();
    std::string stduserInput = userInput.toStdString();
    Journal& current_journal = this->CurrentJournal();
    Entry& entry_edit = current_journal.GetEntries()[current_journal.getCurrentEntryIndex()];
    entry_edit.setText(stduserInput);
    ui->stackedWidget->setCurrentIndex(2);
    ui->journal_title3->setText(QString::fromStdString(this->CurrentJournal().getName()));
    ui->time_label->setText(entry_edit.getQTime().toString("hh:mm AP"));
    ui->date_label->setText(entry_edit.getQDate().toString("🗓yyyy-MM-dd"));
    ui->mood_label->setText(QString::fromStdString(entry_edit.getMood()));
    ui->weather_label->setText(QString::fromStdString(entry_edit.getWeather()));
    qDebug() << entry_edit.getText();
    ui->entry_input2->setPlainText(QString::fromStdString(entry_edit.getText()));

}

Journal& JournalSystem::CurrentJournal(){
    return this->journal_list_[this->current_journal_index_];
}
void JournalSystem::on_save_btn_clicked(){

    qDebug() << "Current journal index: " <<this->current_journal_index_;
    //Get user input
    std::string weather_input = ui->weather_entry->text().toStdString();
    std::string mood_input = ui->mood_entry->text().toStdString();
    std::string description_input = ui->description_entry->text().toStdString();
    std::string text_input = ui->user_entry->toPlainText().toStdString();

    //Create Entry
    Entry new_entry(text_input, mood_input, weather_input, description_input);
    qDebug() << "current journal name: "<< this->journal_list_[this->current_journal_index_].getName();
    this->CurrentJournal().addEntry(new_entry);
    qDebug() << "Size of Journal: "<<this->CurrentJournal().GetEntries().size();
    qDebug() << this->CurrentJournal().GetEntries()[0].getDescription().length();

    ui->stackedWidget->setCurrentIndex(2);
    this->current_page_index_ = 2;
    qDebug() << "running populate_entry_table";
    this->populate_entry_table();
}

bool JournalSystem::saveJournal(const QString &filePath, const QJsonDocument &jsonDoc, const QByteArray &password)
{
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    QByteArray encryptedData;
    try {
        encryptedData = CryptoUtils::encrypt(jsonData, password);
    } catch (const std::exception &e) {
        qWarning("Encryption failed: %s", e.what());
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Failed to open file for writing");
        return false;
    }

    file.write(encryptedData);
    file.close();
    return true;
}


bool JournalSystem::loadJournal(const QString &filePath, const QByteArray &password, QJsonDocument &outJsonDoc)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QByteArray encryptedData = file.readAll();
    file.close();

    QByteArray decryptedData;
    try {
        decryptedData = CryptoUtils::decrypt(encryptedData, password);
    } catch (const std::exception &e) {
        qWarning() << "Decryption failed:" << e.what();
        return false;
    }

    QJsonParseError parseError;
    outJsonDoc = QJsonDocument::fromJson(decryptedData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    return true;
}





void JournalSystem::closeEvent(QCloseEvent *event)
{
    // Only save if the user is logged in / password is set
    if (m_currentPassword.isEmpty()) {
        event->accept();
        return;
    }

    QJsonDocument jsonDoc = serializeJournalToJson();
    QByteArray password = m_currentPassword.toUtf8();

    if (!saveJournal(getJournalFilePath(), jsonDoc, password)) {
        QMessageBox::warning(this, "Save Error", "Failed to save your journal!");
        return; // prevent closing if desired
    }

    event->accept();
}

void JournalSystem::on_ConfirmBtn_clicked()

{
    QString password = ui->passwordLineEdit->text();
    QString confirmation_password = ui->confirmPasswordLineEdit->text();




    QFile file(getJournalFilePath());
    if (!file.exists()) {
        if (password.isEmpty() == true && confirmation_password.isEmpty() == true) {
            ui->confirmMsg->setVisible(true);
            ui->confirmMsg->setText("This section cannot be blank.");
            ui->passwordMsg->setVisible(true);
            ui->passwordMsg->setText("This section cannot be blank");
            //QMessageBox::warning(this, "Input Error", "Please enter a password.");
            return;
        }else if(password.isEmpty() == false && confirmation_password.isEmpty() == true ){
            ui->confirmMsg->setVisible(true);
            ui->confirmMsg->setText("Please confirm password.");
            ui->passwordMsg->setVisible(false);
            return;
        }else if(password.isEmpty() == true && confirmation_password.isEmpty() == false){
            return;
        }

        else{
            ui->confirmMsg->setVisible(false);
            ui->passwordMsg->setVisible(false);
        }

        if(password != confirmation_password){
            ui->confirmMsg->setVisible(true);
            ui->confirmMsg->setText("Passwords do not match");
            return;
        }else{
            //ui->confirmationLabel->setVisible(false);
        }
        if (password.length() < 8 ||
            !password.contains(QRegularExpression("[A-Z]")) ||      // at least one uppercase
            !password.contains(QRegularExpression("[a-z]")) ||      // at least one lowercase
            !password.contains(QRegularExpression("[0-9]")) ||      // at least one digit
            !password.contains(QRegularExpression("[^a-zA-Z0-9]"))) // at least one special character
        {
            QMessageBox::warning(this, "Weak Password",
                                 "Please use a stronger password:\n"
                                 "- At least 8 characters\n"
                                 "- Includes uppercase and lowercase letters\n"
                                 "- Includes numbers and special characters");
            return;
        }
        // First-time setup: accept this password, save empty journal structure
        m_currentPassword = password;

        QJsonDocument emptyDoc = serializeJournalToJson();  // Should return all journals as JSON
        if (!saveJournal(getJournalFilePath(), emptyDoc, password.toUtf8())) {
            QMessageBox::critical(this, "Error", "Failed to save the new encrypted journal.");
            return;
        }

        QMessageBox::information(this, "Setup Complete", "Your journal has been initialized and password set.");

        emit passwordAccepted();
        ui->stackedWidget->setCurrentIndex(1);
        this->populate_journal_table();
        //this->populate_entry_table();
        return;
    }
    if(password.isEmpty()){
        return;
    }

    // File exists: try to load and decrypt with provided password
    QJsonDocument loadedJson;
    if (!loadJournal(getJournalFilePath(), password.toUtf8(), loadedJson)) {
        QMessageBox::critical(this, "Error", "Wrong password or corrupted file.");
        ui->passwordLineEdit->clear();
        return;
    }

    // Success
    m_currentPassword = password;
    //authorized = true;
    deserializeJournalFromJson(loadedJson);  // Load multiple journals here

    emit passwordAccepted();
    ui->stackedWidget->setCurrentIndex(1);
    this->populate_journal_table();
    //this->populate_entry_table();
}


QJsonDocument JournalSystem::serializeJournalToJson() const
{
    QJsonArray journalsArray;

    for (const Journal &journal : journal_list_) {
        QJsonObject journalObj;
        journalObj["journalName"] = QString::fromStdString(journal.getName());  // or journal.name if public
        journalObj["journalDate"] = journal.getDate().toString("yyyy-MM-dd");

        QJsonArray entriesArray;
        for (const Entry &entry : journal.GetEntries()) {
            QJsonObject entryObj;
            entryObj["description"] = QString::fromStdString(entry.getDescription());
            entryObj["weather"] = QString::fromStdString(entry.getWeather());
            entryObj["mood"] = QString::fromStdString(entry.getMood());
            entryObj["date"] = entry.getQDate().toString("yyyy-MM-dd");
            entryObj["text"] = QString::fromStdString(entry.getText());
            entryObj["time"] = entry.getQTime().toString("hh:mm AP");

            entriesArray.append(entryObj);
        }

        journalObj["entries"] = entriesArray;
        journalsArray.append(journalObj);
    }

    QJsonObject root;
    root["journals"] = journalsArray;
    return QJsonDocument(root);
}

void JournalSystem::deserializeJournalFromJson(const QJsonDocument &doc)
{
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonArray journalsArray = root["journals"].toArray();

   journal_list_.clear();  // Assuming m_journals is a std::vector<Journal>

    for (const QJsonValue &jVal : journalsArray) {
        if (!jVal.isObject()) continue;
        QJsonObject journalObj = jVal.toObject();

        Journal journal;
        journal.setName(journalObj["journalName"].toString().toStdString());
        journal.setDate(QDate::fromString(journalObj["journalDate"].toString(), "yyyy-MM-dd"));


        QJsonArray entriesArray = journalObj["entries"].toArray();
        for (const QJsonValue &eVal : entriesArray) {
            if (!eVal.isObject()) continue;
            QJsonObject entryObj = eVal.toObject();

            Entry entry;
            entry.setDescription(entryObj["description"].toString().toStdString());
            entry.setText(entryObj["text"].toString().toStdString());
            entry.setQDate(QDate::fromString(entryObj["date"].toString(), "yyyy-MM-dd"));
            entry.setQTime(QTime::fromString(entryObj["time"].toString(), "hh:mm AP"));


            entry.setMood(entryObj["mood"].toString().toStdString());
            entry.setWeather(entryObj["weather"].toString().toStdString());

            journal.addEntry(entry);
        }

        journal_list_.push_back(journal);
    }
    qDebug() << journal_list_.size();


}
QString JournalSystem::getJournalFilePath() const
{
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataDir); // Ensure the directory exists
    return appDataDir + "/journal.dat";
}



