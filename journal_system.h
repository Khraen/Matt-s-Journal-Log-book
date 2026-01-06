#ifndef JOURNAL_SYSTEM_H
#define JOURNAL_SYSTEM_H

#include <QMainWindow>
#include <vector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "CryptoUtils.h"
#include "Journal.h"
#include<QString>





QT_BEGIN_NAMESPACE
namespace Ui {
class JournalSystem;
}
QT_END_NAMESPACE

class JournalSystem : public QMainWindow
{
    Q_OBJECT

public:
    JournalSystem(QWidget *parent = nullptr);
    ~JournalSystem();

    void show_journals()const;
    void populate_journal_table();
    bool saveJournal(const QString &filePath, const QJsonDocument &jsonDoc, const QByteArray &password);
    bool loadJournal(const QString &filePath, const QByteArray &password, QJsonDocument &outJsonDoc);
    void closeEvent(QCloseEvent *event);

    QJsonDocument serializeJournalToJson() const;
    Q_INVOKABLE void deserializeJournalFromJson(const QJsonDocument &doc);

    QString getJournalFilePath() const;
    //void populate_entry_table();

signals:
    void passwordAccepted();


    //void onTableRowClicked(int row, int column);

private slots:
    void on_add_journal_btn_clicked();
    void on_delete_journal_btn_clicked();
    void on_delete_entry_btn_clicked();
    void on_edit_btn_clicked();
    void on_cancel_edit_btn_clicked();
    void on_back_btn1_clicked();
    void on_back_btn2_clicked();
    void on_back_btn3_clicked();
    void onJournalTableRowDoubleClicked(int row, int column);
    void onEntryTableRowDoubleClicked(int row, int column);
    void onJournalViewButtonClicked(int row);
    void onEntryViewButtonClicked(int row);
    //void onPasswordEntered();
    void on_ConfirmBtn_clicked();
    void on_add_entry_btn_clicked();
    void on_save_btn_clicked();
    void on_save_btn2_clicked();
    void populate_entry_table();
    Journal& CurrentJournal();



private:
    Ui::JournalSystem *ui;
    std::vector<Journal> journal_list_;
    int current_journal_index_;
    int current_page_index_ = 0;
    QString m_currentPassword;
    bool authorized_ = false;




};
#endif // JOURNAL_SYSTEM_H
