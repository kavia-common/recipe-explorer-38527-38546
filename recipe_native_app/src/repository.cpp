#include "repository.h"
#include <algorithm>

RecipeRepository::RecipeRepository()
    : m_settings("RecipeExplorer", "RecipeNativeApp")
{
    loadMockData();
    loadFavorites();
}

void RecipeRepository::loadMockData() {
    m_recipes.clear();

    Recipe r1;
    r1.id = "classic_pancakes";
    r1.title = "Classic Pancakes";
    r1.imagePath = ""; // Placeholder: No image loading in this minimal app
    r1.ingredients = {"1 1/2 cups flour", "3 1/2 tsp baking powder", "1 tsp salt", "1 tbsp sugar", "1 1/4 cups milk", "1 egg", "3 tbsp butter, melted"};
    r1.steps = {"Mix dry ingredients.", "Whisk in milk, egg, and butter.", "Cook on griddle until golden."};
    r1.prepMinutes = 10;
    r1.cookMinutes = 15;

    Recipe r2;
    r2.id = "spaghetti_bolognese";
    r2.title = "Spaghetti Bolognese";
    r2.ingredients = {"200g spaghetti", "250g minced beef", "1 onion", "2 cloves garlic", "400g tomato sauce", "Salt & pepper"};
    r2.steps = {"Cook spaghetti.", "Sauté onion and garlic.", "Brown beef.", "Add tomato sauce.", "Simmer and serve."};
    r2.prepMinutes = 15;
    r2.cookMinutes = 30;

    Recipe r3;
    r3.id = "avocado_toast";
    r3.title = "Avocado Toast";
    r3.ingredients = {"2 slices bread", "1 ripe avocado", "Salt, pepper", "Chili flakes (optional)"};
    r3.steps = {"Toast bread.", "Mash avocado.", "Season and spread."};
    r3.prepMinutes = 5;
    r3.cookMinutes = 2;

    Recipe r4;
    r4.id = "grilled_chicken_salad";
    r4.title = "Grilled Chicken Salad";
    r4.ingredients = {"Chicken breast", "Mixed greens", "Cherry tomatoes", "Cucumber", "Olive oil", "Lemon juice"};
    r4.steps = {"Grill chicken.", "Chop veggies.", "Toss with dressing."};
    r4.prepMinutes = 20;
    r4.cookMinutes = 15;

    Recipe r5;
    r5.id = "chocolate_chip_cookies";
    r5.title = "Chocolate Chip Cookies";
    r5.ingredients = {"2 1/4 cups flour", "1 tsp baking soda", "1 tsp salt", "1 cup butter", "3/4 cup sugar", "2 eggs", "2 cups chocolate chips"};
    r5.steps = {"Preheat oven.", "Mix ingredients.", "Bake until golden."};
    r5.prepMinutes = 15;
    r5.cookMinutes = 12;

    m_recipes = {r1, r2, r3, r4, r5};
}

void RecipeRepository::loadFavorites() {
    const QStringList ids = m_settings.value("favorites", QStringList{}).toStringList();
    m_favIds = QSet<QString>(ids.begin(), ids.end());
    // Mark favorite flags
    for (auto &r : m_recipes) {
        r.isFavorite = m_favIds.contains(r.id);
    }
}

void RecipeRepository::saveFavorites() {
    m_settings.setValue("favorites", QStringList(m_favIds.begin(), m_favIds.end()));
}

QList<Recipe> RecipeRepository::allRecipes() const {
    return m_recipes;
}

QList<Recipe> RecipeRepository::featuredRecipes(int limit) const {
    QList<Recipe> copy = m_recipes;
    if (limit > 0 && copy.size() > limit) {
        copy = copy.mid(0, limit);
    }
    return copy;
}

QList<Recipe> RecipeRepository::search(const QString &query) const {
    if (query.trimmed().isEmpty()) {
        return {};
    }
    const QString q = query.trimmed().toLower();
    QList<Recipe> out;
    for (const auto &r : m_recipes) {
        const bool inTitle = r.title.toLower().contains(q);
        bool inIngredients = false;
        for (const auto &ing : r.ingredients) {
            if (ing.toLower().contains(q)) { inIngredients = true; break; }
        }
        if (inTitle || inIngredients) out.push_back(r);
    }
    return out;
}

Recipe RecipeRepository::getById(const QString &id) const {
    for (const auto &r : m_recipes) {
        if (r.id == id) return r;
    }
    return {};
}

bool RecipeRepository::isFavorite(const QString &id) const {
    return m_favIds.contains(id);
}

void RecipeRepository::toggleFavorite(const QString &id) {
    if (m_favIds.contains(id)) {
        m_favIds.remove(id);
    } else {
        m_favIds.insert(id);
    }
    saveFavorites();
}

QList<Recipe> RecipeRepository::favorites() const {
    QList<Recipe> f;
    for (const auto &r : m_recipes) {
        if (m_favIds.contains(r.id)) {
            Recipe rr = r;
            rr.isFavorite = true;
            f.push_back(rr);
        }
    }
    return f;
}
