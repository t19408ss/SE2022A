# プログラムの仕様 ／ Program Specifications

1. 1000,000レコードを挿入

2. `pthread`のスレッドを6個用意し、5つをトランザクション（ランダムに選択した16のレコードについて`update`する、nowait方式を使用）の実行に、1つを時間測定に当てる

3. 時間測定（3秒）の間、トランザクションを無限繰り返しで実行し、成功（コミット）数と失敗（アボート）数、合計ををカウントして表示（スループット）

以上の手順をB+木のIndexを使用する場合/しない場合で測定する


# 実行結果 ／ Execution Result

```shell:zsh
(base) ho-h-oh@HOHOH 20230106 % make all  
g++ -g -c -Wall -O2 simpledb.cc
g++ -lm -pthread  -o simpledb simpledb.o
(base) ho-h-oh@HOHOH 20230106 % ./simpledb

==== Insert data ==========================
Inserted key  100000:    2546
Inserted key  200000:    1798
Inserted key  300000:    6091
Inserted key  400000:     699
Inserted key  500000:    6884
Inserted key  600000:    9277
Inserted key  700000:    1424
Inserted key  800000:    3454
Inserted key  900000:    8557
Inserted key 1000000:     968

== Transacton throughput : with index =====
Number of Commit:          4160428
Number of Abort:          22782389
Total:                    26942817

== Transacton throughput : without index ==
Number of Commit:             5917
Number of Abort:          47850821
Total:                    47856738
```