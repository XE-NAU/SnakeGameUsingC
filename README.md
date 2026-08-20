# SnakeGameUsingC

A C++ Snake game built with [raylib](https://www.raylib.com/). This project uses simple object-oriented design to organize the game into separate classes for the snake, fruit, and game controller.

## Overview

This game is a C++ port of a Snake game implementation and keeps the structure organized around three main classes:

- `Snake` — controls the snake body, movement, direction, and snake sprites.
- `Fruit` — controls the fruit position and drawing.
- `Main` — controls game state, collision logic, score, rendering, and update flow.

The game uses:

- `raylib` for windowing, input, drawing, textures, and fonts
- C++ STL containers like `std::vector`
- a simple integer grid system for snake movement

## Features

- Grid-based snake movement
- Directional snake head and tail graphics
- Corner and straight body segment rendering
- Random fruit spawning
- Score display
- Wall collision and self-collision detection
- Timer-based movement instead of frame-based movement
- Keyboard controls with reversal prevention

## Project Structure

The main gameplay logic is in `snake.cpp`.

Expected assets are loaded from a `graphics/` folder relative to the executable:

- `apple.png`
- `head_up.png`
- `head_down.png`
- `head_left.png`
- `head_right.png`
- `tail_up.png`
- `tail_down.png`
- `tail_left.png`
- `tail_right.png`
- `body_horizontal.png`
- `body_vertical.png`
- `body_br.png`
- `body_bl.png`
- `body_tl.png`
- `body_tr.png`

## How the Game Works

### 1. Grid System

The game board is a `20 x 20` grid, and each cell is `40 x 40` pixels.

```cpp
const int cell_size = 40;
const int cell_number = 20;
```

This means the window size becomes:

- width: `20 * 40 = 800`
- height: `20 * 40 = 800`

Every snake and fruit position is stored in grid coordinates, not raw pixel coordinates.

### 2. Vec2 Utility

`Vec2` is a small integer vector type used instead of `pygame.Vector2`.

It supports:

- equality checks
- inequality checks
- addition
- subtraction

This makes movement and collision logic easier to read.

### 3. Snake Class

The `Snake` class handles the snake itself.

#### Main data members

- `body` — a `std::vector<Vec2>` containing all snake segments
- `direction` — current movement direction
- texture fields for head, tail, and body pieces
- `head` and `tail` — active textures selected based on direction

#### Constructor

The constructor:

- creates the initial snake body
- sets initial direction to the right
- loads all snake textures from the graphics folder
- assigns default head and tail textures

#### `unload()`

Releases all loaded textures to prevent memory leaks.

#### `create_snake()`

Draws the full snake on screen.

It:

- updates the head and tail graphics
- loops through each body segment
- draws the correct texture for:
  - head
  - tail
  - straight body segments
  - corner body segments

#### `add_body()`

Adds one segment to the front of the snake when food is eaten.

#### `move_snake()`

Moves the snake forward by:

- copying all body segments except the last
- inserting a new head position at the front
- reusing the existing body length

#### `update_graphics()`

Chooses the correct head and tail texture based on movement direction.

This is what gives the snake its visual orientation.

### 4. Fruit Class

The `Fruit` class manages the apple.

#### `pos`

Stores the fruit position in grid coordinates.

#### Constructor

Calls `randomize()` immediately so the fruit starts in a random location.

#### `create_fruit(Texture2D apple)`

Draws the apple texture on the board.

#### `randomize()`

Picks a new random grid cell using `GetRandomValue()`.

### 5. Main Class

The `Main` class acts as the game controller.

It owns:

- `Fruit fruit`
- `Snake snake`
- `Texture2D apple`
- `Font game_font`

#### Constructor

Loads the apple texture and sets the default font.

#### `unload()`

Cleans up the apple texture and snake textures.

#### `update()`

Runs game logic in order:

1. move the snake
2. check fruit collision
3. check wall/self collision

#### `draw_elements()`

Handles all visible elements:

- background grass
- fruit
- snake
- score

#### `check_collide()`

If the snake head touches the fruit:

- the fruit is randomized
- the snake grows by one block

#### `check_hit()`

Checks for losing conditions:

- hitting the right or bottom wall
- hitting the left or top wall
- colliding with itself
- fruit overlapping with the body is also corrected by re-randomizing the fruit

#### `game_over()`

Resets the snake back to its starting shape.

#### `draw_grass()`

Clears the screen with a light green background.

#### `display_score()`

Displays the score at the top-right area of the board.

The score is calculated as:

```cpp
snake.body.size() - 3
```

This works because the snake starts with 3 segments.

## Game Loop Process

The `main()` function is the runtime loop.

### Initialization

The game starts by:

- seeding randomness with the current time
- creating the window
- setting the target FPS to 60
- creating the `Main` game object

### Timer-based movement

Instead of moving every frame, the snake moves every `0.15` seconds.

That gives the game a consistent speed regardless of rendering frame rate.

### Input handling

Arrow keys control direction.

The game prevents direct reversal:

- if moving vertically, it blocks immediate vertical reversal
- if moving horizontally, it blocks immediate horizontal reversal

This avoids the snake turning into itself instantly.

### Drawing

Each frame:

- `BeginDrawing()` starts rendering
- `main_game.draw_elements()` draws the whole scene
- `EndDrawing()` finishes the frame

### Shutdown

When the window closes:

- textures are unloaded
- the window is closed
- the program exits cleanly

## Object-Oriented Design in This Project

This project uses OOP to split responsibilities cleanly.

### Encapsulation

Each class keeps its own data and behavior together:

- `Snake` owns snake movement and snake textures
- `Fruit` owns fruit placement and drawing
- `Main` owns gameplay flow

### Separation of Concerns

The code avoids putting everything inside `main()`.

That makes the program easier to:

- read
- debug
- expand
- maintain

### Reusability

Because logic is placed into methods, features can be changed without rewriting the whole game.

Examples:

- changing snake graphics
- changing update speed
- changing score display
- adding pause or restart features

## raylib Elements Used

This project uses several raylib features:

- `InitWindow()` — create the game window
- `CloseWindow()` — close the window
- `SetTargetFPS()` — limit rendering rate
- `WindowShouldClose()` — detect exit request
- `BeginDrawing()` / `EndDrawing()` — frame rendering
- `ClearBackground()` — set background color
- `DrawTexture()` — draw sprites
- `DrawTextEx()` — draw score text
- `MeasureTextEx()` — measure text width/height for alignment
- `LoadTexture()` — load image files
- `UnloadTexture()` — free image resources
- `GetFontDefault()` — use the default font
- `GetRandomValue()` — generate random fruit positions
- `GetTime()` — support timed movement
- `IsKeyPressed()` — read keyboard input
- `SetRandomSeed()` — initialize randomness

## How It Was Made From `snake.cpp`

Based on `snake.cpp`, the project was built in this order:

1. **Define the grid rules**
   - Set cell size and board size
   - Use grid coordinates for all logic

2. **Create a vector type**
   - Implement `Vec2` to manage positions and movement

3. **Build the `Snake` class**
   - Store body segments
   - Load snake textures
   - Handle movement and drawing

4. **Build the `Fruit` class**
   - Randomize position
   - Draw the apple sprite

5. **Build the `Main` class**
   - Control update flow
   - Handle collisions
   - Show score
   - Reset on game over

6. **Add the main game loop**
   - Initialize raylib
   - Process timer-based updates
   - Read keyboard input
   - Render every frame

7. **Add clean shutdown**
   - Unload textures
   - Close the window properly

## Controls

- `Up Arrow` — move up
- `Down Arrow` — move down
- `Left Arrow` — move left
- `Right Arrow` — move right

## Notes

- The game expects the texture files to exist in the `graphics/` directory.
- If you move the executable, you may need to adjust `GFX` in the source file.
- The snake grows when it eats fruit.
- Game over currently resets the snake instead of showing a separate game-over screen.

## License

Add your preferred license here if the repository does not already include one.
