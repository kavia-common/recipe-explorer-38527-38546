#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QVariantMap>

/**
 * Recipe data model representing a cooking recipe with basic attributes.
 */
struct Recipe {
    QString id;
    QString title;
    QString imagePath; // For demo: local file path or URL string (not loaded from net)
    QStringList ingredients;
    QStringList steps;
    int prepMinutes = 0;
    int cookMinutes = 0;
    bool isFavorite = false;

    QVariantMap toVariant() const {
        QVariantMap m;
        m["id"] = id;
        m["title"] = title;
        m["imagePath"] = imagePath;
        m["ingredients"] = ingredients;
        m["steps"] = steps;
        m["prepMinutes"] = prepMinutes;
        m["cookMinutes"] = cookMinutes;
        m["isFavorite"] = isFavorite;
        return m;
    }

    static Recipe fromVariant(const QVariantMap &m) {
        Recipe r;
        r.id = m.value("id").toString();
        r.title = m.value("title").toString();
        r.imagePath = m.value("imagePath").toString();
        r.ingredients = m.value("ingredients").toStringList();
        r.steps = m.value("steps").toStringList();
        r.prepMinutes = m.value("prepMinutes").toInt();
        r.cookMinutes = m.value("cookMinutes").toInt();
        r.isFavorite = m.value("isFavorite").toBool();
        return r;
    }
};

#endif // MODELS_H
