#ifndef _RPS_H
#define _RPS_H

// Player 1
#define PLR_1_ROCK_BTN      2
#define PLR_1_PAPER_BTN     3
#define PLR_1_SCISSOR_BTN   4
#define PLR_1_LED_1         8
#define PLR_1_LED_2         9
#define PLR_1_LED_3         10

// Player 2
#define PLR_2_ROCK_BTN      5
#define PLR_2_PAPER_BTN     6
#define PLR_2_SCISSOR_BTN   7
#define PLR_2_LED_1         11
#define PLR_2_LED_2         12
#define PLR_2_LED_3         13

//TURNS
#define BLOCKED_TURN        0
#define PLAYER_1_TURN       1
#define PLAYER_2_TURN       2

// TEAMS
#define TEAM_RED            1
#define TEAM_BLUE           2

int redLeds[3] = {PLR_1_LED_1, PLR_1_LED_2, PLR_1_LED_3};
int blueLeds[3] = {PLR_1_LED_1, PLR_1_LED_2, PLR_1_LED_3};
int redButtons[3] = {PLR_1_ROCK_BTN, PLR_1_PAPER_BTN, PLR_1_SCISSOR_BTN};
int blueButtons[3] = {PLR_2_ROCK_BTN, PLR_2_PAPER_BTN, PLR_2_SCISSOR_BTN};

enum POWERS {
    UNSELECTED,
    ROCK,
    PAPER,
    SCISSOR
};

struct Player {
    int team; //red / blue
    int score;
    POWERS choice = UNSELECTED;
    int* leds;
    int* buttons;
};

struct Game {
    Player player1;
    Player player2;
    int round;
    int playerTurn;
};


void initializeGame();
void loop(struct Game* game);
void playerTurn(struct Game* game, struct Player* player);
void onGameEnd(struct Game* game);
bool isGameOver(struct Game* game);
bool validateInput(struct Game* game);
Player initializePlayer(int team);
void turnOffLeds();
int getWinner(struct Game* game);
void playLedShow(int leds[3]);
void displayGameState(struct Game* game);
char* stringifyChoice(POWERS choice);
void addScoreToPlayer(struct Player* player);
#endif