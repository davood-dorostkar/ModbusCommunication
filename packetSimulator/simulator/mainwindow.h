#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QQueue>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSerialPort comPort;
    QTimer timer;
    QByteArray _dataBuffer;
    uint8_t _sampleArray[8]= {0x64,0x04,0x00,0x01,0x00,0x00,0x69,0xff};
    QByteArray _samplePacket =QByteArray(reinterpret_cast<const char*>(_sampleArray), 8);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    uint8_t baudRate;
    void Respond(QByteArray request);
    QByteArray recievedData;
    QByteArray Array2QArray(uint8_t *input,int size);
    bool PacketIsValid(QByteArray data);
private slots:
    void readyRead();
    void ProcessPackets(QList<QByteArray> packets);
    void on_connectButton_clicked();
    void on_refreshButton_clicked();
    void on_disconnectButton_clicked();
    void on_saveButton_clicked();
    void on_clearButton_clicked();
    QByteArray ModbusCRC(QByteArray data);

private:
    Ui::MainWindow *ui;
    QByteArray buffer;
    int cursorPosition;
};

#endif // MAINWINDOW_H
