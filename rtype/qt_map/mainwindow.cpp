#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation de l'objet JSON
    sceneJson["entities"] = QJsonArray();
    sceneJson["sprites"] = QJsonArray();
    sceneJson["texts"] = QJsonArray();
    sceneJson["positions"] = QJsonArray();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addEntityButton_clicked()
{
    QString entityName = ui->entityNameLineEdit->text();
    if (!entityName.isEmpty()) {
        QJsonArray entities = sceneJson["entities"].toArray();
        entities.append(entityName);
        sceneJson["entities"] = entities;

        updateJsonPreview();
        ui->entityNameLineEdit->clear();
    }
}

void MainWindow::on_addSpriteButton_clicked()
{
    QString entity = ui->spriteEntityLineEdit->text();
    QString asset = ui->spriteAssetLineEdit->text();
    double scaleX = ui->spriteScaleXSpinBox->value();
    double scaleY = ui->spriteScaleYSpinBox->value();

    if (!entity.isEmpty() && !asset.isEmpty()) {
        QJsonObject sprite;
        sprite["entity"] = entity;
        sprite["asset"] = asset;

        QJsonObject scale;
        scale["x"] = scaleX;
        scale["y"] = scaleY;
        sprite["scale"] = scale;

        QJsonArray sprites = sceneJson["sprites"].toArray();
        sprites.append(sprite);
        sceneJson["sprites"] = sprites;

        updateJsonPreview();

        ui->spriteEntityLineEdit->clear();
        ui->spriteAssetLineEdit->clear();
        ui->spriteScaleXSpinBox->setValue(1.0);
        ui->spriteScaleYSpinBox->setValue(1.0);
    }
}

void MainWindow::on_addTextButton_clicked()
{
    QString entity = ui->textEntityLineEdit->text();
    QString content = ui->textContentLineEdit->text();
    QString font = ui->textFontLineEdit->text();
    int size = ui->textSizeSpinBox->value();
    int r = ui->textColorRSpinBox->value();
    int g = ui->textColorGSpinBox->value();
    int b = ui->textColorBSpinBox->value();

    if (!entity.isEmpty() && !content.isEmpty() && !font.isEmpty()) {
        QJsonObject text;
        text["entity"] = entity;
        text["content"] = content;
        text["font"] = font;
        text["size"] = size;

        QJsonObject color;
        color["r"] = r;
        color["g"] = g;
        color["b"] = b;
        text["color"] = color;

        QJsonArray texts = sceneJson["texts"].toArray();
        texts.append(text);
        sceneJson["texts"] = texts;

        updateJsonPreview();

        ui->textEntityLineEdit->clear();
        ui->textContentLineEdit->clear();
        ui->textFontLineEdit->clear();
        ui->textSizeSpinBox->setValue(12);
        ui->textColorRSpinBox->setValue(255);
        ui->textColorGSpinBox->setValue(255);
        ui->textColorBSpinBox->setValue(255);
    }
}

void MainWindow::on_saveJsonButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(sceneJson).toJson());
            file.close();
            QMessageBox::information(this, "Success", "JSON file saved successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save the file.");
        }
    }
}

void MainWindow::updateJsonPreview()
{
    QJsonDocument doc(sceneJson);
    ui->jsonPreviewTextEdit->setPlainText(doc.toJson(QJsonDocument::Indented));
}