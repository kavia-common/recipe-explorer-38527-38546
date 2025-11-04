#include "main_window.h"
#include "views.h"
#include "theme.h"

#include <QVBoxLayout>
#include <QTabBar>
#include <QStackedWidget>
#include <QToolBar>
#include <QAction>
#include <QScrollArea>
#include <QDialog>
#include <QScreen>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();
    setWindowTitle("Recipe Explorer");
    // Apply background color
    setStyleSheet(QString("QMainWindow{background:%1;}").arg(Theme::Background.name()));
}

void MainWindow::setupUi() {
    m_tabBar = new QTabBar(this);
    m_tabBar->addTab("Home");
    m_tabBar->addTab("Search");
    m_tabBar->addTab("Favorites");
    m_tabBar->setAccessibleName("Main navigation tabs");
    m_tabBar->setExpanding(true);

    QWidget *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(m_tabBar);

    m_stack = new QStackedWidget(central);
    layout->addWidget(m_stack, 1);

    m_homeView = new HomeView(&m_repo, m_stack);
    m_searchView = new SearchView(&m_repo, m_stack);
    m_favoritesView = new FavoritesView(&m_repo, m_stack);

    m_stack->addWidget(m_homeView);
    m_stack->addWidget(m_searchView);
    m_stack->addWidget(m_favoritesView);

    setCentralWidget(central);

    // Start at Home
    m_tabBar->setCurrentIndex(0);
    m_stack->setCurrentIndex(0);

    // Responsive initial size
    resize(900, 620);
}

void MainWindow::connectSignals() {
    connect(m_tabBar, &QTabBar::currentChanged, this, &MainWindow::onTabChanged);

    connect(m_homeView, &HomeView::openDetails, this, &MainWindow::onOpenDetails);
    connect(m_searchView, &SearchView::openDetails, this, &MainWindow::onOpenDetails);
    connect(m_favoritesView, &FavoritesView::openDetails, this, &MainWindow::onOpenDetails);

    connect(m_homeView, &HomeView::toggleFavorite, this, &MainWindow::onToggledFavorite);
    connect(m_searchView, &SearchView::toggleFavorite, this, &MainWindow::onToggledFavorite);
    connect(m_favoritesView, &FavoritesView::toggleFavorite, this, &MainWindow::onToggledFavorite);

    connect(this, &MainWindow::recipeFavoriteToggled, this, &MainWindow::onToggledFavorite);
}

void MainWindow::onTabChanged(int index) {
    m_stack->setCurrentIndex(index);
    // Refresh the visible view to reflect favorite changes and states
    switch (index) {
        case 0: m_homeView->refresh(); break;
        case 1: m_searchView->refresh(); break;
        case 2: m_favoritesView->refresh(); break;
    }
}

void MainWindow::openDetails(const QString &recipeId) {
    // Build details dialog
    auto recipe = m_repo.getById(recipeId);
    QDialog dlg(this);
    dlg.setWindowTitle(recipe.title);
    dlg.setModal(true);
    auto *layout = new QVBoxLayout(&dlg);

    auto *scroll = new QScrollArea(&dlg);
    scroll->setWidgetResizable(true);
    auto *view = new RecipeDetailsView(recipe, &m_repo, scroll);
    connect(view, &RecipeDetailsView::toggledFavorite, this, &MainWindow::onToggledFavorite);
    scroll->setWidget(view);

    layout->addWidget(scroll);
    auto *closeBtn = new QPushButton("Close", &dlg);
    closeBtn->setStyleSheet(Theme::pillButtonStyle(Theme::Primary));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);

    dlg.resize(700, 500);
    dlg.exec();
}

void MainWindow::onOpenDetails(const QString &recipeId) {
    openDetails(recipeId);
}

void MainWindow::onToggledFavorite(const QString & /*recipeId*/) {
    // Update all views to reflect new favorite state
    m_homeView->refresh();
    m_searchView->refresh();
    m_favoritesView->refresh();
}
