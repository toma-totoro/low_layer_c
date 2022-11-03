#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    # 9ccを実行してアセンブラ・ファイルを出力する。
    ./9cc "$input" > tmp.s
    # (TODO=>DONE)9cc の戻り値をチェックし正常終了でないとき、メッセージを表示して、以降の処理をスキップする。
    # (TODO)本当はパースできない文字列の時の確認方法を考える必要あり。
    if [ $? -ne 0 ]; then
        echo 9cc実行時エラー
        exit 1
    fi
    # アセンブルし、バイナリを作成する。
    cc -o tmp tmp.s
    # アセンブルしたバイナリを実行
    ./tmp
    # 実行結果（バイナリの戻り値）を取得
    actual="$?"

    # 期待値との比較
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi

}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 11 "0-5+20-4"
assert 11 " 0 - 5 + 20 - 4"
assert 21 "05+000020-04"
assert 21 "5 +20-4"

# 演算子エラー
assert 21 "+5 +20-4"

# 数値エラー
assert 21 "5 ++20-4"

# トークナイズ・エラー
assert 0 "@0"
echo OK