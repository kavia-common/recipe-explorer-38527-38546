#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabBar>
#include <QToolBar>
#include <QAction>
#include "repository.h"

class HomeView;
class SearchView;
class FavoritesView;
class RecipeDetailsView;

/**
 * PUBLIC_INTERFACE
 * MainWindow hosts the top-level navigation (tabs) and coordinates detail navigation.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    RecipeRepository m_repo;
    QTabBar *m_tabBar;
    QStackedWidget *m_stack;

    HomeView *m_homeView;
    SearchView *m_searchView;
    FavoritesView *m_favoritesView;

    void setupUi();
    void connectSignals();
    void openDetails(const QString &recipeId);

signals:
    void recipeFavoriteToggled(const QString &recipeId);

private slots:
    void onTabChanged(int index);
    void onOpenDetails(const QString &recipeId);
    void onToggledFavorite(const QString &recipeId);
};

#endif // MAIN_WINDOW_H
