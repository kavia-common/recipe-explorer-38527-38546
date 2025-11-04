#ifndef VIEWS_H
#define VIEWS_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include "repository.h"
#include "theme.h"

// Helper: recipe list item widget
class RecipeListItem : public QFrame {
    Q_OBJECT
public:
    explicit RecipeListItem(const Recipe &recipe, QWidget *parent = nullptr);

signals:
    void openDetails(const QString &recipeId);

private:
    Recipe m_recipe;
};

// PUBLIC_INTERFACE
class HomeView : public QWidget {
    Q_OBJECT
public:
    explicit HomeView(RecipeRepository *repo, QWidget *parent = nullptr);

signals:
    void openDetails(const QString &recipeId);
    void toggleFavorite(const QString &recipeId);

public slots:
    void refresh();

private:
    RecipeRepository *m_repo;
    QVBoxLayout *m_layout;
    QWidget *buildFeaturedSection(const QList<Recipe> &items);
    QWidget *buildListSection(const QString &title, const QList<Recipe> &items);
};

// PUBLIC_INTERFACE
class SearchView : public QWidget {
    Q_OBJECT
public:
    explicit SearchView(RecipeRepository *repo, QWidget *parent = nullptr);

signals:
    void openDetails(const QString &recipeId);
    void toggleFavorite(const QString &recipeId);

public slots:
    void refresh();

private slots:
    void onQueryChanged(const QString &q);

private:
    RecipeRepository *m_repo;
    QLineEdit *m_input;
    QVBoxLayout *m_resultsLayout;
    QLabel *m_stateLabel;

    void renderResults(const QList<Recipe> &items, const QString &emptyText);
};

// PUBLIC_INTERFACE
class FavoritesView : public QWidget {
    Q_OBJECT
public:
    explicit FavoritesView(RecipeRepository *repo, QWidget *parent = nullptr);

signals:
    void openDetails(const QString &recipeId);
    void toggleFavorite(const QString &recipeId);

public slots:
    void refresh();

private:
    RecipeRepository *m_repo;
    QVBoxLayout *m_layout;
};

// PUBLIC_INTERFACE
class RecipeDetailsView : public QWidget {
    Q_OBJECT
public:
    explicit RecipeDetailsView(const Recipe &recipe, RecipeRepository *repo, QWidget *parent = nullptr);

signals:
    void toggledFavorite(const QString &recipeId);

private slots:
    void onToggleFavorite();

private:
    Recipe m_recipe;
    RecipeRepository *m_repo;
    QPushButton *m_favButton;

    void rebuild();
};

#endif // VIEWS_H
