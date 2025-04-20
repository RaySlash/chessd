use std::any::type_name_of_val;

use raylib::prelude::*;

const FONT_SIZE: i32 = 24;
const FONT_SPACING: f32 = 0.4;

const ROWS: usize = 8;
const COLUMNS: usize = 8;

enum GameScreen {
    GAME,
    MENU,
}

#[derive(Clone, Debug)]
struct Cell {
    x: i32,
    y: i32,
}

fn main() {
    let mut game_screen: GameScreen = GameScreen::MENU;
    let (mut rl, thread) = raylib::init()
        .size(800, 600)
        .title("Chessd")
        .resizable()
        .undecorated()
        .build();

    let atkinson = rl
        .load_font(&thread, "./assets/fonts/AtkinsonHyperlegible-Regular.ttf")
        .expect("ERROR: Failed to load font.");

    let mut screen_width = rl.get_screen_width();
    let mut screen_height = rl.get_screen_height();

    let cell_width = (screen_width as f32) * (0.1);
    let mut grid = vec![vec![Cell { x: 0, y: 0 }; COLUMNS]; ROWS];

    for i in 0..ROWS {
        for j in 0..COLUMNS {
            grid[i][j] = Cell {
                x: (i as i32) * (cell_width.round() as i32),
                y: (j as i32) * (cell_width.round() as i32),
            };
        }
    }
    while !rl.window_should_close() {
        screen_width = rl.get_screen_width();
        screen_height = rl.get_screen_height();
        rl.gui_set_font(&atkinson);

        match game_screen {
            GameScreen::MENU => {
                if rl.is_key_pressed(KeyboardKey::KEY_ENTER) {
                    game_screen = GameScreen::GAME
                }
            }
            GameScreen::GAME => {
                if rl.is_key_pressed(KeyboardKey::KEY_ENTER) {
                    game_screen = GameScreen::MENU
                }
            }
        };
        let mut d = rl.begin_drawing(&thread);
        d.gui_set_font(&atkinson);

        match game_screen {
            GameScreen::MENU => {
                d.clear_background(Color::BLACK);
                d.draw_text_ex(
                    &atkinson,
                    "Press Enter to START..",
                    Vector2::new(12.0, 12.0),
                    FONT_SIZE as f32,
                    FONT_SPACING,
                    Color::WHITE,
                );
            }
            GameScreen::GAME => {
                d.clear_background(Color::BLACK);
                d.draw_text_ex(
                    &atkinson,
                    "Press Enter to END..",
                    Vector2::new(12.0, 12.0),
                    FONT_SIZE as f32,
                    FONT_SPACING,
                    Color::WHITE,
                );
                for i in 0..ROWS {
                    for j in 0..COLUMNS {
                        let cell = &grid[i][j];
                        d.draw_rectangle(
                            cell.x,
                            cell.y,
                            cell_width.round() as i32,
                            cell_width.round() as i32,
                            Color::RED,
                        );
                    }
                }
            }
        }
    }
}
