#include "views.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QAccessible>
#include <QLabel>

RecipeListItem::RecipeListItem(const Recipe &recipe, QWidget *parent)
    : QFrame(parent), m_recipe(recipe)
{
    setObjectName("RecipeListItem");
    setAccessibleName("Recipe item");
    setStyleSheet(Theme::listItemStyle());
    auto *hl = new QHBoxLayout(this);

    auto *info = new QVBoxLayout();
    auto *title = new QLabel(recipe.title, this);
    title->setFont(Theme::headingFont(16, true));
    title->setStyleSheet(QString("color:%1;").arg(Theme::Text.name()));
    title->setAccessibleName("Recipe title");

    auto *sub = new QLabel(QString("Prep %1m • Cook %2m").arg(recipe.prepMinutes).arg(recipe.cookMinutes), this);
    sub->setFont(Theme::bodyFont(11));
    sub->setStyleSheet("color: rgba(17,24,39,0.75);");
    sub->setAccessibleName("Recipe times");

    info->addWidget(title);
    info->addWidget(sub);

    auto *openBtn = new QPushButton("View", this);
    openBtn->setAccessibleName("Open details");
    openBtn->setStyleSheet(Theme::pillButtonStyle(Theme::Primary));
    connect(openBtn, &QPushButton::clicked, this, [this]() {
        emit openDetails(m_recipe.id);
    });

    hl->addLayout(info, 1);
    hl->addWidget(openBtn, 0);
}

// HomeView
HomeView::HomeView(RecipeRepository *repo, QWidget *parent)
    : QWidget(parent), m_repo(repo)
{
    setAccessibleName("Home screen");
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);

    auto *title = new QLabel("Featured Recipes", this);
    title->setFont(Theme::headingFont(20, true));
    title->setStyleSheet(QString("color:%1;").arg(Theme::Text.name()));
    root->addWidget(title);

    m_layout = new QVBoxLayout();
    root->addLayout(m_layout, 1);

    refresh();
}

QWidget* HomeView::buildFeaturedSection(const QList<Recipe> &items) {
    auto *wrap = new QWidget(this);
    auto *vl = new QVBoxLayout(wrap);
    vl->setSpacing(10);

    if (items.isEmpty()) {
        auto *empty = new QLabel("No featured recipes available.", wrap);
        empty->setAccessibleName("Empty state");
        empty->setStyleSheet("color: rgba(17,24,39,0.7);");
        vl->addWidget(empty);
        return wrap;
    }

    for (const auto &r : items) {
        auto *card = new QFrame(wrap);
        card->setStyleSheet(Theme::cardStyle());
        auto *cl = new QVBoxLayout(card);

        auto *title = new QLabel(r.title, card);
        title->setFont(Theme::headingFont(16, true));
        cl->addWidget(title);

        auto *meta = new QLabel(QString("Prep %1m • Cook %2m").arg(r.prepMinutes).arg(r.cookMinutes), card);
        meta->setFont(Theme::bodyFont(11));
        meta->setStyleSheet("color: rgba(17,24,39,0.7);");
        cl->addWidget(meta);

        auto *row = new QHBoxLayout();
        auto *viewBtn = new QPushButton("View", card);
        viewBtn->setStyleSheet(Theme::pillButtonStyle(Theme::Primary));
        connect(viewBtn, &QPushButton::clicked, this, [this, r]() { emit openDetails(r.id); });

        auto *favBtn = new QPushButton(r.isFavorite ? "Unfavorite" : "Favorite", card);
        favBtn->setStyleSheet(Theme::pillButtonStyle(r.isFavorite ? Theme::Error : Theme::Secondary));
        connect(favBtn, &QPushButton::clicked, this, [this, r]() { emit toggleFavorite(r.id); });

        row->addWidget(viewBtn, 0);
        row->addWidget(favBtn, 0);
        row->addStretch(1);
        cl->addLayout(row);

        vl->addWidget(card);
    }

    return wrap;
}

QWidget* HomeView::buildListSection(const QString &titleText, const QList<Recipe> &items) {
    auto *wrap = new QWidget(this);
    auto *vl = new QVBoxLayout(wrap);
    auto *title = new QLabel(titleText, wrap);
    title->setFont(Theme::headingFont(18, true));
    vl->addWidget(title);

    if (items.isEmpty()) {
        auto *empty = new QLabel("No recipes available.", wrap);
        empty->setAccessibleName("Empty state");
        empty->setStyleSheet("color: rgba(17,24,39,0.7);");
        vl->addWidget(empty);
        return wrap;
    }

    for (const auto &r : items) {
        auto *item = new RecipeListItem(r, wrap);
        connect(item, &RecipeListItem::openDetails, this, [this](const QString &id){ emit openDetails(id); });
        vl->addWidget(item);
    }
    return wrap;
}

void HomeView::refresh() {
    // Clear layout
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    auto featured = m_repo->featuredRecipes(4);
    m_layout->addWidget(buildFeaturedSection(featured));
}

// SearchView
SearchView::SearchView(RecipeRepository *repo, QWidget *parent)
    : QWidget(parent), m_repo(repo)
{
    setAccessibleName("Search screen");
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);

    auto *title = new QLabel("Search Recipes", this);
    title->setFont(Theme::headingFont(20, true));
    root->addWidget(title);

    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Search by title or ingredient...");
    m_input->setAccessibleName("Search input");
    m_input->setStyleSheet(Theme::inputStyle());
    root->addWidget(m_input);

    m_stateLabel = new QLabel("Type to search recipes.", this);
    m_stateLabel->setStyleSheet("color: rgba(17,24,39,0.7); padding: 8px;");
    root->addWidget(m_stateLabel);

    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    auto *wrap = new QWidget(scroll);
    m_resultsLayout = new QVBoxLayout(wrap);
    m_resultsLayout->setSpacing(10);
    scroll->setWidget(wrap);
    root->addWidget(scroll, 1);

    connect(m_input, &QLineEdit::textChanged, this, &SearchView::onQueryChanged);
}

void SearchView::renderResults(const QList<Recipe> &items, const QString &emptyText) {
    // Clear
    QLayoutItem *child;
    while ((child = m_resultsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (items.isEmpty()) {
        m_stateLabel->setText(emptyText);
        return;
    } else {
        m_stateLabel->setText(QString("Found %1 result(s)").arg(items.size()));
    }

    for (const auto &r : items) {
        auto *card = new QFrame(this);
        card->setStyleSheet(Theme::cardStyle());
        auto *cl = new QHBoxLayout(card);

        auto *v = new QVBoxLayout();
        auto *t = new QLabel(r.title, card);
        t->setFont(Theme::headingFont(16, true));
        auto *m = new QLabel(QString("Prep %1m • Cook %2m").arg(r.prepMinutes).arg(r.cookMinutes), card);
        m->setStyleSheet("color: rgba(17,24,39,0.7);");
        v->addWidget(t);
        v->addWidget(m);

        auto *viewBtn = new QPushButton("View", card);
        viewBtn->setStyleSheet(Theme::pillButtonStyle(Theme::Primary));
        connect(viewBtn, &QPushButton::clicked, this, [this, r]() { emit openDetails(r.id); });

        cl->addLayout(v, 1);
        cl->addWidget(viewBtn, 0);

        m_resultsLayout->addWidget(card);
    }
}

void SearchView::onQueryChanged(const QString &q) {
    if (q.trimmed().isEmpty()) {
        renderResults({}, "Type to search recipes.");
        return;
    }
    auto res = m_repo->search(q);
    if (res.isEmpty()) {
        renderResults({}, "No recipes match your query.");
    } else {
        renderResults(res, "");
    }
}

void SearchView::refresh() {
    onQueryChanged(m_input->text());
}

// FavoritesView
FavoritesView::FavoritesView(RecipeRepository *repo, QWidget *parent)
    : QWidget(parent), m_repo(repo)
{
    setAccessibleName("Favorites screen");
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);

    auto *title = new QLabel("Favorites", this);
    title->setFont(Theme::headingFont(20, true));
    root->addWidget(title);

    m_layout = new QVBoxLayout();
    root->addLayout(m_layout, 1);

    refresh();
}

void FavoritesView::refresh() {
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    auto favs = m_repo->favorites();
    if (favs.isEmpty()) {
        auto *empty = new QLabel("No favorites yet. Tap the Favorite button on a recipe to save it here.", this);
        empty->setAccessibleName("Empty favorites state");
        empty->setStyleSheet("color: rgba(17,24,39,0.7);");
        m_layout->addWidget(empty);
        return;
    }

    for (const auto &r : favs) {
        auto *item = new RecipeListItem(r, this);
        connect(item, &RecipeListItem::openDetails, this, [this](const QString &id){ emit openDetails(id); });
        m_layout->addWidget(item);
    }
}

// RecipeDetailsView
RecipeDetailsView::RecipeDetailsView(const Recipe &recipe, RecipeRepository *repo, QWidget *parent)
    : QWidget(parent), m_recipe(recipe), m_repo(repo)
{
    setAccessibleName("Recipe details screen");
    rebuild();
}

void RecipeDetailsView::rebuild() {
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);

    auto *title = new QLabel(m_recipe.title, this);
    title->setFont(Theme::headingFont(22, true));
    title->setStyleSheet(QString("color:%1;").arg(Theme::Text.name()));
    root->addWidget(title);

    auto *meta = new QLabel(QString("Prep %1m • Cook %2m").arg(m_recipe.prepMinutes).arg(m_recipe.cookMinutes), this);
    meta->setStyleSheet("color: rgba(17,24,39,0.75);");
    root->addWidget(meta);

    auto *favRow = new QHBoxLayout();
    m_favButton = new QPushButton(m_repo->isFavorite(m_recipe.id) ? "Unfavorite" : "Favorite", this);
    m_favButton->setAccessibleName("Favorite toggle");
    m_favButton->setStyleSheet(Theme::pillButtonStyle(m_repo->isFavorite(m_recipe.id) ? Theme::Error : Theme::Secondary));
    connect(m_favButton, &QPushButton::clicked, this, &RecipeDetailsView::onToggleFavorite);
    favRow->addWidget(m_favButton);
    favRow->addStretch(1);
    root->addLayout(favRow);

    auto *sections = new QHBoxLayout();

    // Ingredients
    auto *ingCard = new QFrame(this);
    ingCard->setStyleSheet(Theme::cardStyle());
    auto *ingL = new QVBoxLayout(ingCard);
    auto *ingTitle = new QLabel("Ingredients", ingCard);
    ingTitle->setFont(Theme::headingFont(16, true));
    ingL->addWidget(ingTitle);
    if (m_recipe.ingredients.isEmpty()) {
        ingL->addWidget(new QLabel("No ingredients listed.", ingCard));
    } else {
        for (const auto &i : m_recipe.ingredients) {
            auto *lbl = new QLabel("• " + i, ingCard);
            lbl->setFont(Theme::bodyFont(12));
            ingL->addWidget(lbl);
        }
    }

    // Steps
    auto *stepsCard = new QFrame(this);
    stepsCard->setStyleSheet(Theme::cardStyle());
    auto *stepsL = new QVBoxLayout(stepsCard);
    auto *stepsTitle = new QLabel("Steps", stepsCard);
    stepsTitle->setFont(Theme::headingFont(16, true));
    stepsL->addWidget(stepsTitle);
    if (m_recipe.steps.isEmpty()) {
        stepsL->addWidget(new QLabel("No steps provided.", stepsCard));
    } else {
        int idx = 1;
        for (const auto &s : m_recipe.steps) {
            auto *lbl = new QLabel(QString::number(idx++) + ". " + s, stepsCard);
            lbl->setFont(Theme::bodyFont(12));
            stepsL->addWidget(lbl);
        }
    }

    sections->addWidget(ingCard, 1);
    sections->addWidget(stepsCard, 2);
    root->addLayout(sections, 1);
}

void RecipeDetailsView::onToggleFavorite() {
    m_repo->toggleFavorite(m_recipe.id);
    bool fav = m_repo->isFavorite(m_recipe.id);
    m_favButton->setText(fav ? "Unfavorite" : "Favorite");
    m_favButton->setStyleSheet(Theme::pillButtonStyle(fav ? Theme::Error : Theme::Secondary));
    emit toggledFavorite(m_recipe.id);
}
