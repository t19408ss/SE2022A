# プログラムの仕様 ／ Program Specifications

1. 10,000,000個のデータを挿入

2. `pthread`のスレッドを10個用意し、以下の配分で並列処理

    操作 | 順序 | スレッド数 |
    :--- | :--- | ---: |
    検索（`read`）| 昇順（1, 2, ..., 10000000） | 1
    検索 | 降順（10000000, 9999999, ..., 1） | 1
    検索 | ランダム（シャッフル） | 1
    検索と更新（`read`→`write`）| 昇順 | 2
    検索と更新| 降順 | 2
    検索と更新| ランダム | 3

    - それぞれ全てのデータに対して処理

    - 検索では、特定の`key`が持つデータの`val`を`read`する。

    - 更新では、`read`した`val`に`1`を加えた値を`write`する。よって、全てのデータの`val`が`7`になっていれば成功。

3. 全てのデータの`val`が`7`になっているか確認

※1000000個の挿入／検索／更新完了ごとに標準出力。


# 実行結果 ／ Execution Result

```shell:zsh
20221209 % make all
g++ -g -c -Wall -O2 bptree.cc
g++ -lm -pthread  -o bptree bptree.o
20221209 % ./bptree

==== Insert ===============
Inserted: 1000000
Inserted: 2000000
Inserted: 3000000
Inserted: 4000000
Inserted: 5000000
Inserted: 6000000
Inserted: 7000000
Inserted: 8000000
Inserted: 9000000
Inserted: 10000000

==== Search and Update ====
Thread 5: key 10000000  update(0 => 1)
Thread 6: key 10000000  update(1 => 2)
Thread 0: key 1000000   read(0)
Thread 1: key 1000000   read(0)
Thread 5: key 9000000   update(0 => 1)
Thread 4: key 1000000   update(0 => 1)
Thread 3: key 1000000   update(1 => 2)
Thread 6: key 9000000   update(1 => 2)
Thread 0: key 2000000   read(0)
Thread 1: key 2000000   read(0)
Thread 3: key 2000000   update(0 => 1)
Thread 4: key 2000000   update(1 => 2)
Thread 6: key 8000000   update(0 => 1)
Thread 5: key 8000000   update(1 => 2)
Thread 1: key 3000000   read(0)
Thread 0: key 3000000   read(0)
Thread 3: key 3000000   update(0 => 1)
Thread 4: key 3000000   update(1 => 2)
Thread 2: key 2016778   read(2)
Thread 6: key 7000000   update(0 => 1)
Thread 5: key 7000000   update(1 => 2)
Thread 7: key 8183040   update(2 => 3)
Thread 1: key 4000000   read(0)
Thread 0: key 4000000   read(0)
Thread 9: key 1802767   update(2 => 3)
Thread 3: key 4000000   update(0 => 1)
Thread 8: key 7335115   update(2 => 3)
Thread 4: key 4000000   update(1 => 2)
Thread 6: key 6000000   update(0 => 1)
Thread 5: key 6000000   update(1 => 2)
Thread 1: key 5000000   read(0)
Thread 0: key 5000000   read(0)
Thread 3: key 5000000   update(0 => 1)
Thread 4: key 5000000   update(1 => 2)
Thread 5: key 5000000   update(2 => 3)
Thread 6: key 5000000   update(3 => 4)
Thread 1: key 6000000   read(2)
Thread 0: key 6000000   read(2)
Thread 4: key 6000000   update(2 => 3)
Thread 3: key 6000000   update(3 => 4)
Thread 6: key 4000000   update(2 => 3)
Thread 5: key 4000000   update(3 => 4)
Thread 1: key 7000000   read(2)
Thread 0: key 7000000   read(2)
Thread 4: key 7000000   update(2 => 3)
Thread 3: key 7000000   update(3 => 4)
Thread 6: key 3000000   update(2 => 3)
Thread 5: key 3000000   update(3 => 4)
Thread 1: key 8000000   read(2)
Thread 0: key 8000000   read(2)
Thread 4: key 8000000   update(2 => 3)
Thread 3: key 8000000   update(3 => 4)
Thread 5: key 2000000   update(3 => 4)
Thread 6: key 2000000   update(4 => 5)
Thread 1: key 9000000   read(2)
Thread 0: key 9000000   read(2)
Thread 3: key 9000000   update(2 => 3)
Thread 4: key 9000000   update(3 => 4)
Thread 1: key 10000000  read(2)
Thread 6: key 1000000   update(2 => 3)
Thread 0: key 10000000  read(2)
Thread 5: key 1000000   update(3 => 4)
Thread 4: key 10000000  update(2 => 3)
Thread 3: key 10000000  update(3 => 4)
Thread 2: key 2746441   read(4)
Thread 9: key 4256332   update(4 => 5)
Thread 7: key 3227941   update(4 => 5)
Thread 8: key 1089097   update(5 => 6)
Thread 2: key 7762625   read(5)
Thread 9: key 5999651   update(5 => 6)
Thread 8: key 6176508   update(5 => 6)
Thread 7: key 9648165   update(5 => 6)
Thread 2: key 9067045   read(5)
Thread 9: key 1230224   update(4 => 5)
Thread 7: key 5806625   update(6 => 7)
Thread 8: key 2386700   update(5 => 6)
Thread 8: key 3165711   update(6 => 7)
Thread 9: key 5199692   update(4 => 5)
Thread 2: key 6826048   read(4)
Thread 7: key 5113905   update(4 => 5)
Thread 8: key 8732145   update(6 => 7)
Thread 2: key 2422261   read(6)
Thread 9: key 9574720   update(6 => 7)
Thread 7: key 7215890   update(6 => 7)
Thread 8: key 8957399   update(6 => 7)
Thread 2: key 7258149   read(7)
Thread 9: key 2873987   update(5 => 6)
Thread 7: key 8702115   update(5 => 6)
Thread 2: key 7449704   read(7)
Thread 8: key 5049903   update(5 => 6)
Thread 9: key 9231741   update(5 => 6)
Thread 7: key 870071    update(5 => 6)
Thread 2: key 7780427   read(5)
Thread 8: key 9454101   update(6 => 7)
Thread 9: key 610326    update(5 => 6)
Thread 7: key 5837656   update(6 => 7)
Thread 2: key 6859886   read(7)
Thread 8: key 3190922   update(6 => 7)
Thread 9: key 3580505   update(5 => 6)
Thread 7: key 5091274   update(6 => 7)

==== Check =================
Expected: val = 7
Checked: key 1000000    (val = 7)
Checked: key 2000000    (val = 7)
Checked: key 3000000    (val = 7)
Checked: key 4000000    (val = 7)
Checked: key 5000000    (val = 7)
Checked: key 6000000    (val = 7)
Checked: key 7000000    (val = 7)
Checked: key 8000000    (val = 7)
Checked: key 9000000    (val = 7)
Checked: key 10000000   (val = 7)
Check Passed!
```
