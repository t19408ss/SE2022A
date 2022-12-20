# プログラムの仕様 ／ Program Specifications

- 挿入データ数、スレッド数（実行するトランザクションの数）をコマンド引数に取る
- データを挿入後、ランダムな`key`が持つ`data`の`val`を`read`あるいは`write`する操作を16回行うトランザクションを、コマンド引数で設定した数だけ生成する
- 実際にトランザクションを実行し、時間を測定する


# 実行結果 ／ Execution Result

```shell:zsh
20221216 % make all          
g++ -g -c -Wall -O2 simpledb.cc
g++ -lm -pthread  -o simpledb simpledb.o
20221216 % ./simpledb 1000 10

DATA LENGTH: 1000
NUMBER OF THREADS: 10

Database init ==> Inserted 1000 data!

Running Transactions <read only> .......... Finished!
        Time: 0.000322 s

Running Transactions <read and write> .......... Finished!
        Time: 0.000205 s

20221216 % ./simpledb 1000000 100

DATA LENGTH: 1000000
NUMBER OF THREADS: 100

Database init ==> Inserted 1000000 data!

Running Transactions <read only> .................................................................................................... Finished!
        Time: 0.001601 s

Running Transactions <read and write> .................................................................................................... Finished!
        Time: 0.001465 s

```

# スループットの測定 ／ Measure throughput

それぞれの項目ごとに10回実行し、所要時間の平均［単位:マイクロミリ秒］を記載。（小数点以下は切り捨て）

元データは`time_measure.csv`に記載。

データ件数＼スレッド数 |  | 10 | 50 | 100 | 500
:--- | :--- | ---: | ---: | ---: | ---:
1000 | `read` | 367 | 986 | 1830 | 11059
1000 | `read` & `write` | 210 | 858 | 1676 | 10600
|
1000000 | `read` | 192 | 846 | 1876 | 10577
1000000 | `read` & `write` | 160 | 759 | 1641 | 10389