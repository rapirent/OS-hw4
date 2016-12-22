# OS-hw4
## summary
-  author:丁國騰(Kuo Teng, Ding)
-  student_id:E94036209
-  class:資工三乙
-  title:hw4

##  Usage:
-  makefile:

```
  gcc -o hw4 hw4.c
```

- start the program:
    - use redirecte（資料流重導向）
```
    ./hw4 < [input_file] > [output_file]
```

## place replacement policies' pros and cons:

### LRU

#### 優點
- 較為準確，使page fault的比率較少發生

#### 缺點
- 需要有一個欄位維護page之進入時間（hit時更新），故有額外的空間需求
- 每次page fault發生時，都得尋找最少被用到的page（以迴圈尋找進入時間最久者），故有O(N)的cost（N為page數量）

### Random

#### 優點
- 沒有額外的運算或時間成本，因為只是單純的以亂數選擇替換者

#### 缺點
- 全憑運氣做出選擇，使得page fault rate時好時壞
### FIFO

#### 優點
- 雖然有額外的空間成本，但發生page fault時只需要O(1)的時間找到替換者（因為只是從queue中pop出）
- 相較random更好

#### 缺點
- 不一定最先進入的就最少被用，故沒有LRU來的精確
