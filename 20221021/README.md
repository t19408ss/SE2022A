# 実行結果 ／ Execution Result

```shell:zsh
20221021 % make           
g++ -c -Wall -g -O3 -std=c++1z sort.cc
g++ -o sort sort.o lib.o -lm -lpthread -g
20221021 % ./sort 1000
Insert: 0 ms 
Quick: 0 ms 
Count: 6 ms 
std::sort: 0 ms 
20221021 % ./sort 10000  
Insert: 10 ms 
Quick: 0 ms 
Count: 7 ms 
std::sort: 0 ms 
20221021 % ./sort 100000
Insert: 937 ms 
Quick: 5 ms 
Count: 8 ms 
std::sort: 6 ms 
20221021 % ./sort 1000000
Insert: 102638 ms 
Quick: 82 ms 
Count: 47 ms 
std::sort: 83 ms 
20221021 % ./sort 10000000
Insert: 11028402 ms
Quick: 748 ms 
Count: 1227 ms 
std::sort: 701 ms 
```