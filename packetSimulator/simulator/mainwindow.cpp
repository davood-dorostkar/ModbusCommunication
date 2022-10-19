#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datalist.h"
#include "QThread"

static QList<QSerialPortInfo> portList;
dataList PacketList;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    portList = QSerialPortInfo::availablePorts();
    for (int i=0;i<portList.length();i++)
        ui->portBox->addItem(portList[i].portName());
}
// =========================================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
// =========================================================================================================================
void MainWindow::readyRead()
{
    QByteArray readBuffer;
    readBuffer = comPort.readAll();
    if(readBuffer.count()== 6) Respond(readBuffer);
    readBuffer = NULL;
    comPort.flush();


//    qDebug()<<"HEX: "<<data.toHex();
//    qDebug()<<"raw: "<<data;
//    qDebug()<<data;
    //buffer.append("\n");
//    buffer.append(data);
//    ui->monitorBox->insert("\r\n");
//    int length = sizeof (recievedData)/sizeof (int8_t);
//    qDebug()<<length;
//    if(length<6)
//    {
//        recievedData.append((const char*)(data), sizeof(int8_t));
//    }
//    else if(length==6){
//    ui->monitorBox->insert(data);
//    Respond(data);
//    recievedData = NULL;
//    }
//    else
//    {recievedData = NULL;}
//        ui->monitorBox->insert("***");
//        ui->monitorBox->insert(data.toHex());
//        Respond(data);
//        recievedData = NULL;

}
// =========================================================================================================================
void MainWindow::Respond(QByteArray request)
{
    int relayRowCount = sizeof(PacketList.RelayRequests) / sizeof(PacketList.RelayRequests[0]);
    int mainboardRowCount = sizeof(PacketList.MainboardRequests) / sizeof(PacketList.MainboardRequests[0]);
    for(int i=0;i<relayRowCount;i++){
        int elementLength = sizeof(PacketList.RelayRequests[i]) / sizeof(uint8_t);
        QByteArray element = Array2QArray(PacketList.RelayRequests[i],elementLength);
        if (request == element)
        {
            QByteArray response = Array2QArray(PacketList.RelayResponses[i],elementLength);
            QByteArray responseWithCRC = ModbusCRC(response);
            qDebug()<<"resp: "<<responseWithCRC.toHex();
            ui->monitorBox->append("Relay Request: "+request.toHex()+", Response: "+responseWithCRC.toHex());
            comPort.write(responseWithCRC);
        }
    }
    for(int i=0;i<mainboardRowCount;i++){
        int elementLength = sizeof(PacketList.MainboardRequests[i]) / sizeof(uint8_t);
        QByteArray element = Array2QArray(PacketList.MainboardRequests[i],elementLength);
        if (request == element)
        {
            QByteArray response = Array2QArray(PacketList.MainboardResponses[i],elementLength);
            QByteArray responseWithCRC = ModbusCRC(response);
            qDebug()<<"resp: "<<responseWithCRC.toHex();
            ui->monitorBox->append("Mainboard Request: "+request.toHex()+", Response: "+responseWithCRC.toHex());
            comPort.write(responseWithCRC);
        }
    }
}
// =========================================================================================================================
QByteArray MainWindow::Array2QArray(uint8_t *input, int size)
{
    QByteArray result;
    for (int i = 0; i < size; i++)
    {
        result.append((const char*)(input+i), sizeof(uint8_t));
    }
    return  result;
}
// =========================================================================================================================
void MainWindow::on_connectButton_clicked()
{
    comPort.setPortName(ui->portBox->currentText());// ttyUSB0
    comPort.setBaudRate(ui->baudBox->currentText().toInt());
    comPort.setDataBits(QSerialPort::Data8);
    //comPort.setParity(QSerialPort::NoParity);
    //comPort.setStopBits(QSerialPort::OneStop);
    //comPort.setFlowControl(QSerialPort::);
    connect(&comPort,SIGNAL(readyRead()),this,SLOT(readyRead()));
    bool result= comPort.open(QSerialPort::ReadWrite);
    if (!result)
        ui->statusBar->showMessage("Open Port Error!");
    else
    {
        ui->statusBar->showMessage("connected successfully");
        buffer.clear();
    }
}
// =========================================================================================================================
void MainWindow::on_disconnectButton_clicked()
{
    if (comPort.isOpen())
        comPort.close();
    buffer.clear();
    ui->statusBar->showMessage("disconnected successfully");
}
// =========================================================================================================================
void MainWindow::on_refreshButton_clicked()
{
    portList = QSerialPortInfo::availablePorts();
    for (int i=0;i<portList.length();i++)
        ui->portBox->addItem(portList[i].portName());
}
// =========================================================================================================================
//void MainWindow::on_getSampleButton_clicked()
//{
//    buffer.clear();
//    timer.disconnect();
//    samples = ui->samplesBox->value();
//    interval=ui->intervalBox->value();
//    timer.start(interval);
//    connect(&timer,SIGNAL(timeout()),this,SLOT(SendMessageSamples()));
//    //isSampleButtonPressed = true;
//}
// =========================================================================================================================
//void MainWindow::on_getSecondsButtons_clicked()
//{
//    buffer.clear();
//    timer.disconnect();
//    fullTimeMilliseconds=ui->secondsBox->value();
//    interval=ui->intervalBox->value();
//    timer.start(interval);
//    connect(&timer,SIGNAL(timeout()),this,SLOT(SendMessageSeconds()));
//    //isSampleButtonPressed = false;
//}
// =========================================================================================================================
//void MainWindow::SendMessageSamples()
//{
//    comPort.write("QR\r\n");
//    samples--;
//    if (samples<=0)
//        timer.stop();
//}
// =========================================================================================================================
//void MainWindow::SendMessageSeconds()
//{
//    comPort.write("QR\r\n");
//    fullTimeMilliseconds -= interval;
//    if (fullTimeMilliseconds<=0)
//        timer.stop();
//}
// =========================================================================================================================
void MainWindow::on_saveButton_clicked()
{
    QFileDialog *dlg = new QFileDialog();
    // dlg->exec();
    QString saveURL=dlg->getSaveFileName();
    qDebug() << "save dialog"<<saveURL;
    //    if (dlg->getSaveFileName()!=nullptr)
    //        return;
    QFile file(saveURL);
    //file.write(buffer);
    if (!file.open(QFile::ReadWrite))
        return;
    file.write(buffer);
    file.close();
}
// =========================================================================================================================
void MainWindow::on_clearButton_clicked()
{
    ui->monitorBox->clear();
    buffer = NULL;
}
// =========================================================================================================================
QByteArray MainWindow::ModbusCRC(QByteArray data)
{
        unsigned int crc = 0xFFFF;
        int pos=0;
        int i;
        for (pos = 0; pos < data.length(); pos++)
        {
            crc ^= ((unsigned int)data[pos]&0x00FF);          // XOR byte into least sig. byte of crc

            for (i = 8; i != 0; i--)
            {    // Loop over each bit
                if ((crc & 0x0001) != 0)
                {      // If the LSB is set
                    crc >>= 1;                    // Shift right and XOR 0xA001
                    crc ^= 0xA001;
                }
                else                            // Else LSB is not set
                    crc >>= 1;                    // Just shift right
            }
        }
        data.append(crc&0xff);
        data.append((crc&0xff00)>>8);

        // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
        return data;

}
