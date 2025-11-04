#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <QApplication>
#include "main_window.h"

/**
 * PUBLIC_INTERFACE
 * Entrypoint: constructs QApplication and shows MainWindow.
 */
class App {
public:
    // PUBLIC_INTERFACE
    static int run(int argc, char *argv[]) {
        QApplication app(argc, argv);
        MainWindow w;
        w.show();
        return app.exec();
    }
};

#endif // MAIN_APP_H