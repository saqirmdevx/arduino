#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "lcd_wrapper.h"
#include "string.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
}

void lcd_clear()
{
    lcd.clear();
}


void lcd_set_cursor(int y, int x)
{
    lcd.setCursor(x, y);
}


void lcd_print(char* text)
{
    lcd.print(text);
}


void lcd_print_at(int y, int x, char* text)
{
    lcd_set_cursor(y, x);
    lcd_print(text);
}

void printTextInSequence(int y, int x, char* text, int time) {
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        char str[2] = "";
        sprintf(str, "%c", text[i]);
        lcd_print_at(y, x + i, str);
        delay(time);
    }
}