#include "lcd_i2c.h"

#define LCD_ADDR  0x27  // Zmien na 0x3F w razie problemów
#define SCL_PIN   3     // PTB3
#define SDA_PIN   4     // PTB4

// Proste opóznienie dla I2C (lokalne)
static void Delay_us(uint32_t d) {
    d *= 10; while(d--) __asm("nop");
}

static void SDA_Set(int s) { 
    if(s) PTB->PSOR |= (1<<SDA_PIN); 
    else  PTB->PCOR |= (1<<SDA_PIN); 
}

static void SCL_Set(int s) { 
    if(s) PTB->PSOR |= (1<<SCL_PIN); 
    else  PTB->PCOR |= (1<<SCL_PIN); 
}

static void I2C_WriteByte(uint8_t data) {
    for(int i=0; i<8; i++) {
        SDA_Set((data & 0x80) != 0); Delay_us(5);
        SCL_Set(1); Delay_us(5); SCL_Set(0); Delay_us(5);
        data <<= 1;
    }
    SDA_Set(1); SCL_Set(1); Delay_us(5); SCL_Set(0); Delay_us(5);
}

static void LCD_Write_Nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0) | 0x08 | (rs & 0x01);
    SDA_Set(1); SCL_Set(1); Delay_us(5);
    SDA_Set(0); Delay_us(5); SCL_Set(0); Delay_us(5); // Start
    I2C_WriteByte(LCD_ADDR << 1);
    I2C_WriteByte(data | 0x04); // E=1
    I2C_WriteByte(data & ~0x04); // E=0
    SDA_Set(0); SCL_Set(1); Delay_us(5); SDA_Set(1); // Stop
    Delay_us(100); 
}

void LCD_Init(void) {
    // Init GPIO
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    PORTB->PCR[SCL_PIN] = PORT_PCR_MUX(1);
    PORTB->PCR[SDA_PIN] = PORT_PCR_MUX(1);
    PTB->PDDR |= (1 << SCL_PIN) | (1 << SDA_PIN);
    PTB->PSOR |= (1 << SCL_PIN) | (1 << SDA_PIN);

    Delay_us(50000);
    LCD_Write_Nibble(0x30, 0); Delay_us(5000);
    LCD_Write_Nibble(0x30, 0); Delay_us(2000);
    LCD_Write_Nibble(0x30, 0); Delay_us(2000);
    LCD_Write_Nibble(0x20, 0);
    
    LCD_Write_Nibble(0x20, 0); LCD_Write_Nibble(0x80, 0); // Function set
    LCD_Write_Nibble(0x00, 0); LCD_Write_Nibble(0xC0, 0); // Display ON
    LCD_Write_Nibble(0x00, 0); LCD_Write_Nibble(0x10, 0); // Clear
    Delay_us(5000);
}

void LCD_Clear(void) {
    LCD_Write_Nibble(0x00, 0); 
    LCD_Write_Nibble(0x10, 0); 
    Delay_us(5000);
}

void LCD_SetCursor(int x, int y) {
    uint8_t addr = 0x80 + x;
    if (y >= 1) addr = 0xC0 + x;
    LCD_Write_Nibble(addr & 0xF0, 0);
    LCD_Write_Nibble((addr << 4) & 0xF0, 0);
}

void LCD_Char(char c) {
    LCD_Write_Nibble(c & 0xF0, 1);
    LCD_Write_Nibble((c << 4) & 0xF0, 1);
}

void LCD_String(char* str) {
    while(*str) LCD_Char(*str++);
}