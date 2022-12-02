# 実行結果 ／ Execution Result

```shell:zsh
20221202 % time ./counter_non_lock 
Counter: 260775758 (Ref. 1000000000)
./counter_non_lock  16.06s user 0.11s system 417% cpu 3.870 total
20221202 % time ./counter_pthread_lock 
Counter: 1000000000 (Ref. 1000000000)
./counter_pthread_lock  64.47s user 172.18s system 482% cpu 49.000 total
20221202 % time ./counter_tas
Counter: 1000000000 (Ref. 1000000000)
./counter_tas  536.04s user 1.64s system 519% cpu 1:43.55 total
20221202 % time ./counter_ttas         
Counter: 1000000000 (Ref. 1000000000)
./counter_ttas  63.81s user 130.64s system 459% cpu 42.314 total
20221202 % time ./counter_fetch_and_add 
Counter: 1000000000 (Ref. 1000000000)
./counter_fetch_and_add  97.17s user 0.41s system 489% cpu 19.953 total

```