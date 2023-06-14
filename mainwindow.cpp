#include "mainwindow.h"
#include "./ui_mainwindow.h"

QString fileName;
int kbps = 16;
QProcess mTranscodingProcess;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open INPUT Video...", "./");
    ui->textBrowserFP->setText(fileName);
}


void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    kbps = position;
    ui->compressionLabel->setText(QString("%1kb/s").arg(kbps));
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->statusLabel->setText("Status: Converting...");
    QString program = "ffmpeg"; // or the path to ffmpeg
    QStringList arguments;
    QString outfile = QString("%1%2").arg(fileName.mid(0, (fileName.lastIndexOf('.')))).arg("-compressed.mp4");

    arguments << "-i" << fileName << "-vf" << "scale=320:-1,fps=10"
              << "-b:v" << QString("%1%2").arg(kbps).arg('k')
              << "-b:a" << QString("%1%2").arg(kbps).arg('k')  << outfile;

    qDebug() << arguments;

    QProcess *ffmpegProc = new QProcess(this); // this can be replaced by a valid parent
    ffmpegProc->start(program, arguments);

    if ( !(ffmpegProc->waitForFinished()) )
    {
        qDebug() << "Conversion failed:" << ffmpegProc->errorString();
        ui->statusLabel->setText("Status: Error!");
    }
    else
    {
        qDebug() << "Conversion output:" << ffmpegProc->readAll();
        ui->statusLabel->setText("Status: Done!");
    }


}

