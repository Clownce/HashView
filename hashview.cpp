#include "hashview.h"
#include "ui_hashview.h"

HashView::HashView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HashView)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    ui->textEdit->setReadOnly(true);//设为只读
    ui->textEdit->setAcceptDrops(false);
    ui->textEdit->append(tr("Drag and drop file here:"));
    setAcceptDrops(true);//支持拖拽
}

HashView::~HashView()
{
    delete ui;
}
//当用户拖动文件到窗口部件上，触发dragEnterEvent.
void HashView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))//如果为文件，则支持拖放。
    event->acceptProposedAction();
}
//当用户松开鼠标左键，触发dropEvent.
void HashView::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    ui->textEdit->clear();
    foreach(QUrl url,urls)
    {
        QString path=url.toLocalFile();
        QDir dir(path);//文件夹判断
        if(dir.exists())
        foreach(QString file,dir.entryList(QDir::Files))//读取文件夹内所有文件，但文件夹除外。
        {
            showFileInfo(QFileInfo(dir,file).filePath());
            ui->textEdit->append(showHash(QFileInfo(dir,file).filePath(),MD5));
            ui->textEdit->append(showHash(QFileInfo(dir,file).filePath(),SHA1));
            ui->textEdit->append(showHash(QFileInfo(dir,file).filePath(),SHA256));
        }
        else
        {
            showFileInfo(path);
            ui->textEdit->append(showHash(path,MD5));
            ui->textEdit->append(showHash(path,SHA1));
            ui->textEdit->append(showHash(path,SHA256));
        }
    }
}
//显示文件名、大小、日期
void HashView::showFileInfo(QString path)
{
    QFileInfo fileinfo(path);
    //文件名
    QString namestr(tr("Name: "));
    namestr+=fileinfo.fileName();
    ui->textEdit->append(namestr);
    //文件大小
    qint64 size=fileinfo.size();
    QString sizestr="Size: ",unit=" B";
    if(size>1024)
    {
        size/=1024;
        unit=" K";
        if(size>1024)
        {
            size/=1024;
            unit=" M";
            if(size>1024)
            {
                size/=1024;
                unit=" G";
                if(size>1024)
                {
                    size/=1024;
                    unit=" T";
                }
            }
        }
    }
    sizestr+=QString::number(size,10);
    sizestr+=unit;
    ui->textEdit->append(sizestr);
    //时间
    QDateTime time=fileinfo.lastModified();
    QString timestr="Date: ";
    timestr+=time.toString(Qt::LocalDate);
    ui->textEdit->append(timestr);
}
//显示Hash
QString HashView::showHash(QString path,HASH flag)
{
    QFile file(path);
    QString result;
    QByteArray BA;
    file.open(QIODevice::ReadOnly);
    if(flag==MD5)
    {
        result+="MD5: ";
        BA=QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
    }
    else if(flag==SHA1)
    {
        result+="SHA-1: ";
        BA=QCryptographicHash::hash(file.readAll(),QCryptographicHash::Sha1);
    }
    else if(flag==SHA256)
    {
        result+="SHA-256: ";
        BA=QCryptographicHash::hash(file.readAll(),QCryptographicHash::Sha256);
    }
    else if(flag==SHA512)
    {
        result+="SHA-512: ";
        BA=QCryptographicHash::hash(file.readAll(),QCryptographicHash::Sha512);
    }
    file.close();
    result+=BA.toHex().constData();
    return result;
}
