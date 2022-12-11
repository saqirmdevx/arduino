#include "rps.h"
#include "lcd_wrapper.h"
#include <time.h>
#include <stdlib.h>
#include "string.h"

//private functions
void initializeLeds();
void initializeButtons();

int totalScore[2] = { 0, 0 };

void setup() {
    lcd_init();
    initializeLeds();
    initializeButtons();
    randomSeed(analogRead(A1));
}

void loop() {
    initializeGame(totalScore);
}

void initializeLeds()
{
    pinMode(PLR_1_LED_1, OUTPUT);
    pinMode(PLR_1_LED_2 , OUTPUT);
    pinMode(PLR_1_LED_3, OUTPUT);
    pinMode(PLR_2_LED_1, OUTPUT);
    pinMode(PLR_2_LED_2 , OUTPUT);
    pinMode(PLR_2_LED_3, OUTPUT);
}

void initializeButtons()
{
    pinMode(PLR_1_ROCK_BTN, INPUT);
    pinMode(PLR_1_PAPER_BTN, INPUT);
    pinMode(PLR_1_SCISSOR_BTN, INPUT);

    pinMode(PLR_2_ROCK_BTN, INPUT);
    pinMode(PLR_2_PAPER_BTN, INPUT);
    pinMode(PLR_2_SCISSOR_BTN, INPUT);
}