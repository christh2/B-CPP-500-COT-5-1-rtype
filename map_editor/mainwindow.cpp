#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>


#include <QFile>
#include <QTextStream>
#include <QApplication>

void MainWindow::applyStyle()
{
    QFile file("style.css");  // Assurez-vous que le chemin est correct
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString style = stream.readAll();
        qApp->setStyleSheet(style);  // Applique le CSS à l'ensemble de l'application
        file.close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    applyStyle();  // Applique le style CSS

    // Connexion des boutons aux slots
    connect(ui->applyButton, &QPushButton::clicked, this, &MainWindow::onApplyButtonClicked);
    connect(ui->previewButton, &QPushButton::clicked, this, &MainWindow::onPreviewButtonClicked);
    connect(ui->addEntityButton, &QPushButton::clicked, this, &MainWindow::on_addEntityButton_clicked);
    connect(ui->addSpriteButton, &QPushButton::clicked, this, &MainWindow::on_addSpriteButton_clicked);
    connect(ui->addTextButton, &QPushButton::clicked, this, &MainWindow::on_addTextButton_clicked);
    connect(ui->addPositionButton, &QPushButton::clicked, this, &MainWindow::on_addPositionButton_clicked);
    connect(ui->saveJsonButton, &QPushButton::clicked, this, &MainWindow::on_saveJsonButton_clicked);

    // Initialisation de l'objet JSON avec des champs vides
    initializeJsonStructure();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeJsonStructure()
{
    sceneJson["entities"] = QJsonArray();
    sceneJson["sprites"] = QJsonArray();
    sceneJson["texts"] = QJsonArray();
    sceneJson["positions"] = QJsonArray();
    sceneJson["clickables"] = QJsonArray();
    sceneJson["colliders"] = QJsonArray();
    sceneJson["sounds"] = QJsonArray();
    sceneJson["inputs"] = QJsonArray();
    sceneJson["rectangles"] = QJsonArray();
    sceneJson["circles"] = QJsonArray();
    sceneJson["snacks"] = QJsonArray();
    sceneJson["spritesheets"] = QJsonArray();
    sceneJson["velocities"] = QJsonArray();
}

void MainWindow::on_addEntityButton_clicked()
{
    QString entityName = ui->entityNameLineEdit->text();
    QJsonArray entities = sceneJson["entities"].toArray();
    entities.append(entityName.isEmpty() ? "" : entityName);
    sceneJson["entities"] = entities;

    updateJsonPreview();
    ui->entityNameLineEdit->clear();
}

void MainWindow::on_addSpriteButton_clicked()
{
    QString entity = ui->spriteEntityLineEdit->text();
    QString asset = ui->spriteAssetLineEdit->text();
    double scaleX = ui->spriteScaleXSpinBox->value();
    double scaleY = ui->spriteScaleYSpinBox->value();

    QJsonObject sprite;
    sprite["entity"] = entity.isEmpty() ? "" : entity;
    sprite["asset"] = asset.isEmpty() ? "" : asset;
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

void MainWindow::on_addTextButton_clicked()
{
    QString entity = ui->textEntityLineEdit->text();
    QString content = ui->textContentLineEdit->text();
    QString font = ui->textFontLineEdit->text();
    int size = ui->textSizeSpinBox->value();
    int r = ui->textColorRSpinBox->value();
    int g = ui->textColorGSpinBox->value();
    int b = ui->textColorBSpinBox->value();

    QJsonObject text;
    text["entity"] = entity.isEmpty() ? "" : entity;
    text["content"] = content.isEmpty() ? "" : content;
    text["font"] = font.isEmpty() ? "" : font;
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

void MainWindow::on_addPositionButton_clicked()
{
    QString entity = ui->positionEntityLineEdit->text();
    int x = ui->positionXSpinBox->value();
    int y = ui->positionYSpinBox->value();

    QJsonObject position;
    position["entity"] = entity.isEmpty() ? "" : entity;
    position["x"] = x;
    position["y"] = y;

    QJsonArray positions = sceneJson["positions"].toArray();
    positions.append(position);
    sceneJson["positions"] = positions;

    updateJsonPreview();
    ui->positionEntityLineEdit->clear();
    ui->positionXSpinBox->setValue(0);
    ui->positionYSpinBox->setValue(0);
}

// void MainWindow::on_saveJsonButton_clicked()
// {
//     QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
//     if (!filePath.isEmpty()) {
//         QFile file(filePath);
//         if (file.open(QIODevice::WriteOnly)) {
//             file.write(QJsonDocument(sceneJson).toJson(QJsonDocument::Indented));
//             file.close();
//             QMessageBox::information(this, "Success", "JSON file saved successfully.");
//         } else {
//             QMessageBox::warning(this, "Error", "Failed to save the file.");
//         }
//     }
// }


void MainWindow::on_saveJsonButton_clicked()
{
    // Créer un nouvel objet JSON avec les sections dans l'ordre voulu
    QJsonObject orderedJson;

    // Ajouter les sections dans l'ordre spécifique
    orderedJson["entities"] = sceneJson["entities"];
    orderedJson["sprites"] = sceneJson["sprites"];
    orderedJson["texts"] = sceneJson["texts"];
    orderedJson["positions"] = sceneJson["positions"];
    orderedJson["clickables"] = sceneJson["clickables"];
    orderedJson["colliders"] = sceneJson["colliders"];
    orderedJson["sounds"] = sceneJson["sounds"];
    orderedJson["inputs"] = sceneJson["inputs"];
    orderedJson["rectangles"] = sceneJson["rectangles"];
    orderedJson["circles"] = sceneJson["circles"];
    orderedJson["snacks"] = sceneJson["snacks"];
    orderedJson["spritesheets"] = sceneJson["spritesheets"];
    orderedJson["velocities"] = sceneJson["velocities"];

    // Sauvegarder l'objet JSON dans un fichier
    QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(orderedJson).toJson(QJsonDocument::Indented));
            file.close();
            QMessageBox::information(this, "Success", "JSON file saved successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save the file.");
        }
    }
}

void MainWindow::onApplyButtonClicked()
{
    QMessageBox::information(this, "Apply", "Changes applied successfully.");
}

void MainWindow::onPreviewButtonClicked()
{
    QMessageBox::information(this, "Preview", "Previewing the changes.");
}

void MainWindow::updateJsonPreview()
{
    QJsonDocument doc(sceneJson);
    ui->jsonPreviewTextEdit->setPlainText(doc.toJson(QJsonDocument::Indented));
}
