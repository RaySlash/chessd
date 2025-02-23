#include <raylib.h>

#define FONT_SIZE 20
#define GAME_SCREEN_SIZE 720
#define ROWS 8
#define COLUMNS 8

typedef enum GameMode { START = 0, END, GAME } GameMode;

typedef struct Cell {
  int x;
  int y;
} Cell;

// Grid Textures
Texture2D squareBrownTexture;
Texture2D squareGrayTexture;

void load_textures() {
  Image squareBrownImage = LoadImage("src/assets/square_brown_dark.png");
  squareBrownTexture = LoadTextureFromImage(squareBrownImage);
  UnloadImage(squareBrownImage);

  Image squareGrayImage = LoadImage("src/assets/square_gray_dark.png");
  squareGrayTexture = LoadTextureFromImage(squareGrayImage);
  UnloadImage(squareGrayImage);
};

void draw_grid(Cell cell, int cellWidth, Vector2 gameScreenPos) {
  Texture2D texture;
  if ((cell.x + cell.y) % 2 == 0) { // Alternating textures
    texture = squareBrownTexture;
  } else {
    texture = squareGrayTexture;
  }

  Vector2 position = {gameScreenPos.x + cell.x * cellWidth,
                      gameScreenPos.y + cell.y * cellWidth};

  float scale = (float)cellWidth / texture.width;

  DrawTextureEx(texture, position, 0.0f, scale, WHITE);
};

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 800;
  Vector2 gameScreenDimensions = {GAME_SCREEN_SIZE, GAME_SCREEN_SIZE};
  Vector2 gameScreenPos = {(screenWidth / 2) - (GAME_SCREEN_SIZE / 2),
                           (screenHeight / 2) - (GAME_SCREEN_SIZE / 2)};

  Cell grid[COLUMNS][ROWS];
  int cellWidth = GAME_SCREEN_SIZE / 8;
  GameMode currentScreen = START;

  InitWindow(screenWidth, screenHeight, "Raychess");
  load_textures();

  // For the game screen in which the game will be drawn
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (currentScreen) {
    case START: {
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = GAME;
      }
      if (IsKeyPressed(KEY_Q)) {
        return 0;
      }
    } break;
    case END: {
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = START;
      }
      if (IsKeyPressed(KEY_Q)) {
        return 0;
      }
    } break;
    case GAME: {
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = END;
      }
    } break;
    default:
      break;
    }
    BeginDrawing();

    switch (currentScreen) {
    case START: {
      ClearBackground(BLACK);
      DrawText("Press Enter or Tap to BEGIN", 20, 20, FONT_SIZE, WHITE);
      DrawText("Press Q to EXIT app", 20, 40, FONT_SIZE, WHITE);
    } break;
    case END: {
      ClearBackground(BLACK);
      DrawText("Press Enter or Tap to PLAY AGAIN", 20, 20, FONT_SIZE, WHITE);
      DrawText("Press Q to EXIT app", 20, 40, FONT_SIZE, WHITE);
    } break;
    case GAME: {
      ClearBackground(BLACK);
      for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
          grid[i][j] = (Cell){i, j};
          draw_grid(grid[i][j], cellWidth, gameScreenPos);
        };
      };
      DrawText("ENTER to end game!", 20, 20, FONT_SIZE, WHITE);
    } break;
    default:
      break;
    }
    EndDrawing();
  }

  UnloadTexture(squareBrownTexture);
  UnloadTexture(squareGrayTexture);
  CloseWindow();
  return 0;
};
