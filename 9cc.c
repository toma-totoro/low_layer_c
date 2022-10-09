#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません。\n");
        return 1;
    }
    char *p = argv[1];

    /* お決まり */
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    /*先頭の数字をレジスタにセット*/
    printf("  mov rax, %ld\n", strtol(p, &p, 10));
    while (*p){
        if (*p == '+'){
            p++;
            printf("  add rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if (*p == '-'){
            p++;
            printf("  sub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "予期しない文字です: %c\n", *p);
        return 1;
    }

    /* アセンブラ終わり */ 
    printf("  ret\n");
    return 0;
}