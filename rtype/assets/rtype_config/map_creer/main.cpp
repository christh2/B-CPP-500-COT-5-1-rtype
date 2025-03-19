#include <QApplication>
#include "JsonEditor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    JsonEditor editor;  // Créer l'éditeur
    editor.show();      // Afficher l'interface graphique

    return app.exec();
}
