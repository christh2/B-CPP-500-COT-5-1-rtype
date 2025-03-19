#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initializeJsonStructure();
    void on_generateLevelJsonButton_clicked();  // Déclaration de la méthode pour gérer le clic du bouton

private slots:
    void on_addEntityButton_clicked();
    void on_addSpriteButton_clicked();
    void on_addTextButton_clicked();
    void on_saveJsonButton_clicked();
    void onApplyButtonClicked();   // Déclaration du slot
    void onPreviewButtonClicked(); // Déclaration du slot
    void on_addPositionButton_clicked();  // Ajoute cette ligne

private:
    Ui::MainWindow *ui;
    QJsonObject sceneJson;
    void applyStyle();  // Déclaration de la fonction applyStyle
    void updateJsonPreview();
};

#endif // MAINWINDOW_H
