# 課題1 二分探索 ／ Q1 Binary Search

基本に授業スライドの`int binary_search(int K, int m, int n)`を使用し、引数を追加。

追加した引数：
- `c` ... カウンタ（Stepの回数を記録）
- `a` ... 検索する配列

```shell:zsh
20221014 % gcc -o binary binary.c -Wall
20221014 % ./binary                    
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 
Key? 4
Found (Step = 5)
20221014 % ./binary
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 
Key? 30
Not found.
```

# 課題2 ハッシュ構造 ／ Q2 Hash

実行ごとに乱数を指定した個数だけ発生させ、Hash構造に格納する形とした。
（SEEDを変更していないため毎度同じ数が発生する）

`int hash_insert`がHash構造に数値を格納する関数、`int hash_search`が数値の検索を行う関数。

```shell:zsh
20221014 % gcc -o hash hash.c -Wall
20221014 % ./hash                  
1 132 756 459 533 219 48 679 680 935 384 520 831 35 54 530 672 8 384 67 
Key? 756
Found (Step = 1)
20221014 % ./hash
1 132 756 459 533 219 48 679 680 935 384 520 831 35 54 530 672 8 384 67 
Key? 530
Found (Step = 3)
20221014 % ./hash
1 132 756 459 533 219 48 679 680 935 384 520 831 35 54 530 672 8 384 67 
Key? 180
Not found.
```
