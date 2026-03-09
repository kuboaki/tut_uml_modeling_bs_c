# ボーリングチュートリアル C言語版 - ドキュメントガイド

## 概要

このディレクトリには、UMLモデリングチュートリアル（C言語版）のAsciiDocソースファイルが含まれています。

## ファイル構成

### メインドキュメント

- `tut_uml_modeling_bs_c.adoc` - メインのドキュメントファイル

### 章ごとのファイル

- `tut_uml_modeling_bs01_c.adoc` - 第1章: 準備（C言語版）
- `tut_uml_modeling_bs02.adoc` - 第2章: スコアシートの構造を調べる
- `tut_uml_modeling_bs03_c.adoc` - 第3章: モデルとコードの対応づけ（C言語版）
- `tut_uml_modeling_bs04.adoc` - 第4章: スコアやフレームの状態を調べる
- `tut_uml_modeling_bs05_c.adoc` - 第5章: できあがったプログラムを試す（C言語版）

### 付帯ファイル

- `front_matter.adoc` - ドキュメントのメタデータと設定
- `revnumber.adoc` - バージョン番号
- `preface.adoc` - はじめに
- `copyright.adoc` - 著作権表示
- `attributes.adoc` - 日本語のラベル定義
- `image_size_matter.adoc` - 画像サイズの設定
- `appendix-01.adoc` - 付録
- `bibliography.adoc` - 参考文献

## C言語版で変更された部分

### 第1章（tut_uml_modeling_bs01_c.adoc）

- Rubyのインストール手順 → C言語コンパイラのインストール手順に変更
- Windows（MinGW-w64/Visual Studio）、macOS、Linuxの環境構築を追加
- C言語の基礎知識への参考リンクを追加

### 第3章（tut_uml_modeling_bs03_c.adoc）

- Rubyのクラス定義 → C言語の構造体定義に変更
- Rubyのメソッド → C言語の関数に変更
- Rubyのシンボル → C言語のenumに変更
- すべてのコード例をC言語で書き直し
- C言語でのカプセル化（static関数）の説明を追加

### 第5章（tut_uml_modeling_bs05_c.adoc）

- `ruby score.rb` → `make` および `./bowling` に変更
- ビルドプロセスの説明を追加
- C言語版とRuby版の違いの説明を追加
- コンパイラの動作確認方法を追加

### front_matter.adoc

- タイトルに「（C言語版）」を追加
- `:ruby-version:` → `:c-standard:`, `:gcc-version:`, `:clang-version:` に変更

### revnumber.adoc

- `pdf_0290` → `c_0100` に変更

## PDFの生成方法

### 前提条件

AsciiDocからPDFを生成するには、以下のツールが必要です：

1. **Ruby** - AsciiDocのツールチェーンを実行するため
2. **asciidoctor** - AsciiDocをHTMLやPDFに変換
3. **asciidoctor-pdf** - PDFファイルを生成
4. **asciidoctor-diagram** (オプション) - PlantUML図を含める場合

### インストール手順

#### Rubyのインストール

Rubyがインストールされていない場合：

**Windows:**
```bash
# RubyInstallerをダウンロードしてインストール
# https://rubyinstaller.org/
```

**macOS:**
```bash
brew install ruby
```

**Linux:**
```bash
sudo apt-get install ruby-full  # Ubuntu/Debian
sudo dnf install ruby           # Fedora
```

#### AsciiDoctorのインストール

```bash
gem install asciidoctor
gem install asciidoctor-pdf
gem install asciidoctor-diagram  # PlantUML図を使う場合
```

### HTMLの生成

```bash
asciidoctor tut_uml_modeling_bs_c.adoc
```

生成されたファイル: `tut_uml_modeling_bs_c.html`

### PDFの生成

```bash
asciidoctor-pdf tut_uml_modeling_bs_c.adoc
```

生成されたファイル: `tut_uml_modeling_bs_c.pdf`

### PlantUML図を含む場合

PlantUML図を含める場合は、追加で以下が必要です：

1. **Java** - PlantUMLを実行するため
2. **Graphviz** - 図の描画エンジン

```bash
# PlantUML図を有効にしてPDF生成
asciidoctor-pdf -r asciidoctor-diagram tut_uml_modeling_bs_c.adoc
```

## カスタマイズ

### テーマのカスタマイズ

PDFのデザインをカスタマイズするには、`front_matter.adoc`で指定されているテーマファイルを編集します：

```
:pdf-theme: ./theme/pdf/mystyle-theme.yml
```

### 画像の配置

画像ファイルは`:imagesdir:`で指定されたディレクトリに配置します：

```
:imagesdir: images
```

### ソースコードの配置

ソースコードファイルは`:sourcesdir:`で指定されたディレクトリに配置します：

```
:sourcesdir: codes
```

## ビルドスクリプトの例

### Makefile（ドキュメント用）

```makefile
# Makefile for documentation

DOC = tut_uml_modeling_bs_c
ADOC = $(DOC).adoc

.PHONY: all html pdf clean

all: html pdf

html:
	asciidoctor $(ADOC)

pdf:
	asciidoctor-pdf $(ADOC)

pdf-with-diagrams:
	asciidoctor-pdf -r asciidoctor-diagram $(ADOC)

clean:
	rm -f $(DOC).html $(DOC).pdf
```

使い方：
```bash
make html       # HTMLを生成
make pdf        # PDFを生成
make clean      # 生成されたファイルを削除
```

## トラブルシューティング

### 日本語フォントの問題

PDFで日本語が表示されない場合、日本語フォントを含むテーマファイルを使用してください：

```yaml
# theme/pdf/mystyle-theme.yml
font:
  catalog:
    merge: true
    Noto Serif JP:
      normal: path/to/NotoSerifJP-Regular.otf
      bold: path/to/NotoSerifJP-Bold.otf
```

### 画像が表示されない

画像ファイルが見つからない場合、`:imagesdir:`のパスを確認してください。
相対パスは`.adoc`ファイルの場所からではなく、ドキュメントのルートディレクトリから解決されます。

### コードのシンタックスハイライトが効かない

`front_matter.adoc`で適切なシンタックスハイライターが設定されているか確認してください：

```
:source-highlighter: rouge
```

または

```
:source-highlighter: pygments
```

## 元のRuby版との互換性

元のRuby版のドキュメントとの違い：

1. **コード例**: すべてのRubyコード例がC言語に置き換えられています
2. **環境構築**: Ruby環境ではなくC言語開発環境の構築を説明
3. **実行方法**: `ruby score.rb` から `make` と `./bowling` に変更

その他のモデリングやUMLに関する説明は、元のバージョンと同じです。

## ライセンス

このドキュメントは、元のRuby版と同じライセンスに従います。
詳細は`copyright.adoc`を参照してください。

## 貢献

ドキュメントの改善提案やバグ報告は歓迎します。

## 参考リンク

- AsciiDoc公式サイト: https://asciidoc.org/
- Asciidoctor: https://asciidoctor.org/
- Asciidoctor PDF: https://github.com/asciidoctor/asciidoctor-pdf
- PlantUML: https://plantuml.com/
