# Recipe Explorer - Native App

A native Qt6 application that allows users to browse, search, and manage cooking recipes.

Features:
- Home: Featured recipes with quick actions.
- Search: Query recipes by title or ingredient, with instant results and empty states.
- Recipe Details: Title, prep/cook times, ingredients, steps, and a Favorite toggle.
- Favorites: Persisted list of saved recipes (local QSettings).
- Theme: Ocean Professional (primary #2563EB, secondary/success #F59E0B, error #EF4444, background #f9fafb, surface #ffffff, text #111827) with rounded corners, subtle shadows, and smooth transitions.

Technology:
- Qt 6 (Core, Widgets)
- CMake build
- Local persistence via QSettings

Getting Started:
1. Prerequisites
   - Qt 6 with Core and Widgets modules (qt6-base-dev)
   - XKB libraries and headers (libxkbcommon-dev, libxkbcommon-x11-dev, libxcb-xkb-dev)
   - CMake >= 3.16
   - A C++17 compiler (e.g., GCC, Clang, MSVC)

2. Build
   - cd recipe-explorer-38527-38546/recipe_native_app
   - mkdir -p build && cd build
   - cmake .. -DCMAKE_PREFIX_PATH="<path to your Qt6>"
     Example: -DCMAKE_PREFIX_PATH="$HOME/Qt/6.5.3/gcc_64"
   - cmake --build . -j

3. Run
   - From build directory: ctest  (optional)
   - From build directory: ./MainApp
   - Or: cmake --build . --target run

Project Structure:
- recipe_native_app/
  - include/
    - mainApp.h: App entry point wrapper
    - main_window.h: Main window with tab navigation
    - views.h: Home, Search, Favorites, RecipeDetails views
    - repository.h, models.h: Data layer and model
    - theme.h: Ocean Professional theme constants
  - src/
    - mainApp.cpp: main() entry
    - main_window.cpp: Navigation and wiring
    - views.cpp: UI implementation
    - repository.cpp: Mock data and QSettings persistence
  - CMakeLists.txt

Accessibility and Responsiveness:
- Accessible names are applied to key controls for assistive technologies.
- Layouts use stretches and scalable fonts for responsive behavior across window sizes.
- Empty/error states: informative labels when no data or no search results.

Notes:
- Images are not loaded from the network in this minimal sample. Use imagePath field for extension if needed.
- Favorites are stored under QSettings org "RecipeExplorer", app "RecipeNativeApp".
