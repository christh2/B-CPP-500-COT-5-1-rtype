#ifndef JSONEDITOR_H
#define JSONEDITOR_H

#include <QWidget>

class JsonEditor : public QWidget {
    Q_OBJECT // Nécessaire pour les classes Qt utilisant les signaux et slots

public:
    explicit JsonEditor(QWidget *parent = nullptr); // Constructeur
    ~JsonEditor(); // Destructeur virtuel

    // Ajoutez des fonctions publiques si nécessaire

private:
    // Ajoutez vos variables membres ici

signals:
    // Ajoutez vos signaux ici si nécessaire

public slots:
    // Ajoutez vos slots ici si nécessaire
};

#endif // JSONEDITOR_H
