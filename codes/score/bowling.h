/* bowling.h - ボーリングスコア管理システム */
#ifndef BOWLING_H
#define BOWLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* 定数定義 */
#define MAX_FRAMES 15       /* ダミーフレーム含む配列サイズ */
#define MAX_PLAYERS 10      /* 最大プレイヤー数 */
#define MAX_GAMES 20        /* 最大ゲーム数 */
#define MAX_NAME_LEN 64     /* プレイヤー名の最大長 */
#define ID_LEN 12           /* ID文字列の長さ */

/* フレームの状態 */
typedef enum {
    RESERVED,
    BEFORE_1ST,
    BEFORE_2ND,
    PENDING,
    FIXED
} FrameState;

/* イベントタイプ */
typedef enum {
    SETUP,
    PINS,
    DETERMINE
} EventType;

/* スコアの状態 */
typedef enum {
    WAIT_FOR_1ST,
    WAIT_FOR_2ND,
    FINISHED
} ScoreState;

/* Frame構造体: 1フレーム分のピン数やボーナスを記録 */
typedef struct {
    int frame_no;
    int first;
    int second;
    int spare_bonus;
    int strike_bonus;
    int total;
    FrameState state;
} Frame;

/* Frame関数 */
void frame_init(Frame *frame, int frame_no);
void frame_action(Frame *frame, EventType event, int pins);
int frame_score(const Frame *frame);
bool frame_is_strike(const Frame *frame);
bool frame_is_spare(const Frame *frame);
bool frame_is_miss(const Frame *frame);
bool frame_is_gutter(const Frame *frame);
bool frame_is_fixed(const Frame *frame);
void frame_print(const Frame *frame);

/* Score構造体: 各人の10フレーム分のスコアを記録 */
typedef struct {
    char id[ID_LEN];
    char player[MAX_NAME_LEN];
    int fno;
    Frame frames[MAX_FRAMES];
    ScoreState state;
} Score;

/* Score関数 */
void score_init(Score *score, const char *name);
int score_fno2idx(int fno);
Frame* score_frame(Score *score, int fno);
void score_go_next_frame(Score *score);
Frame* score_current(Score *score);
Frame* score_prev(Score *score);
Frame* score_pprev(Score *score);
void score_calc_spare_bonus_after_1st(Score *score);
void score_calc_strike_bonus_after_1st(Score *score);
void score_calc_strike_bonus_after_2nd(Score *score);
void score_update_total(Score *score);
bool score_is_finished(const Score *score);
void score_scoring(Score *score, int pins);
void score_print(const Score *score);

/* Game構造体: 複数名の1ゲーム分のスコアのセット */
typedef struct {
    char id[ID_LEN];
    int turn;
    Score scores[MAX_PLAYERS];
    int player_count;
} Game;

/* Game関数 */
void game_init(Game *game);
void game_entry(Game *game, const char *name);
const char* game_turn_player_name(const Game *game);
void game_go_next_turn(Game *game);
void game_playing(Game *game, int score_index, int pins);
bool game_is_finished(const Game *game);
void game_print(const Game *game);

/* ScoreSheet構造体: 複数名の複数回のGameを記録 */
typedef struct {
    char id[ID_LEN];
    time_t play_date;
    Game games[MAX_GAMES];
    int game_count;
} ScoreSheet;

/* ScoreSheet関数 */
void scoresheet_init(ScoreSheet *sheet, time_t date);
void scoresheet_add_game(ScoreSheet *sheet, int games);
void scoresheet_print(const ScoreSheet *sheet);

/* ユーティリティ関数 */
void generate_id(char *id);
const char* frame_state_to_string(FrameState state);

#endif /* BOWLING_H */
