/*  Toefl Speaking Practice
 *  Copyright (C) 2019  h6a8ke1k @ GitHub
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void prepStart(QMediaPlayer::State state);
    void updateTime();
    void allComplete();
    void on_pushButton_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int taskNum;
    QTimer *timer;
    int secRemain;
    int min;
    int sec;
    int secAll;
    int step;
    QAudioRecorder *recorder;
    QAudioEncoderSettings settings;
    QFile outputFile;
    void hideLayout();
    void showLayout();
    void beforePrep();
    void updatePrep();
    void updateAns();
    void startRecord();
    void stopRecord();
    bool checkAvai(int i);
};

#endif // MAINWINDOW_H
