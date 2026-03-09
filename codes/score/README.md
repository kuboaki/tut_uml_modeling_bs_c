# ボーリングスコアシステム - C言語実装

このディレクトリには、UMLモデリングチュートリアルで使用するボーリングスコアシステムのC言語実装が含まれています。

## 概要

このプログラムは、ボーリングのスコアを記録し、ストライクやスペアのボーナス計算を含む正確なスコア計算を行います。
元のRuby実装をポータブルなC言語(C99標準)に置き換えたものです。

## ファイル構成

```
.
├── bowling.h       # ヘッダーファイル（構造体と関数の宣言）
├── bowling.c       # 実装ファイル（関数の実装）
├── main.c          # メインプログラム（テストコード）
├── Makefile        # ビルド用Makefile
└── README.md       # このファイル
```

## 主な構造体

### Frame

1フレーム分のピン数やボーナスを記録する構造体です。

```c
typedef struct {
    int frame_no;        // フレーム番号
    int first;           // 1投目のピン数
    int second;          // 2投目のピン数
    int spare_bonus;     // スペアボーナス
    int strike_bonus;    // ストライクボーナス
    int total;           // 累計スコア
    FrameState state;    // 現在の状態
} Frame;
```

#### 状態遷移

Frameは以下の状態を持ちます：

- `RESERVED`: 予約済み（初期状態）
- `BEFORE_1ST`: 1投目待ち
- `BEFORE_2ND`: 2投目待ち
- `PENDING`: ボーナス確定待ち
- `FIXED`: 確定

### Score

1プレイヤーの10フレーム分のスコアを記録する構造体です。

```c
typedef struct {
    char id[ID_LEN];           // スコアID
    char player[MAX_NAME_LEN]; // プレイヤー名
    int fno;                   // 現在のフレーム番号
    Frame frames[MAX_FRAMES];  // 全フレームのデータ
    ScoreState state;          // 現在の状態
} Score;
```

### Game

複数プレイヤーの1ゲーム分のスコアを管理する構造体です。

```c
typedef struct {
    char id[ID_LEN];                  // ゲームID
    int turn;                         // 現在のターン
    Score scores[MAX_PLAYERS];        // 全プレイヤーのスコア
    int player_count;                 // プレイヤー数
} Game;
```

### ScoreSheet

複数ゲームの記録を管理する構造体です。

```c
typedef struct {
    char id[ID_LEN];             // スコアシートID
    time_t play_date;            // プレイ日時
    Game games[MAX_GAMES];       // 全ゲームのデータ
    int game_count;              // ゲーム数
} ScoreSheet;
```

## ビルド方法

### 前提条件

- C99対応のCコンパイラ（gcc, clang, Visual C++など）
- makeコマンド（オプション）

### makeを使用する場合

```bash
make
```

### 手動でコンパイルする場合

```bash
gcc -Wall -Wextra -std=c99 -pedantic -O2 -c bowling.c
gcc -Wall -Wextra -std=c99 -pedantic -O2 -c main.c
gcc -Wall -Wextra -std=c99 -pedantic -O2 -o bowling main.o bowling.o
```

## 実行方法

```bash
./bowling
```

プログラムを実行すると、2人のプレイヤー（「くぼあき」と「うえはら」）のゲームがシミュレーションされます。
各投球後、Enterキーを押すと次の投球に進みます。

## 実行例

```
Score Sheet Date: 2025-02-16 01:23:45 (id:ID00000000)

----------------------------------------
turn: くぼあき
input pins: 7
Press Enter to continue...
Game(id:ID00000001)
Player:くぼあき, Score(id: ID00000002), Frame:1
|No|1st|2nd|Total|Frame Score|Spare Bonus|Strike Bonus|Frame State|
|-1|  0|  0|    0|          0|          0|           0|      FIXED|
| 0|  0|  0|    0|          0|          0|           0|      FIXED|
| 1|  7|  0|   .|          7|          0|           0|  BEFORE_2ND|
...
```

## C言語版の特徴

### Ruby版との違い

1. **明示的な型定義**: 構造体と列挙型で厳密に型を定義
2. **メモリ管理**: 静的配列を使用し、動的メモリ割り当てを回避
3. **ポータビリティ**: C99標準に準拠し、主要なプラットフォームで動作
4. **カプセル化**: ヘッダーファイルとソースファイルの分離、static関数の使用

### 設計上の工夫

1. **状態機械パターン**: Ruby版と同じステートマシンの考え方を採用
2. **関数ポインタの回避**: 保守性とポータビリティのため、関数ポインタは使用せず
3. **エラーハンドリング**: printfでエラー状況を通知
4. **ID生成**: SecureRandom の代わりに、シンプルなカウンターベースのID生成

## コーディング規約

- インデント: スペース4個
- 命名規則:
  - 構造体: PascalCase (例: `Frame`, `ScoreSheet`)
  - 関数: snake_case + プレフィックス (例: `frame_init`, `score_scoring`)
  - 列挙型: UPPER_CASE (例: `BEFORE_1ST`, `PENDING`)
- ヘッダーガード: `#ifndef XXX_H` / `#define XXX_H`
- コメント: C言語スタイル (`/* ... */`) とC++スタイル (`//`) の併用

## 参考資料

- 元のRuby実装: `https://github.com/ChangeVision/tut_uml_modeling_bs`
- チュートリアルPDF: `tut_uml_modeling_bs.pdf`
- 『作って身につくC言語入門』（久保秋 真 著）

## ライセンス

このコードは教育目的で作成されており、元のRuby版と同じライセンスに従います。

## 作成者

久保秋 真 (kuboaki)
- Email: kuboaki@example.com
- GitHub: https://github.com/kuboaki

## 変更履歴

- 2025-02-16: C言語版の初版作成
