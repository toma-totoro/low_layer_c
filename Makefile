CFLAGS=-std=c11 -g -static

9cc: 9cc.c

# 9ccという実行ファイルが9cc.cより古い場合に限り、makeは、テストを実行するより前に9ccをビルドしてくれます。
test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean