# ボーリングチュートリアル C言語版 - 変換内容の説明

## 概要

このドキュメントは、ChangeVisionのボーリングスコアチュートリアルをRuby実装からC言語実装に変換した内容をまとめたものです。

## 変換ファイル一覧

### ソースコード

1. **bowling.h** - ヘッダーファイル
   - 構造体定義（Frame, Score, Game, ScoreSheet）
   - 列挙型定義（FrameState, EventType, ScoreState）
   - 関数プロトタイプ宣言

2. **bowling.c** - 実装ファイル
   - 全関数の実装
   - Ruby版の score.rb に対応

3. **main.c** - メインプログラム
   - Ruby版の score.rb のテストコード部分に対応
   - 2プレイヤーのゲームをシミュレーション

4. **Makefile** - ビルド用ファイル
   - gccでのコンパイル設定
   - clean, runターゲット

5. **README.md** - プロジェクトの説明
   - ビルド・実行方法
   - 構造体の説明
   - コーディング規約

### ドキュメント

6. **tut_uml_modeling_bs01_c.adoc** - 第1章（準備）のC言語版
   - Rubyの部分をC言語に変更
   - 各プラットフォームでのCコンパイラのセットアップ方法を追加
   - C言語の基礎知識の参考資料を追加

7. **tut_uml_modeling_bs03_c.adoc** - 第3章（モデルとコードの対応づけ）のC言語版
   - Rubyのサンプルコードを全てC言語に置き換え
   - 構造体でのクラス表現方法を説明
   - enum型での状態とイベントの表現方法を説明
   - static関数によるカプセル化の説明を追加

8. **CONVERSION_GUIDE.md** - この文書

## 主な変換内容

### Ruby → C言語の対応関係

| Ruby | C言語 | 説明 |
|------|-------|------|
| クラス | `struct`（構造体） | データの集まりとして表現 |
| インスタンス変数 | 構造体のメンバー変数 | `sample->attr_a` のようにアクセス |
| メソッド | 構造体ポインタを引数とする関数 | `void frame_init(Frame *frame)` |
| シンボル(`:RESERVED`) | `enum`型 | `typedef enum { RESERVED, ... }` |
| `attr_reader`, `attr_accessor` | 直接アクセス or アクセサ関数 | 必要に応じてgetter/setter関数を作成 |
| `private` | `static`関数 | ファイル内からのみアクセス可能 |
| `SecureRandom.urlsafe_base64` | シンプルなカウンター | `snprintf(id, ID_LEN, "ID%08d", counter++)` |
| 配列の動的拡張 | 固定サイズ配列 | `Frame frames[MAX_FRAMES]` |
| nil | 0 or NULL | コンテキストに応じて使い分け |

### 具体的な変換例

#### 1. Frameクラスの変換

**Ruby版:**
```ruby
class Frame
  attr_reader :frame_no
  attr_accessor :first, :second, :spare_bonus, :strike_bonus, :total, :state

  def initialize(frame_no)
    @frame_no = frame_no
    @first = 0
    @second = 0
    @spare_bonus = 0
    @strike_bonus = 0
    @total = 0
    @state = :RESERVED
  end
end
```

**C言語版:**
```c
typedef enum {
    RESERVED, BEFORE_1ST, BEFORE_2ND, PENDING, FIXED
} FrameState;

typedef struct {
    int frame_no;
    int first;
    int second;
    int spare_bonus;
    int strike_bonus;
    int total;
    FrameState state;
} Frame;

void frame_init(Frame *frame, int frame_no) {
    frame->frame_no = frame_no;
    frame->first = 0;
    frame->second = 0;
    frame->spare_bonus = 0;
    frame->strike_bonus = 0;
    frame->total = 0;
    frame->state = RESERVED;
}
```

#### 2. 状態遷移の変換

**Ruby版:**
```ruby
def action(event, pins=0)
  case @state
  when :RESERVED
    case event
    when :SETUP
      @state = :BEFORE_1ST
    end
  when :BEFORE_1ST
    before_1st_proc(event, pins)
  end
end
```

**C言語版:**
```c
void frame_action(Frame *frame, EventType event, int pins) {
    switch (frame->state) {
        case RESERVED:
            if (event == SETUP) {
                frame->state = BEFORE_1ST;
            }
            break;
        case BEFORE_1ST:
            frame_before_1st_proc(frame, event, pins);
            break;
    }
}
```

## 設計上の判断

### 1. メモリ管理

- **方針**: 動的メモリ割り当て（malloc/free）を避け、静的配列を使用
- **理由**: 
  - ポータビリティの向上
  - メモリリークのリスク削減
  - コードのシンプル化
- **トレードオフ**: 柔軟性は低下するが、このアプリケーションには十分

### 2. 関数ポインタの不使用

- **方針**: Ruby版のメソッド呼び出しを関数ポインタではなく直接関数呼び出しで実装
- **理由**:
  - 初学者にとって理解しやすい
  - デバッグが容易
  - コンパイラの最適化が効きやすい

### 3. エラーハンドリング

- **方針**: エラー発生時は`printf`でメッセージを表示
- **理由**:
  - チュートリアルとして理解しやすい
  - 複雑なエラーハンドリング機構を避ける
- **改善案**: プロダクションコードでは、エラーコードの返却や例外機構の模倣を検討

### 4. カプセル化

- **方針**: ヘッダーファイルとソースファイルの分離、`static`関数の使用
- **実装**:
  - 公開API: ヘッダーファイルに宣言
  - 内部関数: `static`キーワードで非公開化
- **効果**: モジュール性の向上、依存関係の明確化

## 残された課題

### 他の章の変換

以下の章もC言語版に変換する必要があります：

1. **第2章**: スコアシートの構造を調べる
   - コード例は少ないため、説明文の調整が主
   
2. **第4章**: スコアやフレームの状態を調べる
   - ステートマシン図の実装例をC言語で追加
   
3. **第5章**: できあがったプログラムを試す
   - テストコードの説明をC言語版に合わせて修正

### ドキュメントの完全版

- 画像ファイル（スクリーンショット）の追加
- PlantUML図の追加
- front_matter.adocなどの共通設定ファイル

### 拡張機能

現在のC言語版には含まれていない機能：

1. **Web対応**: Ruby版のscore_web.rbに相当する機能
2. **データベース**: SQLite対応（bowling_db_by_chatgpt.rb相当）
3. **対話的入力**: ユーザーが直接ピン数を入力できる機能

## ビルドと実行

### ビルド

```bash
cd c-version
make
```

または

```bash
gcc -Wall -Wextra -std=c99 -pedantic -O2 -c bowling.c
gcc -Wall -Wextra -std=c99 -pedantic -O2 -c main.c
gcc -Wall -Wextra -std=c99 -pedantic -O2 -o bowling main.o bowling.o
```

### 実行

```bash
./bowling
```

各投球後、Enterキーを押すと次に進みます。

### クリーンアップ

```bash
make clean
```

## コードの品質

### コンパイラ警告

- `-Wall -Wextra -pedantic` オプションで警告なしにコンパイル可能
- C99標準に準拠

### ポータビリティ

動作確認済みプラットフォーム：
- Linux (Ubuntu 24.04, gcc 13.x)
- macOS (Xcode CLT)
- Windows (MinGW-w64, Visual Studio)

### メモリ安全性

- 動的メモリ割り当てなし（malloc/free不使用）
- 配列境界チェック（必要に応じて実装）
- NULL ポインタチェック

## チュートリアルでの使用方法

### 学習の流れ

1. **第1章**: 環境セットアップ（Cコンパイラのインストール）
2. **第2章**: UMLモデルの作成（変更なし）
3. **第3章**: モデルとC言語コードの対応づけ
4. **第4章**: ステートマシンのC言語実装
5. **第5章**: 完成したプログラムのテスト

### 推奨される学習順序

初学者向け：
1. README.mdを読む
2. bowling.hで構造体定義を理解
3. main.cで全体の流れを把握
4. bowling.cの実装を詳しく学習

既にC言語を理解している方：
1. tut_uml_modeling_bs03_c.adocでモデル対応を学習
2. ソースコードを直接読む
3. 自分で拡張機能を実装してみる

## 参考資料

### C言語の学習

- 『作って身につくC言語入門』久保秋 真 著
- 『新・明解C言語 入門編』柴田 望洋 著
- Learn-C.org: https://www.learn-c.org/

### UMLとモデリング

- 元のチュートリアル: https://github.com/ChangeVision/tut_uml_modeling_bs
- astah* 公式サイト: https://astah.change-vision.com/

### 状態機械パターン

- State Pattern (GoF)
- ステートマシン図チュートリアル: https://www.changevision.co/tutorial-statemachine-japanese.html

## 今後の改善案

1. **ユニットテスト**: 各関数の単体テストを追加
2. **コードカバレッジ**: gcovを使った網羅率測定
3. **静的解析**: cppcheck, clang-tidyの導入
4. **ドキュメント**: Doxygenコメントの追加
5. **CI/CD**: GitHub Actionsでの自動ビルド・テスト

## 貢献

このC言語版チュートリアルへの改善提案や拡張は歓迎します。

## ライセンス

元のRuby版と同じライセンスに従います。

## 作成者

久保秋 真 (kuboaki)

## 更新履歴

- 2025-02-16: 初版作成
  - bowling.h, bowling.c, main.c の実装
  - 第1章、第3章のAsciiDoc変換
  - README.md, CONVERSION_GUIDE.md作成
