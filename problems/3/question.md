### 問題
今、スレッドセーフ「でない」最大二分ヒープが実装されている。(heap_insert, heap_delete_max関数)  
heap_insert, heap_delete_max関数は、関数の開始時にlock、終了時にunlock関数を呼ぶが、両関数の中身は空となっている。
この2つの関数の中にspinlockを実装して、二分ヒープ全体の実装をスレッドセーフにせよ。  
ただし、spinlockはナイーブなtest-and-setループで良い。  
#### [githubでみる](https://github.com/PFLab-OS/syspro2019_baremetal/tree/master/problems/3)
