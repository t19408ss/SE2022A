# 実行結果 ／ Execution Result

```shell:zsh
20221028 % make    
g++ -g -c -Wall -O2 bptree.cc
g++ -lm -pthread  -o bptree bptree.o
20221028 % ./bptree
-----Insert-----
Key: 1
[1]
Key: 2
[1 2]
Key: 3
[1 2 3]
20221028 % ./bptree
-----Insert-----
Key: 3
[3]
Key: 2
[2 3]
Key: 1
[1 2 3]
Key: 4
[1 2 3]
Key: ^C

```