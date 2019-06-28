#include <toshokan/friend/export.h>
#include <toshokan/friend/offload.h>
#include "shared.h"

int64_t SHARED_SYMBOL(sync_flag);
uint64_t SHARED_SYMBOL(test_value[0x20]);
void* SHARED_SYMBOL(hpet_addr);

int cnt = 0;
//ローカル環境でprintfデバッグしたいときは，
//以下の行とhakase.ccの7行目をコメントアウトを外してください
//int (*EXPORTED_SYMBOL(printf))(const char *format, ...);
//
//<printfの使い方>
//OFFLOAD_FUNC(printf,args...);
//のようにOFFLOAD_FUNC経由で呼び出します

uint64_t get_main_counter_value(void *reg_base) {
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //add codes here
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

struct Page {
  uint64_t entry[512];
} __attribute__((aligned(4096)));

Page pml4t;
Page pdpt;
Page pd;
Page pt;

void friend_main() {
  static const size_t k256TB = 256UL * 1024 * 1024 * 1024 * 1024;
  static const size_t k512GB = 512UL * 1024 * 1024 * 1024;
  static const size_t k1GB = 1024UL * 1024 * 1024;
  static const size_t k2MB = 2UL * 1024 * 1024;
  static const size_t k4KB = 4UL * 1024;

  static const size_t kDeployPhysAddrStart = 0x40000000UL;
  static const size_t kDeployPhysAddrEnd = 0x60000000UL;

  //printf sample
  //OFFLOAD_FUNC(printf, "hello\n");

  // コア0以外は即座に終了
  if(__sync_fetch_and_add(&cnt, 1) != 0 ){
    __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
    return;
  }

  pml4t.entry[(kDeployPhysAddrStart % k256TB) / k512GB] =
      (size_t)(&pdpt) | (1 << 0) | (1 << 1) |(1 << 2);
  pdpt.entry[(kDeployPhysAddrStart % k512GB) / k1GB] =
      (size_t)(&pd) | (1 << 0) | (1 << 1) |(1 << 2);

  for (int i = 0; i < 512; i++) {
    pd.entry[i] = pt.entry[i] = 0;
  }
  for (size_t addr = kDeployPhysAddrStart; addr < kDeployPhysAddrEnd; addr += k2MB) {
    pd.entry[(addr % k1GB) / k2MB] = addr | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 7);
  }

  pd.entry[((uint64_t)0x70000000ULL % k1GB) / k2MB] = (size_t)(&pt) | (1 << 0) | (1 << 1) | (1 << 2);
  pt.entry[((uint64_t)0x70000000ULL % k2MB) / k4KB] = (size_t)SHARED_SYMBOL(hpet_addr) | (1 << 0) | (1 << 1) | (1 << 2);

  asm volatile("movq %0, %%cr3"::"r"((uint64_t)&pml4t ));

  uint32_t* reg_base = (uint32_t*)0x70000000UL;
  
  for(int i = 0; i < 0x20; i++) {
    SHARED_SYMBOL(test_value[i]) = get_main_counter_value(reg_base);
  }

  __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
}
