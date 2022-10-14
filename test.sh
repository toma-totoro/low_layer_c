#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    # 9ccを実行してアセンブラ・ファイルを出力する。
    ./9cc "$input" > tmp.s
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
assert 11 "-5+20-4"
assert 0 "@0"
assert 21 "05+000020-04"
assert 0 "5 +20-4"
echo OK