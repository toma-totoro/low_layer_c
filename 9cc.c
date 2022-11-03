#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
# typedef とは既存のデータ型に新たな名前をつけるキーワードです。
# ポイントは、typedef では新たな名前を作ることはできるのですが、新たな型自体を作るわけではない点です。
# typedef では元々ある型を違う名前で使用するためのものであって、必ず元となる型が存在します。
*/
// kind of token
typedef enum{
    TK_RESERVED, // 記号
    TK_NUM,      // 数字トークン
    TK_EOF,      // 入力の終わりを表すトークン　
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token
{
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // トークンの型がTK_NUMのとき、その値
    char *str;      // トークン文字列
};

// 現在着目しているトークン
Token *token;

// エラーメッセージを出力する関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}
// 着目ているトークンが期待している記号のときには、トークンを１つ読み進めて、
// 真を返す。それ以外の場合には偽を返す
bool consume(char op) {
    if(token->kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }
    // 次のトークへ移動
    token = token->next;
    return true;
}
// 着目しているトークンが期待している記号のときには、トークンを１つ読み進める。
// それ以外の場合にはエラーを報告する。
// (??? consume関数との違いは)
void expect(char op) {
    if(token->kind != TK_RESERVED || token->str[0] != op){
        error("'%c'ではありません", op);
    }
    token = token->next;
}

// 着目しているトークンが数値の場合、トークンを１つ読み進めて
// その数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number(){
    if (token->kind != TK_NUM){
        error("数ではありません。");
    }
    int val = token->val; // 数値
    token = token->next; // 着目しているトークンを次へ
    return val;
}

// トークンが終了したかのチェック
bool at_eof(){
    return token->kind == TK_EOF;
}

// 新しいトークンを作成してcurに繋げる
// ?? curとは、lispの用語??
Token *new_token(TokenKind kind, Token *cur, char *str){
    // mallocでなくcallocでメモリを割り当てるのはなぜ？
    // ==> callocは割り当てたメモリをゼロクリアするため使用した。
    Token *tok = calloc(1, sizeof(Token)); // 新しいTokenのメモリ割り当て
    tok->kind = kind;
    tok->str = str;
    cur->next = tok; //カレントのTokenに、今割り当てたTokenを繋げる
    return tok; // 作成したTokenへのポインタを返す。

}

// 入力文字列pをトークナイズしてTokenを返す。
Token *tokenize(char *p){
    // 引数: p 入力文字列へのポインタ

    Token head; //先頭のTokenを用意して、それにTokenを繋いでいく
    head.next = NULL;
    Token *cur = &head;

    while(*p){
        //空白をスキップ
        if (isspace(*p)){
            p++;
            continue; //whileの先頭へ
        }

        if(*p == '+' || *p == '-'){
            // 演算子は１桁決め打ちなのでp++
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if(isdigit(*p)){
            // 数値の時は、strtolで値の取得＆pの位置を進める
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        //ここまで来るってことは、想定外の値
        //スペースでも、演算子でも、数値でもない
        error("トークナイズできません。");
    }
    // 末尾トークンを繋げる
    // new_tokenの戻りは不要なので、受け取らないってこと？
    new_token(TK_EOF, cur, p); //pはnullを指しているはず。
    return head.next; //トークナイズされた実質のtokenへのポンタを返す。
}



int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません。\n");
        return 1;
    }
    // トークナイズし、結果をグローバルの*tokenに格納する
    token = tokenize(argv[1]);

    /* お決まり */
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // 式の最初は数でなければならないので、それをチェックして
    // 最初のmov命令を出力
    printf("  mov rax, %d\n", expect_number());
    
    // `+ <数値>` or `- <数値>`というトークンの並びを消費しつつ
    // アセンブリコードを出力
    while (!at_eof()){
        if (consume('+')){
            printf("  add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("  sub rax, %d\n", expect_number());
    }

    /* アセンブラ終わり */ 
    printf("  ret\n");
    return 0;
}