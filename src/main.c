#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define FONT_SIZE 20
#define GAME_SCREEN_SIZE 720
#define ROWS 8
#define COLUMNS 8
#define MAX_PIECES 32

typedef enum GameMode { MENU = 0, GAME } GameMode;

typedef struct {
  int x;
  int y;
} Cell;

typedef struct {
  Texture2D darkCell;
  Texture2D lightCell;
  Texture2D blackKing;
  Texture2D blackQueen;
  Texture2D blackPawn;
  Texture2D blackBishop;
  Texture2D blackKnight;
  Texture2D blackRook;
  Texture2D whiteKing;
  Texture2D whiteQueen;
  Texture2D whitePawn;
  Texture2D whiteBishop;
  Texture2D whiteKnight;
  Texture2D whiteRook;
} Textures;

typedef enum { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } PieceType;

typedef struct {
  PieceType type;
  Vector2 position;
  bool isWhite;
  Texture2D texture;
} Piece;

typedef struct {
  Piece pieces[MAX_PIECES];
  int pieceCount;
  Textures textures;
} GameState;

GameState store;

Texture2D texture_from_image(const char *path) {
  Image image = LoadImage(path);
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image);
  return texture;
};

void load_textures() {
  store.textures.darkCell =
      texture_from_image("src/assets/square_brown_light.png");
  store.textures.lightCell =
      texture_from_image("src/assets/square_gray_light.png");
  store.textures.blackKing = texture_from_image("src/assets/b_king.png");
  store.textures.blackQueen = texture_from_image("src/assets/b_queen.png");
  store.textures.blackPawn = texture_from_image("src/assets/b_pawn.png");
  store.textures.blackBishop = texture_from_image("src/assets/b_bishop.png");
  store.textures.blackKnight = texture_from_image("src/assets/b_knight.png");
  store.textures.blackRook = texture_from_image("src/assets/b_rook.png");

  store.textures.whiteKing = texture_from_image("src/assets/w_king.png");
  store.textures.whiteQueen = texture_from_image("src/assets/w_queen.png");
  store.textures.whitePawn = texture_from_image("src/assets/w_pawn.png");
  store.textures.whiteBishop = texture_from_image("src/assets/w_bishop.png");
  store.textures.whiteKnight = texture_from_image("src/assets/w_knight.png");
  store.textures.whiteRook = texture_from_image("src/assets/w_rook.png");
};

void unload_textures() {
  UnloadTexture(store.textures.darkCell);
  UnloadTexture(store.textures.lightCell);
  UnloadTexture(store.textures.blackKing);
  UnloadTexture(store.textures.blackQueen);
  UnloadTexture(store.textures.blackPawn);
  UnloadTexture(store.textures.blackBishop);
  UnloadTexture(store.textures.blackKnight);
  UnloadTexture(store.textures.blackRook);
  UnloadTexture(store.textures.whiteKing);
  UnloadTexture(store.textures.whiteQueen);
  UnloadTexture(store.textures.whitePawn);
  UnloadTexture(store.textures.whiteBishop);
  UnloadTexture(store.textures.whiteKnight);
  UnloadTexture(store.textures.whiteRook);
};

bool is_move_valid(Piece piece, Vector2 targetPos) {
  if (targetPos.x < 0 || targetPos.x >= COLUMNS || targetPos.y < 0 ||
      targetPos.y >= ROWS)
    return false;

  for (int i = 0; i < store.pieceCount; i++) {
    if (Vector2Equals(store.pieces[i].position, targetPos) &&
        store.pieces[i].isWhite == store.pieces[i].isWhite) {
      return false;
    }
  }

  switch (piece.type) {
  case PAWN: {
    if (piece.isWhite) {
      return (targetPos.y == piece.position.y - 1) &&
             (targetPos.x == piece.position.x);
    } else {
      return (targetPos.y == piece.position.y + 1) &&
             (targetPos.x == piece.position.x);
    }
  }
  case ROOK: {
    return (targetPos.x == piece.position.x || targetPos.y == piece.position.y);
  }
  case BISHOP: {
  }
  case KNIGHT: {
  }
  case QUEEN: {
  }
  case KING: {
  }
  default:
    return false;
  }
}

void init_pieces() {
  for (int i = 0; i < COLUMNS; i++) {
    store.pieces[store.pieceCount++] =
        (Piece){PAWN, (Vector2){i, 6}, true, store.textures.whitePawn};
    store.pieces[store.pieceCount++] =
        (Piece){PAWN, (Vector2){i, 1}, false, store.textures.blackPawn};
  }

  store.pieces[store.pieceCount++] =
      (Piece){.type = KING,
              .position = (Vector2){4, 7},
              .isWhite = true,
              .texture = store.textures.whiteKing};
  store.pieces[store.pieceCount++] =
      (Piece){.type = KING,
              .position = (Vector2){4, 0},
              .isWhite = false,
              .texture = store.textures.blackKing};
  store.pieces[store.pieceCount++] =
      (Piece){.type = QUEEN,
              .position = (Vector2){3, 7},
              .isWhite = true,
              .texture = store.textures.whiteQueen};
  store.pieces[store.pieceCount++] =
      (Piece){.type = QUEEN,
              .position = (Vector2){3, 0},
              .isWhite = false,
              .texture = store.textures.blackQueen};
  store.pieces[store.pieceCount++] =
      (Piece){ROOK, (Vector2){0, 7}, true, store.textures.whiteRook};
  store.pieces[store.pieceCount++] =
      (Piece){ROOK, (Vector2){7, 7}, true, store.textures.whiteRook};
  store.pieces[store.pieceCount++] =
      (Piece){ROOK, (Vector2){0, 0}, false, store.textures.blackRook};
  store.pieces[store.pieceCount++] =
      (Piece){ROOK, (Vector2){7, 0}, false, store.textures.blackRook};

  store.pieces[store.pieceCount++] =
      (Piece){KNIGHT, (Vector2){1, 7}, true, store.textures.whiteKnight};
  store.pieces[store.pieceCount++] =
      (Piece){KNIGHT, (Vector2){6, 7}, true, store.textures.whiteKnight};
  store.pieces[store.pieceCount++] =
      (Piece){KNIGHT, (Vector2){1, 0}, false, store.textures.blackKnight};
  store.pieces[store.pieceCount++] =
      (Piece){KNIGHT, (Vector2){6, 0}, false, store.textures.blackKnight};

  store.pieces[store.pieceCount++] =
      (Piece){BISHOP, (Vector2){2, 7}, true, store.textures.whiteBishop};
  store.pieces[store.pieceCount++] =
      (Piece){BISHOP, (Vector2){5, 7}, true, store.textures.whiteBishop};
  store.pieces[store.pieceCount++] =
      (Piece){BISHOP, (Vector2){2, 0}, false, store.textures.blackBishop};
  store.pieces[store.pieceCount++] =
      (Piece){BISHOP, (Vector2){5, 0}, false, store.textures.blackBishop};
}

void draw_grid(Cell cell, int cellWidth, Vector2 gameScreenPos) {
  Texture2D texture;
  if ((cell.x + cell.y) % 2 == 0) { // Alternating textures
    texture = store.textures.darkCell;
  } else {
    texture = store.textures.lightCell;
  }

  Vector2 position = {gameScreenPos.x + cell.x * cellWidth,
                      gameScreenPos.y + cell.y * cellWidth};
  float scale = (float)cellWidth / texture.width;
  DrawTextureEx(texture, position, 0.0f, scale, WHITE);
};

void draw_pieces(int cellWidth, Vector2 gameScreenPos) {
  for (int i = 0; i < store.pieceCount; i++) {
    Piece p = store.pieces[i];
    Vector2 screenPos = {
        (gameScreenPos.x + (cellWidth / 4)) + p.position.x * cellWidth,
        (gameScreenPos.y + (cellWidth / 4)) + p.position.y * cellWidth};
    float scale = ((float)cellWidth / p.texture.width) / 2;
    DrawTextureEx(p.texture, screenPos, 0.0f, scale, WHITE);
  }
}

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 800;
  Vector2 gameScreenDimensions = {GAME_SCREEN_SIZE, GAME_SCREEN_SIZE};
  Vector2 gameScreenPos = {(screenWidth / 2) - (GAME_SCREEN_SIZE / 2),
                           (screenHeight / 2) - (GAME_SCREEN_SIZE / 2)};

  Cell grid[COLUMNS][ROWS];
  int cellWidth = GAME_SCREEN_SIZE / 8;
  int selectedPieceIndex = 0;
  GameMode currentScreen = MENU;

  InitWindow(screenWidth, screenHeight, "Raychess");
  load_textures();
  init_pieces();

  // For the game screen in which the game will be drawn
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (currentScreen) {
    case MENU: {
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = GAME;
      }
      if (IsKeyPressed(KEY_Q)) {
        return 0;
      }
    } break;
    case GAME: {
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 gridPos = (Vector2){(mousePos.x - gameScreenPos.x) / cellWidth,
                                    (mousePos.y - gameScreenPos.y) / cellWidth};
        for (int i = 0; i < store.pieceCount; i++) {
          if (Vector2Equals(store.pieces[i].position, gridPos)) {
            selectedPieceIndex = i;
          }
        }

        if (selectedPieceIndex != -1) {
          if (is_move_valid(store.pieces[selectedPieceIndex], gridPos)) {
            store.pieces[selectedPieceIndex].position = gridPos;
          }
          selectedPieceIndex = -1;
        }
      }
    } break;
    default:
      break;
    }
    BeginDrawing();

    switch (currentScreen) {
    case MENU: {
      ClearBackground(BLACK);
      DrawText("Press Enter or Tap to BEGIN", 20, 20, FONT_SIZE, WHITE);
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
      draw_pieces(cellWidth, gameScreenPos);
      DrawText("ENTER to end game!", 20, 20, FONT_SIZE, WHITE);
    } break;
    default:
      break;
    }
    EndDrawing();
  }

  unload_textures();
  CloseWindow();
  return 0;
};
