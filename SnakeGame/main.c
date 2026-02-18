#include "MKL05Z4.h"
#include <stdlib.h>
// Dolaczamy nasze nowe biblioteki
#include "lcd_i2c.h"
#include "adc_joy.h"

#define WIDTH  16
#define HEIGHT 2
#define WIN_LENGTH 10 // Ile punktów zeby wygrac

typedef struct { int x; int y; } Point;
Point snake[32]; 
Point food;
int snake_len = 3;

volatile int current_dir = 1; 
volatile int next_dir = 1;    
volatile int input_lock = 0;  
volatile uint32_t msTicks = 0;
int game_over = 0; // Flaga stanu gry

void SysTick_Handler(void) { msTicks++; }
uint32_t GetTick(void) { return msTicks; }

void CheckInput() {
    int x = ADC_Read(11); // PTB8
    int y = ADC_Read(10); // PTB9
    int new_req = -1;

    if (x < 800) new_req = 3;      
    else if (x > 3300) new_req = 1;
    else if (y < 800) new_req = 0; 
    else if (y > 3300) new_req = 2;
    
    if (new_req != -1 && !input_lock) {
        if (current_dir == 0 && new_req == 2) return; 
        if (current_dir == 2 && new_req == 0) return; 
        if (current_dir == 1 && new_req == 3) return; 
        if (current_dir == 3 && new_req == 1) return; 
        if (new_req != current_dir) {
            next_dir = new_req;
            input_lock = 1; 
        }
    }
}

void SpawnFood() {
    // Ulepszone losowanie - zeby nie wypadlo na wezu
    int valid = 0;
    while(!valid) {
        valid = 1;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        for(int i=0; i<snake_len; i++) {
            if(snake[i].x == food.x && snake[i].y == food.y) valid = 0;
        }
    }
    LCD_SetCursor(food.x, food.y);
    LCD_Char('*');
}

void GameOverScreen() {
    game_over = 1;
    LCD_Clear();
    LCD_SetCursor(3, 0);
    LCD_String("GAME OVER!");
    LCD_SetCursor(0, 1);
    LCD_String("Score: ");
    LCD_Char((snake_len-3)/10 + '0'); // Dziesiatki
    LCD_Char((snake_len-3)%10 + '0'); // Jednosci
}

void WinScreen() {
    game_over = 1;
    LCD_Clear();
    LCD_SetCursor(3, 0);
    LCD_String("WYGRANA!"); // Polski napis :)
    LCD_SetCursor(1, 1);
    LCD_String("Mistrz Weza!");
}

void UpdateGame() {
    if(game_over) return; // Jesli gra skonczona, nic nie rób

    current_dir = next_dir;
    input_lock = 0; 

    Point tail_to_erase = snake[snake_len - 1];
    Point old_head = snake[0];

    // Przesun cialo
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Przesun glowe
    if (current_dir == 0) snake[0].y--;
    if (current_dir == 1) snake[0].x++;
    if (current_dir == 2) snake[0].y++;
    if (current_dir == 3) snake[0].x--;

    // 1. Sprawdz sciany
    if (snake[0].x >= WIDTH)  snake[0].x = 0;
    if (snake[0].x < 0)       snake[0].x = WIDTH - 1;
    if (snake[0].y >= HEIGHT) snake[0].y = 0;
    if (snake[0].y < 0)       snake[0].y = HEIGHT - 1;

    // 2. NOWOSC: Sprawdz czy waz wjechal sam w siebie
    // Petla od 1, bo 0 to glowa
    for(int i = 1; i < snake_len; i++) {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            GameOverScreen();
            return;
        }
    }

    // 3. Sprawdz czy zjadl
    int ate = 0;
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snake_len < 30) {
            snake_len++; 
            snake[snake_len-1] = tail_to_erase; 
        }
        ate = 1;
        
        // NOWOSC: Sprawdz wygrana
        if(snake_len >= WIN_LENGTH) {
            WinScreen();
            return;
        }
        
        SpawnFood();
    }

    // 4. Rysowanie
    if (!ate) {
        LCD_SetCursor(tail_to_erase.x, tail_to_erase.y);
        LCD_Char(' ');
    }
    LCD_SetCursor(old_head.x, old_head.y);
    LCD_Char('o');
    LCD_SetCursor(snake[0].x, snake[0].y);
    LCD_Char('O');
}

void RestartGame() {
    game_over = 0;
    snake_len = 3;
    snake[0].x = 5; snake[0].y = 0;
    snake[1].x = 4; snake[1].y = 0;
    snake[2].x = 3; snake[2].y = 0;
    for(int i=3; i<32; i++) { snake[i].x=0; snake[i].y=0; }
    
    current_dir = 1;
    next_dir = 1;
    
    LCD_Clear();
    LCD_SetCursor(snake[0].x, snake[0].y); LCD_Char('O');
    LCD_SetCursor(snake[1].x, snake[1].y); LCD_Char('o');
    LCD_SetCursor(snake[2].x, snake[2].y); LCD_Char('o');
    SpawnFood();
}

int main(void) {
    SysTick_Config(41943040 / 1000); // 1ms timer
    LCD_Init();
    ADC_Init();

    RestartGame();

    uint32_t last_input = 0;
    uint32_t last_update = 0;
    uint32_t game_speed = 200;

    while(1) {
        uint32_t now = GetTick();

        if (now - last_input >= 10) {
            last_input = now;
            CheckInput();
            
            // Restart po przegranej (wcisnij joystick w dowolna strone po 2s)
            if(game_over && (now - last_update > 2000)) {
                 int x = ADC_Read(11);
                 if(x < 500 || x > 3500) RestartGame();
            }
        }

        if (now - last_update >= game_speed) {
            // Jesli gra trwa - aktualizuj. 
            // Jesli gra skonczona - tylko odswiezaj czas dla restartu
            if(!game_over) {
                last_update = now;
                UpdateGame();
            }
        }
    }
}