#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "baseconverter.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->convert_but,  &QPushButton::clicked, this, &MainWindow::onConvertClicked);
    connect(ui->download_but, &QPushButton::clicked, this, &MainWindow::onLoadFileClicked);
    connect(ui->save_but,     &QPushButton::clicked, this, &MainWindow::onSaveFileClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConvertClicked() {
    ui->result_field->clear();
    ui->mistakes_field->clear();
    lastOutput = "";
    lastTruncated = false;

    QString inputStr = ui->input_num->text().trimmed();
    QString pStr     = ui->input_p->text().trimmed();
    QString qStr     = ui->input_q->text().trimmed();

    bool pOk, qOk;
    long long p = pStr.toLongLong(&pOk);
    long long q = qStr.toLongLong(&qOk);

    if (!pOk || p < 2 || p > 500) {
        ui->mistakes_field->setPlainText(
            "Ошибка: должно быть целым числом от 2 до 500.");
        return;
    }
    if (!qOk || q < 2 || q > 500) {
        ui->mistakes_field->setPlainText(
            "Ошибка: должно быть целым числом от 2 до 500.");
        return;
    }
    if (inputStr.isEmpty()) {
        ui->mistakes_field->setPlainText("Ошибка: введите число");
        return;
    }
    try {
        BaseConverter::Result res = BaseConverter::convert(
            inputStr.toStdString(), p, q);

        lastOutput    = QString::fromStdString(res.output);
        lastTruncated = res.truncated;

        ui->result_field->setPlainText(lastOutput);

        if (res.truncated) {
            ui->mistakes_field->setPlainText(
                "Много цифр");
        }
    } catch (const std::invalid_argument& e) {
        ui->mistakes_field->setPlainText(QString::fromStdString(e.what()));
    }
}

void MainWindow::onLoadFileClicked() {
    QString path = QFileDialog::getOpenFileName(
        this, "Загрузить файл", "", "Text files (*.txt);;All files (*)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->mistakes_field->setPlainText("Ошибка: не удалось открыть файл.");
        return;
    }

    QTextStream in(&file);
    QString pStr    = in.readLine().trimmed();
    QString qStr    = in.readLine().trimmed();
    QString inputStr = in.readLine().trimmed();
    file.close();

    ui->input_p->setText(pStr);
    ui->input_q->setText(qStr);
    ui->input_num->setText(inputStr);
}

void MainWindow::onSaveFileClicked() {
    if (lastOutput.isEmpty()) {
        ui->mistakes_field->setPlainText("Ошибка: нечего сохранять.");
        return;
    }

    QString path = QFileDialog::getSaveFileName(
        this, "Сохранить результат", "", "Text files (*.txt);;All files (*)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->mistakes_field->setPlainText("Ошибка: не удалось сохранить файл.");
        return;
    }

    QTextStream out(&file);
    out << lastOutput << "\n";
    if (lastTruncated)
        out << "Полная запись результата слишком длинная. Показаны первые 100000 символов.\n";
    file.close();
}
