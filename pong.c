#include <stdio.h>
// Определение ширины и высоты игрового поля
#define WIDTH 80
#define HEIGHT 25

// Определение размера ракетки и символов для отображения мяча, ракетки и границы поля
#define RACKET_SIZE 3
#define BALL_CHAR 'o'
#define RACKET_CHAR '['
#define BORDER_CHAR '.'
#define EMPTY_CHAR ' '

// Определение клавиш управления для левого и правого игроков, а также клавиши пробела
#define LEFT_PLAYER_KEY_A 'a'
#define LEFT_PLAYER_KEY_Z 'z'
#define RIGHT_PLAYER_KEY_K 'k'
#define RIGHT_PLAYER_KEY_M 'm'
#define SPACE_BAR_KEY 32

typedef struct {
    char field[HEIGHT][WIDTH];
    int left_racket;
    int right_racket;
    int ball_i;
    int ball_j;
    int dir_x;
    int dir_y;
    int left_score;
    int right_score;
} GameState;

// Инициализация начального состояния игры
void initialize(GameState *game_state);

// Отрисовка текущего состояния игры
void draw(const GameState *game_state);

// Обновление состояния игры
void update(GameState *game_state);

// Обработка пользовательского ввода
void handle_input(GameState *game_state);

int main() {
    GameState game_state;
    initialize(&game_state);

    while (1) {
        draw(&game_state);
        // Проверка на победу одного из игроков
        if (game_state.left_score == 21) {
            printf("\nЛевый игрок победил!\n");
            break;
        } else if (game_state.right_score == 21) {
            printf("\nПравый игрок победил!\n");
            break;
        }
        handle_input(&game_state);
        update(&game_state);
    }

    return 0;
}

// Инициализация начального состояния игры
void initialize(GameState *game_state) {
    // Инициализация поля
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                game_state->field[i][j] = BORDER_CHAR;
            } else if (j == 0 || j == WIDTH - 1) {
                game_state->field[i][j] = BORDER_CHAR;
            } else {
                game_state->field[i][j] = EMPTY_CHAR;
            }
        }
    }

    // Инициализация ракеток
    game_state->left_racket = HEIGHT / 2 - RACKET_SIZE / 2;
    game_state->right_racket = HEIGHT / 2 - RACKET_SIZE / 2;

    // Инициализация мяча
    game_state->ball_i = HEIGHT / 2;
    game_state->ball_j = WIDTH / 2;
    game_state->dir_x = 1;
    game_state->dir_y = 1;

    // Инициализация счета игры
    game_state->left_score = 0;
    game_state->right_score = 0;
}

// Отрисовка текущего состояния игры
void draw(const GameState *game_state) {
    printf("\033[H"); // Перемещение курсора в верхний левый угол экрана
    printf("\n\n\nСЧЕТ ЛЕВОГО ИГРОКА: %d\t\t\t\t\tСЧЕТ ПРАВОГО ИГРОКА: %d\n", game_state->left_score, game_state->right_score);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if ((i >= game_state->left_racket && i < game_state->left_racket + RACKET_SIZE) && j == 0) {
                printf("%c", RACKET_CHAR);
            } else if ((i >= game_state->right_racket && i < game_state->right_racket + RACKET_SIZE) && j == WIDTH - 1) {
                printf("%c", RACKET_CHAR);
            } else if (i == game_state->ball_i && j == game_state->ball_j) {
                printf("%c", BALL_CHAR);
            } else {
                printf("%c", game_state->field[i][j]);
            }
        }
        printf("\n");
    }
}

// Обновление состояния игры
void update(GameState *game_state) {
    // Обновление позиции мяча
    game_state->ball_i += game_state->dir_y;
    game_state->ball_j += game_state->dir_x;

    // Проверка на столкновение с ракетками
    if ((game_state->ball_j == 1) &&
        (game_state->ball_i >= game_state->left_racket && game_state->ball_i < game_state->left_racket + RACKET_SIZE)) {
        game_state->dir_x *= -1;
    } else if ((game_state->ball_j == WIDTH - 2) &&
               (game_state->ball_i >= game_state->right_racket && game_state->ball_i < game_state->right_racket + RACKET_SIZE)) {
        game_state->dir_x *= -1;
    }

    // Проверка на столкновение с верхней/нижней границей
    if (game_state->ball_i == 0 || game_state->ball_i == HEIGHT - 1) {
        game_state->dir_y *= -1;
    }

    // Проверка на гол
    if (game_state->ball_j == WIDTH - 1) {
        game_state->left_score += 1; // Увеличение счета левого игрока
        // Перемещение мяча в центр, НЕ перезапуская игру
        game_state->ball_i = HEIGHT / 2;
        game_state->ball_j = WIDTH / 2;
    } else if (game_state->ball_j == 0) {
        game_state->right_score += 1;
        game_state->ball_i = HEIGHT / 2;
        game_state->ball_j = WIDTH / 2;
    }
}

// Обработка пользовательского ввода
void handle_input(GameState *game_state) {
    char key;
    key = getchar(); // Получение символа ввода

    // Обработка символов
    switch (key) {
        case LEFT_PLAYER_KEY_A:
            if (game_state->left_racket > 0) (game_state->left_racket)--;
            break;
        case LEFT_PLAYER_KEY_Z:
            if (game_state->left_racket < HEIGHT - RACKET_SIZE) (game_state->left_racket)++;
            break;
        case RIGHT_PLAYER_KEY_K:
            if (game_state->right_racket > 0) (game_state->right_racket)--;
            break;
        case RIGHT_PLAYER_KEY_M:
            if (game_state->right_racket < HEIGHT - RACKET_SIZE) (game_state->right_racket)++;
            break;
    }
}
