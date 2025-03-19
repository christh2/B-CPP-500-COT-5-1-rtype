// #include <QApplication>
// #include <QWidget>
// #include <QPushButton>
// #include <QLineEdit>
// #include <QObject>

// int main(int argc, char **argv) {
//     QApplication app(argc, argv);

//     QWidget window;
//     window.setFixedSize(1000, 1000);

//     QPushButton *button = new QPushButton("Click me", &window);
//     button->setGeometry(100, 100, 200, 50);

//     QLineEdit *input = new QLineEdit(&window);
//     input->setPlaceholderText("Merci Hanniel");
//     input->setGeometry(200, 200, 300, 50);

//     QObject::connect(button, &QPushButton::clicked, [&]() {
//         QString newText = input->text();
//         if (!newText.isEmpty()) {
//             button->setText(newText);
//         }
//     });

//     window.show();

//     return app.exec();
// }


#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QListWidget>
#include <QLabel>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Game Studio");

    // Créer un splitter principal
    QSplitter *mainSplitter = new QSplitter;

    // Barre latérale gauche : Liste des scènes
    QListWidget *sceneList = new QListWidget;
    sceneList->addItems({"Cave", "Deep Space", "Deeper Underground", "Dream", "Launch Site", "Menu Page 1", 
                         "Menu Page 2", "Music House", "Parallax Example", "Path to Sample Town", 
                         "Player's House", "Sample Town", "Space Battle"});
    sceneList->setMinimumWidth(200);

    // Zone centrale : Prévisualisation
    QWidget *previewArea = new QWidget;
    QVBoxLayout *previewLayout = new QVBoxLayout(previewArea);

    QLabel *previewLabel = new QLabel("Preview Area");
    previewLabel->setStyleSheet("background-color: lightgray; border: 1px solid black; height: 300px;");
    previewLabel->setAlignment(Qt::AlignCenter);

    QLabel *previewDetails = new QLabel("Path to Sample Town\nS: 43/64    T: 1/30");
    previewDetails->setAlignment(Qt::AlignCenter);

    previewLayout->addWidget(previewLabel);
    previewLayout->addWidget(previewDetails);

    // Barre latérale droite : Propriétés
    QWidget *propertyArea = new QWidget;
    QVBoxLayout *propertyLayout = new QVBoxLayout(propertyArea);

    QLabel *propertyTitle = new QLabel("Parallax Example");
    propertyTitle->setStyleSheet("font-weight: bold; font-size: 16px;");
    propertyLayout->addWidget(propertyTitle);

    QFormLayout *formLayout = new QFormLayout;
    QComboBox *typeComboBox = new QComboBox;
    typeComboBox->addItems({"Platformer", "RPG", "Shooter"});
    formLayout->addRow("Type:", typeComboBox);

    QLabel *backgroundLabel = new QLabel("Background");
    backgroundLabel->setStyleSheet("font-weight: bold;");
    propertyLayout->addWidget(backgroundLabel);

    QPushButton *editBackgroundButton = new QPushButton("Edit Background");
    propertyLayout->addWidget(editBackgroundButton);

    QGroupBox *parallaxGroup = new QGroupBox("Parallax");
    QVBoxLayout *parallaxLayout = new QVBoxLayout(parallaxGroup);

    QLabel *parallaxInfo = new QLabel("3 Layers");
    parallaxLayout->addWidget(parallaxInfo);

    QFormLayout *layer1Layout = new QFormLayout;
    layer1Layout->addRow("H:", new QLineEdit);
    layer1Layout->addRow("Speed:", new QLineEdit);

    parallaxLayout->addLayout(layer1Layout);
    propertyLayout->addWidget(parallaxGroup);

    propertyLayout->addStretch();

    // Ajouter les widgets au splitter principal
    mainSplitter->addWidget(sceneList);
    mainSplitter->addWidget(previewArea);
    mainSplitter->addWidget(propertyArea);

    mainSplitter->setStretchFactor(1, 3); // Zone centrale plus large
    mainSplitter->setStretchFactor(2, 2);

    mainWindow.setCentralWidget(mainSplitter);
    mainWindow.resize(1000, 600);
    mainWindow.show();

    return app.exec();
}