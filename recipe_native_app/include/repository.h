#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "models.h"
#include <QSettings>
#include <QSet>

/**
 * PUBLIC_INTERFACE
 * RecipeRepository provides in-app data with mock recipes and local persistence for favorites.
 */
class RecipeRepository {
public:
    RecipeRepository();

    // PUBLIC_INTERFACE
    QList<Recipe> allRecipes() const;
    // PUBLIC_INTERFACE
    QList<Recipe> featuredRecipes(int limit = 6) const;
    // PUBLIC_INTERFACE
    QList<Recipe> search(const QString &query) const;
    // PUBLIC_INTERFACE
    Recipe getById(const QString &id) const;

    // PUBLIC_INTERFACE
    bool isFavorite(const QString &id) const;
    // PUBLIC_INTERFACE
    void toggleFavorite(const QString &id);
    // PUBLIC_INTERFACE
    QList<Recipe> favorites() const;

private:
    QList<Recipe> m_recipes;
    QSettings m_settings;
    QSet<QString> m_favIds;

    void loadMockData();
    void loadFavorites();
    void saveFavorites();
};

#endif // REPOSITORY_H
