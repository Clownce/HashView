#ifndef HASHVIEW_H
#define HASHVIEW_H
//#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QTextEdit>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFile>

namespace Ui {
class HashView;
}

class HashView : public QMainWindow
{
    Q_OBJECT

public:
    explicit HashView(QWidget *parent = 0);
    ~HashView();

private:
    Ui::HashView *ui;
    enum HASH{MD5,SHA1,SHA256,SHA512,CRC32};
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void showFileInfo(QString path);
    QString showHash(QString path,HASH flag);
};

#endif // HASHVIEW_H
