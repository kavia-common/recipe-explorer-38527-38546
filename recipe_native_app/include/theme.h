#ifndef THEME_H
#define THEME_H

#include <QString>
#include <QColor>
#include <QFont>

/**
 * PUBLIC_INTERFACE
 * Theme namespace provides the Ocean Professional style theme colors, fonts, and common style helpers.
 */
namespace Theme {
    // Colors
    static const QColor Primary("#2563EB");
    static const QColor Secondary("#F59E0B");
    static const QColor Success("#F59E0B");
    static const QColor Error("#EF4444");
    static const QColor Background("#f9fafb");
    static const QColor Surface("#ffffff");
    static const QColor Text("#111827");

    // Shadows and radii
    static const int RadiusSmall = 8;
    static const int RadiusMedium = 12;
    static const int RadiusLarge = 16;

    // Typography
    inline QFont headingFont(int size = 20, bool bold = true) {
        QFont f;
        f.setPointSize(size);
        f.setBold(bold);
        return f;
    }

    inline QFont bodyFont(int size = 12, bool bold = false) {
        QFont f;
        f.setPointSize(size);
        f.setBold(bold);
        return f;
    }

    // Common style sheets
    inline QString cardStyle() {
        return QString(
            "background:%1; border:1px solid rgba(0,0,0,0.06); border-radius:%2px; "
            "padding:12px; "
        ).arg(Surface.name()).arg(RadiusMedium);
    }

    inline QString pillButtonStyle(const QColor &bg, const QColor &fg = QColor("#ffffff")) {
        return QString(
            "QPushButton{background:%1;color:%2;border:none;border-radius:%3px;padding:8px 14px;}"
            "QPushButton:disabled{background:rgba(0,0,0,0.1);color:rgba(0,0,0,0.4);}"
            "QPushButton:hover{filter:brightness(0.98);}"
        ).arg(bg.name(), fg.name()).arg(RadiusLarge);
    }

    inline QString inputStyle() {
        return QString(
            "QLineEdit{background:%1;border:1px solid rgba(0,0,0,0.10);"
            "border-radius:%2px;padding:8px 10px;color:%3;}"
            "QLineEdit:focus{border:1px solid %4;}"
        ).arg(Surface.name()).arg(RadiusSmall).arg(Text.name()).arg(Primary.name());
    }

    inline QString listItemStyle() {
        return QString(
            "QFrame{background:%1;border:1px solid rgba(0,0,0,0.06);"
            "border-radius:%2px;}"
        ).arg(Surface.name()).arg(RadiusSmall);
    }
}

#endif // THEME_H
