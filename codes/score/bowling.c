/* bowling.c - ボーリングスコア管理システムの実装 */
#include "bowling.h"

void generate_id(char *id) { // <1>
    static int counter = 0;
    snprintf(id, ID_LEN, "ID%08d", counter++);
}

const char* frame_state_to_string(FrameState state) { // <2>
    switch (state) {
        case RESERVED:    return "RESERVED";
        case BEFORE_1ST:  return "BEFORE_1ST";
        case BEFORE_2ND:  return "BEFORE_2ND";
        case PENDING:     return "PENDING";
        case FIXED:       return "FIXED";
        default:          return "UNKNOWN";
    }
}

/* =================================================================
 * Frame実装
 * ================================================================= */

void frame_init(Frame *frame, int frame_no) { // <3>
    frame->frame_no = frame_no;
    frame->first = 0;
    frame->second = 0;
    frame->spare_bonus = 0;
    frame->strike_bonus = 0;
    frame->total = 0;
    frame->state = RESERVED; // <4>
}

static void frame_before_1st_proc(Frame *frame, EventType event, int pins) {
    if (event == PINS) {
        if (pins < 0 || pins > 10) {
            printf("invalid pins: %d\n", pins);
            return;
        }
        frame->first = pins;
        if (frame_is_strike(frame)) {
            frame->second = 0;
            frame->state = PENDING;
        } else {
            frame->state = BEFORE_2ND;
        }
    } else {
        printf("invalid event on %s.\n", frame_state_to_string(frame->state));
    }
}

static void frame_before_2nd_proc(Frame *frame, EventType event, int pins) {
    if (event == PINS) {
        if (pins < 0 || pins > (10 - frame->first)) {
            printf("invalid pins: %d\n", pins);
            return;
        }
        frame->second = pins;
        if (frame_is_spare(frame)) {
            frame->state = PENDING;
        } else {
            frame->state = FIXED;
        }
    } else {
        printf("invalid event on %s.\n", frame_state_to_string(frame->state));
    }
}

void frame_action(Frame *frame, EventType event, int pins) {
    switch (frame->state) { // <1>
    case RESERVED: // <2>
        if (event == SETUP) {
            frame->state = BEFORE_1ST;
        } else {
            printf("invalid event is ignored.\n");
        }
        break;
    case BEFORE_1ST: // <3>
        frame_before_1st_proc(frame, event, pins);
        break;
    case BEFORE_2ND: // <4>
        frame_before_2nd_proc(frame, event, pins);
        break;
    case PENDING: // <5>
        if (event == DETERMINE) {
            frame->state = FIXED;
        }
        break;
    case FIXED:
        printf("fixed.\n");
        break;
    }
}

int frame_score(const Frame *frame) {
    return frame->first + frame->second + frame->spare_bonus + frame->strike_bonus;
}

bool frame_is_strike(const Frame *frame) {
    return frame->first == 10;
}

bool frame_is_spare(const Frame *frame) {
    return frame->first < 10 && (frame->first + frame->second) == 10;
}

bool frame_is_miss(const Frame *frame) {
    return frame->first < 10 && frame->second == 0 && frame->state == FIXED;
}

bool frame_is_gutter(const Frame *frame) {
    return frame->first == 0;
}

bool frame_is_fixed(const Frame *frame) {
    return frame->state == FIXED;
}

void frame_print(const Frame *frame) {
    char total_str[8];
    if (frame->state == FIXED) {
        snprintf(total_str, sizeof(total_str), "%5d", frame->total);
    } else {
        snprintf(total_str, sizeof(total_str), "   .");
    }
    printf("|%2d|%3d|%3d|%5s|%11d|%11d|%12d|%11s|\n",
           frame->frame_no, frame->first, frame->second, total_str,
           frame_score(frame), frame->spare_bonus, frame->strike_bonus,
           frame_state_to_string(frame->state));
}

/* =================================================================
 * Score実装
 * ================================================================= */

void score_init(Score *score, const char *name) { // <1>
    generate_id(score->id);
    strncpy(score->player, name, MAX_NAME_LEN - 1);
    score->player[MAX_NAME_LEN - 1] = '\0';
    score->fno = 1;
    score->state = WAIT_FOR_1ST; // <2>

    /* ダミーフレーム含めて初期化 (-1, 0はダミー) */
    for (int i = 0; i < MAX_FRAMES; i++) {
        frame_init(&score->frames[i], i - 1);
    }

    frame_action(score_frame(score, score->fno), SETUP, 0); // <3>
}

int score_fno2idx(int fno) { // <4>
    return fno + 1; /* frame number 1 => array index 2 (0 origin) */
}

Frame* score_frame(Score *score, int fno) {
    return &score->frames[score_fno2idx(fno)];
}

void score_go_next_frame(Score *score) { // <5>
    score->fno += 1;
    frame_action(score_frame(score, score->fno), SETUP, 0);
}

Frame* score_current(Score *score) { // <6>
    return score_frame(score, score->fno);
}

Frame* score_prev(Score *score) { // <7>
    return score_frame(score, score->fno - 1);
}

Frame* score_pprev(Score *score) { // <8>
    return score_frame(score, score->fno - 2);
}

void score_calc_spare_bonus_after_1st(Score *score) { // <9>
    Frame *prev = score_prev(score);
    Frame *current = score_current(score);

    if (!frame_is_spare(prev)) {
        return;
    }

    prev->spare_bonus = current->first;
    frame_action(prev, DETERMINE, 0);
}

void score_calc_strike_bonus_after_1st(Score *score) { // <10>
    Frame *prev = score_prev(score);
    Frame *pprev = score_pprev(score);
    Frame *current = score_current(score);

    if (!(frame_is_strike(prev) && frame_is_strike(pprev))) {
        return;
    }

    pprev->strike_bonus = prev->first + current->first;
    frame_action(pprev, DETERMINE, 0);
}

void score_calc_strike_bonus_after_2nd(Score *score) { // <11>
    Frame *prev = score_prev(score);
    Frame *current = score_current(score);

    if (!frame_is_strike(prev)) {
        return;
    }

    prev->strike_bonus = current->first + current->second;
    frame_action(prev, DETERMINE, 0);
}

void score_update_total(Score *score) { // <12>
    for (int i = 1; i < MAX_FRAMES - 1; i++) {
        score->frames[i + 1].total =
            score->frames[i].total + frame_score(&score->frames[i + 1]);
    }
}

bool score_is_finished(const Score *score) { // <13>
    return frame_is_fixed(score_frame((Score*)score, 10));
}

static void score_wait_for_1st_proc(Score *score, int pins) { // <1>
    Frame *current = score_current(score);

    frame_action(current, PINS, pins); // <2>
    score_calc_spare_bonus_after_1st(score);
    score_calc_strike_bonus_after_1st(score);
    score_update_total(score);

    if (score_is_finished(score)) { // <3>
        score->state = FINISHED;
    } else if (frame_is_strike(current)) { // <4>
        score->state = WAIT_FOR_1ST;
        score_go_next_frame(score);
    } else {
        score->state = WAIT_FOR_2ND; // <5>
    }
}

static void score_wait_for_2nd_proc(Score *score, int pins) { // <6>
    Frame *current = score_current(score);

    frame_action(current, PINS, pins); // <7>
    score_calc_strike_bonus_after_2nd(score);
    score_update_total(score);

    if (score_is_finished(score)) { // <8>
        score->state = FINISHED;
    } else {
        score->state = WAIT_FOR_1ST; // <9>
        score_go_next_frame(score);
    }
}

void score_scoring(Score *score, int pins) { // <10>
    switch (score->state) {
        case WAIT_FOR_1ST:
            score_wait_for_1st_proc(score, pins);
            break;
        case WAIT_FOR_2ND:
            score_wait_for_2nd_proc(score, pins);
            break;
        case FINISHED:
            printf("finished\n");
            break;
    }
}

void score_print(const Score *score) { // <11>
    printf("Player:%s, Score(id: %s), Frame:%d\n",
           score->player, score->id, score->fno);
    printf("|No|1st|2nd|Total|Frame Score|Spare Bonus|Strike Bonus|Frame State|\n");
    for (int i = 0; i < MAX_FRAMES; i++) {
        frame_print(&score->frames[i]);
    }
}

/* =================================================================
 * Game実装
 * ================================================================= */

void game_init(Game *game) {
    generate_id(game->id); // <1>
    game->turn = 0;
    game->player_count = 0;
}

void game_entry(Game *game, const char *name) { // <2>
    if (game->player_count >= MAX_PLAYERS) {
        printf("Maximum number of players reached.\n");
        return;
    }
    score_init(&game->scores[game->player_count], name);
    game->player_count++;
}

const char* game_turn_player_name(const Game *game) { // <3>
    return game->scores[game->turn].player;
}

void game_go_next_turn(Game *game) { // <4>
    game->turn = (game->turn + 1) % game->player_count;
}

void game_playing(Game *game, int score_index, int pins) { // <5>
    Score *score = &game->scores[score_index];
    score_scoring(score, pins);

    if (score->fno > 10) {
        if (score_is_finished(score)) { // <6>
            game_go_next_turn(game);
        }
    } else if (score_current(score)->state == BEFORE_1ST) { // <7>
        game_go_next_turn(game);
    }
}

bool game_is_finished(const Game *game) { // <8>
    for (int i = 0; i < game->player_count; i++) {
        if (!score_is_finished(&game->scores[i])) {
            return false;
        }
    }
    return true;
}

void game_print(const Game *game) { // <9>
    printf("Game(id:%s)\n", game->id);
    for (int i = 0; i < game->player_count; i++) {
        score_print(&game->scores[i]);
    }
}

/* =================================================================
 * ScoreSheet実装
 * ================================================================= */

void scoresheet_init(ScoreSheet *sheet, time_t date) { // <1>
    generate_id(sheet->id);  // <2>
    sheet->play_date = date;
    sheet->game_count = 0;
}

void scoresheet_add_game(ScoreSheet *sheet, int games) { // <3>
    for (int i = 0; i < games && sheet->game_count < MAX_GAMES; i++) {
        game_init(&sheet->games[sheet->game_count]);
        sheet->game_count++;
    }
}

void scoresheet_print(const ScoreSheet *sheet) { // <4>
    char date_str[64];
    struct tm *tm_info = localtime(&sheet->play_date);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Score Sheet Date: %s (id:%s)\n", date_str, sheet->id);
}
