#include <math.h>
#include <raylib.h>

#define FONT_SIZE 20
#define GAME_SCREEN_SIZE 720
#define ROWS 8
#define COLUMNS 8
#define MAX_PIECES 32

typedef enum GameMode { MENU = 0, GAME } GameMode;

typedef struct Cell {
  int x;
  int y;
} Cell;

typedef struct Textures {
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

typedef enum PieceType { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } PieceType;

typedef struct Piece {
  PieceType type;
  Vector2 position;
  bool isWhite;
  Texture2D texture;
} Piece;

Textures textures;
Piece pieces[MAX_PIECES];
int pieceCount = 0;

Texture2D texture_from_image(const char *path) {
  Image image = LoadImage(path);
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image);
  return texture;
};

void load_textures() {
  textures.darkCell = texture_from_image("src/assets/square_brown_dark.png");
  textures.lightCell = texture_from_image("src/assets/square_gray_dark.png");
  textures.blackKing = texture_from_image("src/assets/b_king.png");
  textures.blackQueen = texture_from_image("src/assets/b_queen.png");
  textures.blackPawn = texture_from_image("src/assets/b_pawn.png");
  textures.blackBishop = texture_from_image("src/assets/b_bishop.png");
  textures.blackKnight = texture_from_image("src/assets/b_knight.png");
  textures.blackRook = texture_from_image("src/assets/b_rook.png");

  textures.whiteKing = texture_from_image("src/assets/w_king.png");
  textures.whiteQueen = texture_from_image("src/assets/w_queen.png");
  textures.whitePawn = texture_from_image("src/assets/w_pawn.png");
  textures.whiteBishop = texture_from_image("src/assets/w_bishop.png");
  textures.whiteKnight = texture_from_image("src/assets/w_knight.png");
  textures.whiteRook = texture_from_image("src/assets/w_rook.png");
};

void unload_textures() {
  UnloadTexture(textures.darkCell);
  UnloadTexture(textures.lightCell);
  UnloadTexture(textures.blackKing);
  UnloadTexture(textures.blackQueen);
  UnloadTexture(textures.blackPawn);
  UnloadTexture(textures.blackBishop);
  UnloadTexture(textures.blackKnight);
  UnloadTexture(textures.blackRook);
  UnloadTexture(textures.whiteKing);
  UnloadTexture(textures.whiteQueen);
  UnloadTexture(textures.whitePawn);
  UnloadTexture(textures.whiteBishop);
  UnloadTexture(textures.whiteKnight);
  UnloadTexture(textures.whiteRook);
};

void init_pieces() {
  for (int i = 0; i < COLUMNS; i++) {
    pieces[pieceCount++] =
        (Piece){PAWN, (Vector2){i, 6}, true, textures.whitePawn};
    pieces[pieceCount++] =
        (Piece){PAWN, (Vector2){i, 1}, false, textures.blackPawn};
  }

  pieces[pieceCount++] = (Piece){.type = KING,
                                 .position = (Vector2){4, 7},
                                 .isWhite = true,
                                 .texture = textures.whiteKing};
  pieces[pieceCount++] = (Piece){.type = KING,
                                 .position = (Vector2){4, 0},
                                 .isWhite = false,
                                 .texture = textures.blackKing};
  pieces[pieceCount++] = (Piece){.type = QUEEN,
                                 .position = (Vector2){3, 7},
                                 .isWhite = true,
                                 .texture = textures.whiteQueen};
  pieces[pieceCount++] = (Piece){.type = QUEEN,
                                 .position = (Vector2){3, 0},
                                 .isWhite = false,
                                 .texture = textures.blackQueen};
  pieces[pieceCount++] =
      (Piece){ROOK, (Vector2){0, 7}, true, textures.whiteRook};
  pieces[pieceCount++] =
      (Piece){ROOK, (Vector2){7, 7}, true, textures.whiteRook};
  pieces[pieceCount++] =
      (Piece){ROOK, (Vector2){0, 0}, false, textures.blackRook};
  pieces[pieceCount++] =
      (Piece){ROOK, (Vector2){7, 0}, false, textures.blackRook};

  pieces[pieceCount++] =
      (Piece){KNIGHT, (Vector2){1, 7}, true, textures.whiteKnight};
  pieces[pieceCount++] =
      (Piece){KNIGHT, (Vector2){6, 7}, true, textures.whiteKnight};
  pieces[pieceCount++] =
      (Piece){KNIGHT, (Vector2){1, 0}, false, textures.blackKnight};
  pieces[pieceCount++] =
      (Piece){KNIGHT, (Vector2){6, 0}, false, textures.blackKnight};

  pieces[pieceCount++] =
      (Piece){BISHOP, (Vector2){2, 7}, true, textures.whiteBishop};
  pieces[pieceCount++] =
      (Piece){BISHOP, (Vector2){5, 7}, true, textures.whiteBishop};
  pieces[pieceCount++] =
      (Piece){BISHOP, (Vector2){2, 0}, false, textures.blackBishop};
  pieces[pieceCount++] =
      (Piece){BISHOP, (Vector2){5, 0}, false, textures.blackBishop};
}

void draw_grid(Cell cell, int cellWidth, Vector2 gameScreenPos) {
  Texture2D texture;
  if ((cell.x + cell.y) % 2 == 0) { // Alternating textures
    texture = textures.darkCell;
  } else {
    texture = textures.lightCell;
  }

  Vector2 position = {gameScreenPos.x + cell.x * cellWidth,
                      gameScreenPos.y + cell.y * cellWidth};
  float scale = (float)cellWidth / texture.width;
  DrawTextureEx(texture, position, 0.0f, scale, WHITE);
};

void draw_pieces(int cellWidth, Vector2 gameScreenPos) {
  for (int i = 0; i < pieceCount; i++) {
    Piece p = pieces[i];
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
