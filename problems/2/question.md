### 問題
PML4Tのk番目のエントリにPML4T自身の物理アドレスが設定されているとする。  
この時、整数k、と仮想アドレス virt_addrを受け取り、対応する物理アドレスを返す  
uint64_t get_phys_addr(uint64_t k, void *virt_addr)  
を実装せよ。  

ただし，kはpml4tエントリのインデックスとして有効なものである．また，virt_addrも有効なアドレスである．  
さらに，huge pageは使われていないものとする．  

#### [githubでみる](https://github.com/PFLab-OS/syspro2019_baremetal/tree/master/problems/2)
