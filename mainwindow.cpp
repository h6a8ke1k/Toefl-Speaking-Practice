#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QDate>
#include <QTime>
#include <QAudioDeviceInfo>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer),
    taskNum(0),
    playlist(new QMediaPlaylist),
    timer(new QTimer),
    secRemain(0),
    min(0),
    sec(0),
    secAll(0),
    step(0),
    recorder(new QAudioRecorder),
    outputFile(new QFile)
{
    ui->setupUi(this);
    hideLayout();
    ui->label->setText("Waiting...");
    this->setWindowTitle("Toefl Speaking Practice");
    this->setFixedSize(this->width(), this->height());
    timer->setTimerType(Qt::PreciseTimer);
    ui->progressBar->setMaximum(180);
    QAudioEncoderSettings settings;
    settings.setCodec("audio/pcm");
    settings.setQuality(QMultimedia::HighQuality);
    settings.setChannelCount(1);
    settings.setSampleRate(44800);
    settings.setBitRate(32);
    settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    recorder->setEncodingSettings(settings, QVideoEncoderSettings(),
                                  "audio/x-wav");
    recorder->setAudioInput("default");
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(prepStart(QMediaPlayer::State)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(1)) return;
    taskNum = 1;
    ui->label->setText("Task 1");
    beforePrep();
}

void MainWindow::hideLayout()
{
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->progressBar->setVisible(false);
    //ui->checkBox->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
}

void MainWindow::showLayout()
{
    if (taskNum < 1 || taskNum > 6) {
        taskNum = 0;
        ui->label->setText("Error!");
        return;
    }
    if (taskNum <= 2) {
        ui->label_5->setText("15 seconds");
        ui->label_7->setText("45 seconds");
    } else if (taskNum <= 4) {
        ui->label_5->setText("30 seconds");
        ui->label_7->setText("60 seconds");
    } else {
        ui->label_5->setText("20 seconds");
        ui->label_7->setText("60 seconds");
    }
    ui->label_2->setText("Prepare your response");
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(true);
    ui->label_5->setVisible(true);
    ui->label_6->setVisible(true);
    ui->label_7->setVisible(true);
    ui->progressBar->setVisible(true);
    //ui->checkBox->setVisible(true);
    ui->pushButton_7->setVisible(true);
}

void MainWindow::beforePrep()
{
    if (step > 1) {
        QTimer::singleShot(1000, this, SLOT(allComplete()));
        ui->label_2->setText("Saving Recording...");
        recorder->stop();
        //outputFile.close();
        return;
    }
    playlist->clear();
    if (step == 0)
        playlist->addMedia(QUrl("qrc:/res/Speaking_Preparation_time.mp3"));
    else
        playlist->addMedia(QUrl("qrc:/res/Speaking_RecordingTime.mp3"));
    playlist->addMedia(QUrl("qrc:/res/Speaking_Beep.mp3"));
    playlist->setCurrentIndex(1);
    player->setPlaylist(playlist);
    player->play();
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->lineEdit->setEnabled(false);
}

void MainWindow::prepStart(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState) {
        showLayout();
        if (step == 0) updatePrep();
        else updateAns();
        ui->pushButton_7->setEnabled(true);
    }
}

void MainWindow::updatePrep()
{
    if (taskNum <= 2) secAll = 15;
    else if (taskNum <= 4) secAll = 30;
    else secAll = 20;
    secRemain = secAll + 1;
    updateTime();
    timer->start(1000);
}

void MainWindow::updateTime()
{
    secRemain--;
    min = secRemain / 60;
    sec = secRemain % 60;
    ui->label_3->setText(QString("%1:%2").arg(min, 2, 10, QChar('0'))
                         .arg(sec, 2, 10, QChar('0')));
    ui->progressBar->setValue((secAll - secRemain) * 180 / secAll);
    if (secRemain == 0) {
        timer->stop();
        step++;
        beforePrep();
    }
}

void MainWindow::updateAns()
{
    ui->label_2->setText("Recording...");
    if (taskNum <= 2) secAll = 45;
    else secAll = 60;
    secRemain = secAll + 1;
    updateTime();
    if (ui->checkBox->isChecked()) {
        QDir dir;
        if (!dir.exists("./record/")) dir.mkpath("./record/");
        /*QString localFile = "/record/" +
                QDate::currentDate().toString("yyyyMMdd") +
                QTime::currentTime().toString("hhmmss") + " - " +
                ui->label->text() + ".wav";*/
        QDir subDir("./record/" + ui->lineEdit->text());
        if (!subDir.exists())
            dir.mkpath("./record/" + ui->lineEdit->text());
        QString localFile = subDir.absoluteFilePath(ui->label->text() + ".wav");
        outputFile.setFileName(localFile);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        outputFile.close();
        recorder->setOutputLocation(QUrl::fromLocalFile(localFile));
        recorder->record();
        //qDebug()<<recorder->actualLocation();
    }
    timer->start(1000);
}

void MainWindow::allComplete()
{
    hideLayout();
    ui->label->setText("Waiting...");
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    step = 0;
    secAll = 0;
    secRemain = 0;
    sec = 0;
    min = 0;
    taskNum = 0;
}

void MainWindow::on_pushButton_7_clicked()
{
    timer->stop();
    recorder->stop();
    //outputFile.close();
    allComplete();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(2)) return;
    taskNum = 2;
    ui->label->setText("Task 2");
    beforePrep();
}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(3)) return;
    taskNum = 3;
    ui->label->setText("Task 3");
    beforePrep();
}

void MainWindow::on_pushButton_4_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(4)) return;
    taskNum = 4;
    ui->label->setText("Task 4");
    beforePrep();
}

void MainWindow::on_pushButton_5_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(5)) return;
    taskNum = 5;
    ui->label->setText("Task 5");
    beforePrep();
}

void MainWindow::on_pushButton_6_clicked()
{
    if (ui->checkBox->isChecked() && !checkAvai(6)) return;
    taskNum = 6;
    ui->label->setText("Task 6");
    beforePrep();
}

void MainWindow::on_pushButton_9_clicked()
{
    QMessageBox msg(QMessageBox::Information, "About",
                    "Toefl Speaking Practice v1.1\n"
                    "\nCopyright (C) 2019. h6a8ke1k @ GitHub."
                    "\nLicensed under GPL v3.\n"
                    "\nThis program comes with ABSOLUTELY NO WARRANTY."
                    "\nThis is free software, and you are welcome to redistribute it"
                    "\nunder certain conditions."
                    "\nYou should have received a copy of the GNU General Public License"
                    "\nalong with this program.  If not, see https://www.gnu.org/licenses/.",
                    QMessageBox::Ok);
    msg.exec();
}

void MainWindow::on_pushButton_8_clicked()
{
    if (checkAvai(0)) {
        QMessageBox msg(QMessageBox::Information, "Success",
                        "Successfully created directory!",
                        QMessageBox::Ok);
        msg.exec();
    }
}

bool MainWindow::checkAvai(int i)
{
    QDir dir;
    bool suc = true;
    if (!dir.exists("./record/")) dir.mkpath("./record/");
    QDir subDir("./record/" + ui->lineEdit->text());
    if (!subDir.exists())
        suc = dir.mkpath("./record/" + ui->lineEdit->text());
    if (!suc) {
        QMessageBox msg(QMessageBox::Warning, "Error",
                        "Failed to create directory!",
                        QMessageBox::Ok);
        msg.exec();
        return false;
    }
    subDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = subDir.entryInfoList();
    int file_count = list.count();
    if (i == 0) {
        if (file_count != 0) {
            QMessageBox msg(QMessageBox::Warning, "Warning",
                            "Target directory not empty!",
                            QMessageBox::Ok);
            msg.exec();
            return false;
        }
    } else {
        QFileInfo info(subDir.absoluteFilePath("Task " +
                                               QString::number(i, 10) +
                                               ".wav"));
        if (list.contains(info)) {
            QMessageBox msg(QMessageBox::Warning, "Warning",
                            "Target file already exists!\n"
                            "Do you want to override?",
                            QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes) return true;
            else return false;
        }
    }
    return true;
}
