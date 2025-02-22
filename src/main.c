#include <raylib.h>

#define CELL_WIDTH 8

typedef enum GameMode { START = 0, END, GAME } GameMode;

typedef struct Cell {
  int x;
  int y;
} Cell;

void draw_grid(Cell cell, int cellWidth, Vector2 gameScreenPos) {
  Vector2 cellPos = {gameScreenPos.x + cell.x * cellWidth,
                     gameScreenPos.y + cell.y * cellWidth};
  Vector2 cellSize = {cellWidth, cellWidth};
  DrawRectangleV(cellPos, cellSize, WHITE);
}

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 800;
  const int gameScreenSide = 800;
  Vector2 gameScreenDimensions = {gameScreenSide, gameScreenSide};
  Vector2 gameScreenPos = {(screenWidth / 2) - (gameScreenSide / 2),
                           (screenHeight / 2) - (gameScreenSide / 2)};

  Cell grid[CELL_WIDTH][CELL_WIDTH];
  int cellWidth = gameScreenSide / 8;

  InitWindow(screenWidth, screenHeight, "Raychess");

  GameMode currentScreen = START;

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
      DrawText("Press Enter or Tap to BEGIN", 20, 20, 20, WHITE);
      DrawText("Press Q to EXIT app", 20, 40, 20, WHITE);
    } break;
    case END: {
      ClearBackground(BLACK);
      DrawText("Press Enter or Tap to PLAY AGAIN", 20, 20, 20, WHITE);
      DrawText("Press Q to EXIT app", 20, 40, 20, WHITE);
    } break;
    case GAME: {
      ClearBackground(RED);
      DrawRectangleV(gameScreenPos, gameScreenDimensions, BLACK);
      for (int i = 0; i < CELL_WIDTH; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) {
          draw_grid(grid[i][j], cellWidth, gameScreenPos);
        }
      }
      DrawText("ENTER to end game!", 20, 20, 20, WHITE);
    } break;
    default:
      break;
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
