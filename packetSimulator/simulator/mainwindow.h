#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QFileDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSerialPort comPort;
    QTimer timer;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    uint8_t baudRate;
    void Respond(QByteArray request);
    QByteArray recievedData;
    QByteArray Array2QArray(uint8_t *input,int size);
private slots:
    void readyRead();
    void on_connectButton_clicked();
    void on_refreshButton_clicked();
    void on_disconnectButton_clicked();
//    void on_getSecondsButtons_clicked();
//    void on_getSampleButton_clicked();
//    void SendMessageSamples();
//    void SendMessageSeconds();
    void on_saveButton_clicked();
    void on_clearButton_clicked();
    QByteArray ModbusCRC(QByteArray data);

private:
    Ui::MainWindow *ui;
    QByteArray buffer;
//    int fullTimeMilliseconds;
//    int interval;
//    int samples;
//    bool isSampleButtonPressed;
    int cursorPosition;
};

#endif // MAINWINDOW_H
