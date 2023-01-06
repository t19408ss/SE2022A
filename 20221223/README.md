# プログラムの仕様 ／ Program Specifications

1. 100レコードを挿入

2. `pthread`のスレッドを6個用意し、5つをトランザクション（ランダムに選択した16のレコードについて`read`または`write`する）実行に、1つを時間測定に当てる

3. 時間測定の間、トランザクションを無限繰り返しで実行し、成功（コミット）数と失敗（アボート）数をカウントして表示（スループット）

※プログラム内に`nowait`方式と`backoff`方式の両方を記載し、手動で切り替えて測定可能


# 実行結果 ／ Execution Result

- 条件
    - レコード数：100
    - 同時実行スレッド数：5（+時間測定1）
    - トランザクション内のオペレーション数：16
    - 測定時間：3秒
    - `backoff`方式の最大待ち時間：10マイクロ秒


- `nowait`方式

    ```shell:zsh
    20221223 % ./simpledb
    Number of Commit: 1
    Number of Abort: 25477950
    20221223 % ./simpledb
    Number of Commit: 930783
    Number of Abort: 36653935
    20221223 % ./simpledb
    Number of Commit: 59
    Number of Abort: 24221624
    ```

- `backoff`方式

    ```shell:zsh
    20221223 % ./simpledb
    20221223 % ./simpledb
    Number of Commit: 31
    Number of Abort: 723758
    20221223 % ./simpledb
    Number of Commit: 6530265
    Number of Abort: 832617
    20221223 % ./simpledb
    Number of Commit: 5139079
    Number of Abort: 612573
    ```
