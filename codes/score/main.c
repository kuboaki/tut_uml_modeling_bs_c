/* main.c - ボーリングスコアシステムのメインプログラム */
#include "bowling.h"

int main(void) {
    ScoreSheet sheet;
    scoresheet_init(&sheet, time(NULL)); // <1>

    scoresheet_add_game(&sheet, 1); // <2>
    Game *game = &sheet.games[sheet.game_count - 1]; // <3>

    game_entry(game, "くぼあき"); // <4>
    game_entry(game, "うえはら");

    scoresheet_print(&sheet); // <5>
    printf("\n");

    int game_records[2][30] = { // <6>
        {7, 0, 5, 5, 10, 10, 5, 4, 10, 7, 3, 5, 4, 7, 3, 7, 3, 4},
        {6, 3, 9, 0, 0, 3, 8, 2, 7, 3, 10, 9, 1, 8, 0, 10, 6, 3}
    };
    int record_index[2] = {0, 0}; // <7>

    while (!game_is_finished(game)) { // <8>
        printf("----------------------------------------\n");
        printf("turn: %s\n", game_turn_player_name(game));

        int score_index = game->turn; // <9>
        int pins = game_records[score_index][record_index[score_index]]; // <10>
        record_index[score_index]++;

        printf("input pins: %d\n", pins);
        game_playing(game, score_index, pins); // <11>

        game_print(game); // <12>

        printf("Press Enter to continue..."); // <13>
        getchar();
    }

    printf("Game(id:%s) is finished.\n", game->id);
    return 0;
}
