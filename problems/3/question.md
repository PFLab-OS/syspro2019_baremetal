### 問題
今、スレッドセーフでない最大二分ヒープが実装されている。  
heap_insert, heap_delete_max関数は、関数の開始時にlock、関数の終了時にunlock関数を呼ぶが、現時点では両関数の中身は空となっている。  
spinlockを実装し、ヒープの実装をスレッドセーフにせよ。  
spinlockはナイーブなtest-and-setループで良い。  
#### [githubでみる](https://github.com/PFLab-OS/syspro2019_baremetal/tree/master/problems/3)
