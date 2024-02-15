int map(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QComboBox>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <qtextbrowser.h>
#include <QTextBrowser>
#include <QTime>
#include <QPixmap>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    isBlinking(false),
    isBlinking2(false)



{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    QTimer *flame = new QTimer (this);

    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(timer, &QTimer::timeout, this, &MainWindow::updateLabel);
    connect(flame, &QTimer::timeout, this, &MainWindow::updateflame);
    timer->start(100);
    flame->start(100);

    showTime();


    ui->lcdNumber->display("-----");
    arduino = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    sicaklik_degeri = 0.0;


    qDebug() << "Portların numarası: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Tanım: " << serialPortInfo.description() << "\n";
            qDebug() << "vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Urun id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Urun ID: " << serialPortInfo.productIdentifier() << "\n";

    }

    bool arduino_is_available = false;
    QString arduino_uno_port_name=("COM8");

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){

        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){

            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true;
                arduino_uno_port_name = serialPortInfo.portName();
            }
        }
    }
    if (arduino_is_available) {
        qDebug() << "Arduino portu bulundu...\n";
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    } else {
        qDebug() << "Arduino icin dogru port bulunamadi\n";
        QMessageBox::information(this, "Seri port hatasi", "Arduino'da seri port acilamadi.");
    }

}
void MainWindow::updateflame()
{
    if (isBlinking2) {

      static bool visible1 = true;
        ui->label_5->setVisible(visible1);
        visible1 = !visible1;
    } else {
       ui->label_5->setVisible(true);
    }
}
void MainWindow::updateLabel()
{
    if (isBlinking) {
     static bool visible2 = true;
       ui->label_10->setVisible(visible2);
       visible2 = !visible2;
    } else {
        ui->label_10->setVisible(true);
    }
}
void MainWindow::startBlinking()
{
    isBlinking = true;
    ui->startBlinking->setEnabled(true);
}
void MainWindow::startBlinking2()
{
    isBlinking2 = true;
    ui->startBlinking2->setEnabled(true);
}
void MainWindow::stopBlinking()
{
    isBlinking = false;

    ui->startBlinking->setEnabled(true);
}
void MainWindow::stopBlinking2()
{
    isBlinking2 = false;
    ui->startBlinking2->setEnabled(true);
}
void MainWindow::on_startBlinking_clicked()
{
    startBlinking();
}
void MainWindow::on_startBlinking2_clicked()
{
    startBlinking2();
}
void MainWindow::on_durdurma_clicked()
{
    stopBlinking();
}
void MainWindow::on_durdurma2_clicked()
{
    stopBlinking2();
}
void MainWindow::showTime()
{   QTime time=QTime::currentTime();
    QString time_text=time.toString(" hh:mm:ss");
    ui->saat_gostergesi->setText(time_text);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString date_text= currentDateTime.toString("dd.MM.yyyy");
    ui->zaman_gostergesi->setText(date_text);

}
MainWindow::~MainWindow()
{
    delete ui;
    if(arduino->isOpen()){
        arduino->close(); //
    }
}

void MainWindow::on_pushButton_0derece_clicked()
{
    ui->horizontalSlider->setValue(180);
    servoAyar = "180";
    ui->label_der_gosterge->setText(servoAyar);
    updateServo(servoAyar);

    QString textFromLineEdit = ui->pushButton_45derece->text();
    ui->label_der_gosterge->setText(textFromLineEdit);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");

    QString currentText = ui->textBrowser->toHtml(); // Mevcut metni al
    ui->textBrowser->setText(currentText + "\nServo 0 dereceye ayarlandi " + formattedDateTime);

}

void MainWindow::on_pushButton_45derece_clicked()
{
    ui->horizontalSlider->setValue(45);
    servoAyar = "45";
    ui->label_der_gosterge->setText(servoAyar);
    updateServo(servoAyar);

    QString textFromLineEdit = ui->pushButton_45derece->text();
    ui->label_der_gosterge->setText(textFromLineEdit);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");

    QString currentText = ui->textBrowser->toHtml(); // Mevcut metni al
    ui->textBrowser->setText(currentText + "\nServo 45 dereceye ayarlandi " + formattedDateTime);

}

void MainWindow::on_pushButton_90derece_clicked()
{
    ui->horizontalSlider->setValue(90);
    servoAyar = "90";
    ui->label_der_gosterge->setText(servoAyar);
    updateServo(servoAyar);

    QString textFromLineEdit =ui -> pushButton_90derece->text();
    ui->label_der_gosterge->setText(textFromLineEdit);
    QDateTime currentDateTime= QDateTime::currentDateTime();
    QString formattedDateTime=currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");
    QString currentText=ui->textBrowser->toHtml();
    ui->textBrowser->setText(currentText + "\n Servo 90 dereceye ayarlandi"+ formattedDateTime);

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QString kaydirma=QString::number(value);
    ui->label_der_gosterge->setText(kaydirma);
}
void MainWindow::updateServo(QString command)
{
    if (arduino->isWritable())
    {
        qDebug() << command.toStdString().c_str();
        arduino->write(command.toStdString().c_str());
    }
    else
    {
        qDebug() << "Arduino portu hatasi";
    }
}

void MainWindow::on_pushButton_ok_clicked()

{



    QString textFromLineEdit = ui->lineEdit->text();
    ui->label_der_gosterge->setText(textFromLineEdit);

    bool conversionOk;
    int servoderecesi = textFromLineEdit.toInt(&conversionOk);

    ui->horizontalSlider->setValue(servoderecesi);
    servoAyar = QString::number(servoderecesi);
    ui->label_der_gosterge->setText(servoAyar);
    updateServo(servoAyar);


    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");

    QString deger= ui->lineEdit->text();
    QString currentText = ui->textBrowser->toHtml();
    ui->textBrowser->setText( currentText + "\nServo " + deger + " dereceye ayarlandi " + formattedDateTime);
}
void MainWindow::readSerial()
{
    QStringList buffer_split = serialBuffer.split(",");

    if(buffer_split.length() < 8) {
        serialData = arduino->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    } else {
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[0];
        sicaklik_degeri= parsed_data.toDouble();
        qDebug() << "Sicaklik: " << sicaklik_degeri << "\n";

        parsed_data = buffer_split[2];
        int gaz_degeri = parsed_data.toInt();
        qDebug() << "Gaz Degeri: " << gaz_degeri << "\n";

        parsed_data = buffer_split[3];
        int yangin_degeri = parsed_data.toInt();
        qDebug() << "Yangin Degeri: " << yangin_degeri << "\n";


        parsed_data=buffer_split[4];
        double voltaj_degeri=parsed_data.toDouble();
        qDebug() <<"Voltaj Degeri:"<< voltaj_degeri<<"\n";

        parsed_data=buffer_split [5];
        double akim_degeri=parsed_data.toDouble();
        qDebug() <<"Akim Degeri:" << akim_degeri << "\n";

        parsed_data = buffer_split[6];
        int suseviye_degeri = parsed_data.toInt();
        qDebug() << "Sıvı Seviyesi " << suseviye_degeri << "\n";
        int progressValue = map(suseviye_degeri, 0, 494, 0, 100);
        ui->progressBar->setValue(progressValue);

       parsed_data=buffer_split[7];
        int nem_degeri=parsed_data.toDouble();
        qDebug() << "Nem Seviyesi" << nem_degeri<< "\n";

        updateValues(sicaklik_degeri, yangin_degeri, gaz_degeri , voltaj_degeri, akim_degeri,suseviye_degeri,nem_degeri);
        ui->label_11->setText(QString("\nTemp:%1 \n Gas: %2 \n Flame: %3\n Voltaj: %4 \n Akim: %5 \n Sivi: ")
                                  .arg(sicaklik_degeri).arg(gaz_degeri).arg(yangin_degeri).arg(voltaj_degeri).
                              arg(akim_degeri).arg(suseviye_degeri));

        ui->lcdNumber_volt->display(voltaj_degeri );
        ui->lcdNumber_Akim->display(akim_degeri);
        ui->lcdNumber_nem->display(nem_degeri);

        double guc = (voltaj_degeri)*(akim_degeri);
        ui->lcdNumber_Guc->display(guc);

        if (gaz_degeri > 110 ) {
            on_startBlinking_clicked();
            QString currentText = ui->textBrowser->toHtml();
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString formattedDateTime = currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");
            ui->textBrowser->setText(currentText + "\n ! GAZ ALGILANDI ! " + formattedDateTime);
        }
        else
        {
            on_durdurma_clicked();
        }
        if ( yangin_degeri > 990 ) {
            on_startBlinking2_clicked();
            QString currentText = ui->textBrowser->toHtml();
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString formattedDateTime = currentDateTime.toString("[dd.MM.yyyy hh:mm:ss]");
            ui->textBrowser->setText(currentText + "\n ! YANGIN ALGILANDI ! " + formattedDateTime);         // Gaz değeri 500'ün üstündeyse label_5'i görünür yap
        }
        else
        {
            on_durdurma2_clicked();}
    }
}


void MainWindow::updateValues(double temperature, int gas, int fire, int voltaj, int Akim, int sivi, int nem)
{
    // Sıcaklık değerini güncelle
    parsed_data = QString::number(temperature, 'g', 4);
    ui->lcdNumber->display(parsed_data);

    parsed_data=QString::number(voltaj, 'g', 4);
    ui->lcdNumber_volt->display(parsed_data);

    parsed_data=QString::number(Akim, 'g', 4);
    ui->lcdNumber_Akim->display(parsed_data);



}


