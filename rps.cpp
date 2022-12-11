#include <Arduino.h>
#include <stdbool.h>
#include "rps.h"
#include "lcd_wrapper.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int redLeds[3] = {PLR_1_LED_1, PLR_1_LED_2, PLR_1_LED_3};
int blueLeds[3] = {PLR_2_LED_1, PLR_2_LED_2, PLR_2_LED_3};
int redButtons[3] = {PLR_1_ROCK_BTN, PLR_1_PAPER_BTN, PLR_1_SCISSOR_BTN};
int blueButtons[3] = {PLR_2_ROCK_BTN, PLR_2_PAPER_BTN, PLR_2_SCISSOR_BTN};

Player initializePlayer(int team) {
    Player player;
    player.team = team;
    player.score = 0;
    player.choice = UNSELECTED;
    player.leds = team == TEAM_RED ? redLeds : blueLeds;
    player.buttons = team == TEAM_RED ? redButtons : blueButtons;

    return player;
}

void initializeGame(int* totalScore) {
    Game game;
    game.player1 = initializePlayer(1);
    game.player2 = initializePlayer(2);
    game.playerTurn = PLAYER_1_TURN;
    game.round = 0;
    turnOffLeds();
    lcd_clear();
    printTextInSequence(0, 0, "Welcome in game", 100);
    printTextInSequence(1, 0, "RPS - 2 Player", 100);
    delay(2000);

    while (!isGameOver(&game)) {
        loop(&game);
    }

    displayGameState(game);
    delay(3000);
    onGameEnd(&game, totalScore);
}

void loop(struct Game* game) {
    displayGameState(game);
    delay(4000);
    // Player 1 turn
    game->playerTurn = PLAYER_1_TURN;
    lcd_clear();
    playerTurn(game, &game->player1, TEAM_RED);
    delay(500);
    game->playerTurn = PLAYER_2_TURN;
    lcd_clear();
    playerTurn(game, &game->player2, TEAM_BLUE);
    delay(500);

    game->playerTurn = BLOCKED_TURN;
    game->round++;
    char* strPlr1 = stringifyChoice(game->player1.choice);
    char* strPlr2 = stringifyChoice(game->player2.choice);
    lcd_clear();
    // Result
    lcd_print_at(0, 0, "Red");
    lcd_print_at(1, 0, strPlr1);

    // Player blue
    lcd_print_at(0, 12, "Blue");
    lcd_print_at(1, 16 - strlen(strPlr2), strPlr2);

    // Get result
    if (game->player1.choice == game->player2.choice) {
        lcd_print_at(1, 7, "==");
        delay(1500);
        lcd_clear();
        lcd_print_at(1,6, "DRAW");
    } else {
        // Non-draw result
        if (game->player1.choice == ROCK && game->player2.choice == PAPER) {
            // plr 2 won
            lcd_print_at(1, 7, "<");
            addScoreToPlayer(&game->player2);
        } else if (game->player1.choice == ROCK && game->player2.choice == SCISSOR) {
            // plr 1 won
            lcd_print_at(1, 7, ">");
            addScoreToPlayer(&game->player1);
        } else if (game->player1.choice == PAPER && game->player2.choice == ROCK) {
            // plr 1 won
            lcd_print_at(1, 7, ">");
            addScoreToPlayer(&game->player1);
        } else if (game->player1.choice == PAPER && game->player2.choice == SCISSOR) {
            // plr 2 won
            lcd_print_at(1, 7, "<");
            addScoreToPlayer(&game->player2);
        } else if (game->player1.choice == SCISSOR && game->player2.choice == ROCK) {
            // plr 2 won
            lcd_print_at(1, 7, "<");
            addScoreToPlayer(&game->player2);
        } else if (game->player1.choice == SCISSOR && game->player2.choice == PAPER) {
            // plr 1 won
            lcd_print_at(1, 7, ">");
            addScoreToPlayer(&game->player1);
        }
    }
    delay(2000);
}

void addScoreToPlayer(struct Player* player) {
    // Turn on leds
    digitalWrite(player->leds[player->score], HIGH);

    player->score++;
}

void playerTurn(struct Game* game, struct Player* player, int team) {
    bool isScoreDisplayed = false;
    lcd_clear();
    lcd_print_at(0, 0, team == TEAM_RED ? "-> Player Red" : "-> Player Blue");

    while (true) {
        if (game->playerTurn == BLOCKED_TURN) {
            break;
        }

        int pressedButtons = 0;
        POWERS selectedPower = UNSELECTED;
        for (int i = 0; i < 3; i++) {
            if (digitalRead(player->buttons[i]) == HIGH) {
                switch (i) {
                    case 0: selectedPower = ROCK; break;
                    case 1: selectedPower = PAPER; break;
                    case 2: selectedPower = SCISSOR; break;
                }
                pressedButtons++;
            }
        }

        if (pressedButtons == 1) {
            player->choice = selectedPower;
            break;
        }

        if (pressedButtons > 1 && !isScoreDisplayed) {
            isScoreDisplayed = true;
            lcd_clear();
            displayGameState(game);
            delay(300);
        } else if (pressedButtons < 2 && isScoreDisplayed) {
            lcd_clear();
            lcd_print_at(0, 0, team == TEAM_RED ? "-> Player Red" : "-> Player Blue");
        }

        // More buttons pressed or none
    }
}

void displayGameState(struct Game* game) {
    lcd_clear();
    char turns[3] = "";
    sprintf(turns, "%d", game->round);

    char scoreP1[2] = "0";
    char scoreP2[2] = "0";
    scoreP1[0] = game->player1.score + '0';
    scoreP2[0] = game->player2.score + '0';
    // Player blue
    lcd_print_at(0, 0, "Red");
    lcd_print_at(1, 0, scoreP1);

    // Player blue
    lcd_print_at(0, 12, "Blue");
    lcd_print_at(1, 15, scoreP2);

    // Round
    lcd_print_at(0, 6, "rnd");
    lcd_print_at(1, 7, turns);
}

bool isGameOver(struct Game* game) {
    if (game->player1.score >= 3 || game->player2.score >= 3) {
        return true;
    }
    return false;
}

int getWinner(struct Game* game) {
    if (game->player1.score >= 3) {
        return TEAM_RED;
    }

    if (game->player2.score >= 3) {
        return TEAM_BLUE;
    }
    return 0;
}

void onGameEnd(struct Game* game, int *totalScore) {
    lcd_clear();
    int winner = getWinner(game);

    printTextInSequence(0, 0, "Game Over!", 100);
    delay(1000);
    lcd_clear();
    printTextInSequence(0, 0, "Winner Is: ", 100);
    turnOffLeds();

    printTextInSequence(1, 1, winner  == TEAM_RED ? 
        "Red Player" :
        "Blue Player", 100
    );

    if (winner == TEAM_RED) {
        playLedShow(redLeds);
        totalScore[0]++;
    } else {
        playLedShow(blueLeds);
        totalScore[1]++;
    }

    free(game);

    lcd_clear();
    char* redTeamScore = "Red sets: 0";
    char* blueTeamScore = "Blue sets: 0";
    redTeamScore[10] = totalScore[0] + '0';
    blueTeamScore[11] = totalScore[1] + '0';

    // Display total score
    lcd_print_at(0, 0, redTeamScore);
    lcd_print_at(1, 0, blueTeamScore);
    delay(3000);
    lcd_clear();
    lcd_print_at(0, 0, "Restarting ...");
    delay(1000);
}

void turnOffLeds() {
    digitalWrite(PLR_1_LED_1, LOW);
    digitalWrite(PLR_1_LED_2, LOW);
    digitalWrite(PLR_1_LED_3, LOW);
    digitalWrite(PLR_2_LED_1, LOW);
    digitalWrite(PLR_2_LED_2, LOW);
    digitalWrite(PLR_2_LED_3, LOW);
}

void playLedShow(int leds[3]) {
    for (int i = 0; i < 5; i++) {
        turnOffLeds();
        delay(150);
        for (int led = 0; led < 3; led++) {
            digitalWrite(leds[led], HIGH);
            delay(50);
        }
    }

    for (int i = 0; i < 10; i++) {
        turnOffLeds();
        digitalWrite(leds[i % 3], HIGH);
        delay(50);
    }

    turnOffLeds();
}

char* stringifyChoice(POWERS choice) {
    if (choice == ROCK) {
        return "Rock";
    } else if (choice == PAPER) {
        return "Paper";
    } else if (choice == SCISSOR) {
        return "Scissor";
    }

    return "err";
}
