#include "JsonEditor.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGraphicsTextItem>

#include "JsonEditor.h"

JsonEditor::~JsonEditor() {
    // Code de nettoyage ici
}


JsonEditor::JsonEditor(QWidget *parent) : QWidget(parent) {
    // Initialisation de l'interface
    //setupLayout();

    // Charger le fichier JSON
    loadJsonFile("help.json");
}

void JsonEditor::setupLayout() {
    // Créer la scène graphique et la vue
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Créer un bouton pour recharger le JSON
    reloadButton = new QPushButton("Recharger JSON", this);
    connect(reloadButton, &QPushButton::clicked, [this]() {
        scene->clear();  // Effacer la scène
        loadJsonFile("help.json"); // Recharger les données
    });

    // Disposer la vue et le bouton dans un layout vertical
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(reloadButton);
    setLayout(layout);
}

void JsonEditor::loadJsonFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier JSON !");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        QMessageBox::critical(this, "Erreur", "Le fichier JSON est invalide !");
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray entities = root["entities"].toArray();
    displayEntities(entities);
}

void JsonEditor::displayEntities(const QJsonArray &entities) {
    // Parcourir les entités et les afficher graphiquement
    for (const QJsonValue &value : entities) {
        QJsonObject entity = value.toObject();
        int x = entity["x"].toInt();
        int y = entity["y"].toInt();
        int width = entity["width"].toInt();
        int height = entity["height"].toInt();
        QString name = entity["name"].toString();

        // Ajouter un rectangle pour chaque entité
        QGraphicsRectItem *rect = scene->addRect(x, y, width, height, QPen(Qt::blue), QBrush(Qt::cyan));
        rect->setToolTip(name);

        // Ajouter le nom de l'entité comme texte
        QGraphicsTextItem *text = scene->addText(name);
        text->setPos(x, y - 20); // Position du texte
    }
}
