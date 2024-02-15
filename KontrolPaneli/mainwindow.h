#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QSerialPort>
#include <QByteArray>







QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QTimer;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void stopBlinking();
    void updateLabel();
    void startBlinking();
    void on_stopBlinking_clicked();

    void stopBlinking2();
    void updateflame();
    void startBlinking2();
    void on_startBlinking2_clicked2();



    void on_pushButton_0derece_clicked();
    void on_pushButton_45derece_clicked();
    void on_pushButton_90derece_clicked();
    void updateServo(QString command);
    void on_horizontalSlider_valueChanged(int value);


    void on_pushButton_ok_clicked();

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_textChanged(const QString &arg1);
    void showTime();
    void readSerial();
    void updateTemperature(QString sensor_reading);

 void updateTemperatureFromSerial();
    void updateValues(double temperature, int gas, int fire, int voltaj, int Akim, int sivi, int nem);



    void on_frame_2_customContextMenuRequested(const QPoint &pos);





    void on_durdurma_clicked();

    void on_startBlinking2_clicked();

    void on_durdurma2_clicked();

    void on_startBlinking_clicked();

    void on_progressBar_valueChanged(int value);


private:
    QString servoAyar;
    Ui::MainWindow *ui;
    void updateClock();
    QTimer *timer;
    QTimer *flame;
     bool isBlinking;
    bool isBlinking2;





    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 6790;
    static const quint16 arduino_uno_product_id = 29987;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double sicaklik_degeri;

};
#endif // MAINWINDOW_H

