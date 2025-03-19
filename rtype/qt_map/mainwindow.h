#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addEntityButton_clicked();
    void on_addSpriteButton_clicked();
    void on_addTextButton_clicked();
    void on_saveJsonButton_clicked();

private:
    Ui::MainWindow *ui;

    QJsonObject sceneJson; // Stockage du JSON complet

    void updateJsonPreview(); // Met à jour l'aperçu JSON dans l'interface
};

#endif // MAINWINDOW_H