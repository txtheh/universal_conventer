#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onConvertClicked();
    void onLoadFileClicked();
    void onSaveFileClicked();

private:
    Ui::MainWindow *ui;

    QString lastOutput;
    bool    lastTruncated = false;
};

#endif // MAINWINDOW_H