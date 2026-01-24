#include "journal_system.h"
#include "CryptoUtils.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JournalSystem w;
    // ADD THESE LINES:
    QCoreApplication::setApplicationName("Diary");
    w.showFullScreen();



    return a.exec();
}
