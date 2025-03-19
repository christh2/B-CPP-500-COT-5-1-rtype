#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QString styleSheet = R"(
        /* CSS pour l'application entière */
        QMainWindow {
            background-color: #f0f0f0;
        }

        /* Style pour les boutons */
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            font-size: 16px;
        }

        QPushButton:hover {
            background-color: #45a049;
        }

        
    )";

    this->setStyleSheet(styleSheet);

    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::generateLevelFile);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::generateLevelFile() {
    int nbr_bonus = ui->nbrBonusSpinBox->value();
    int battle = ui->battleSpinBox->value();
    int papillon = ui->papillonSpinBox->value();
    int commandant = ui->commandantSpinBox->value();
    int alien = ui->alienSpinBox->value();
    int pique = ui->papillonSpinBox->value();
    int robot = ui->bossSpinBox->value();

    QString bossKey = ui->bossComboBox->currentText();
    int bossValue = ui->bossSpinBox->value();

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", "", "Fichiers JSON (*.json)");

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné !");
        return;
    }

    // Construire l'objet JSON
    QJsonObject levelObject;
    levelObject["nbr_bonus"] = nbr_bonus;
    levelObject["battle"] = battle;
    levelObject["papillon"] = papillon;
    levelObject["commandant"] = commandant;
    levelObject["alien"] = alien;
    levelObject["pique"] = pique;
    levelObject["robot"] = robot;
    levelObject[bossKey] = bossValue;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier !");
        return;
    }

    QJsonDocument doc(levelObject);
    file.write(doc.toJson());
    file.close();

    QMessageBox::information(this, "Succès", "Fichier JSON généré avec succès !");
}
