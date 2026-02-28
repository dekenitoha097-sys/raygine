# World - Documentation

## Création d'un monde

```cpp
#include <raygine/world.hpp>

// Gravité par défaut (0, -10)
World world;

// Ou avec gravité personnalisée
World worldCustom(0.0f, -9.81f);
```

## Méthodes

### step(timeStep)
Avance la simulation physique.
```cpp
world.step(1.0f / 60.0f);  // 60 FPS
```

### getId()
Retourne le `b2WorldId` pour Box2D.
```cpp
b2WorldId id = world.getId();
```

---

## Body - Objets statiques et dynamiques

### Création d'un objet

```cpp
#include <raygine/body.hpp>

// Syntaxe complète
Body body(world.getId(), x, y, isDynamic, width, height, r, g, b);

// Exemples
Body dynamicBox(world.getId(), 100, 100, true);                           // défaut
Body dynamicBox(world.getId(), 100, 100, true, 40, 40, 0, 255, 0);       // personnalisé
Body staticGround(world.getId(), 400, 550, false, 800, 50, 100, 100, 100);
```

### Paramètres
- `worldId` - ID du monde (world.getId())
- `x, y` - Position initiale
- `isDynamic` - true = dynamique, false = statique
- `width, height` - Dimensions (défaut: 40x40)
- `r, g, b` - Couleur RGB (défaut: bleu)

### Méthodes

```cpp
body.draw();           // Dessine le corps
body.setVelocity(vx, vy);  // Définit la vitesse
body.applyForce(fx, fy);  // Applique une force
body.setPosition(x, y);    // Définit la position
body.getId();              // Retourne le b2BodyId
```

## Exemple complet

```cpp
#include <raygine/world.hpp>
#include <raygine/body.hpp>
#include <raylib.h>

int main() {
    InitWindow(800, 600, "Game");
    SetTargetFPS(60);

    World world(0.0f, -500.0f);
    
    Body player(world.getId(), 400, 100, true, 40, 40, 0, 100, 255);
    Body ground(world.getId(), 400, 550, false, 800, 50, 100, 100, 100);

    while (!WindowShouldClose()) {
        world.step(1.0f / 60.0f);

        if (IsKeyDown(KEY_LEFT)) player.setVelocity(-200, 0);
        if (IsKeyDown(KEY_RIGHT)) player.setVelocity(200, 0);
        if (IsKeyPressed(KEY_SPACE)) player.applyForce(0, -500);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        ground.draw();
        player.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```
