#include <toshokan/friend/export.h>
#include <toshokan/friend/offload.h>
#include "shared.h"

int64_t SHARED_SYMBOL(sync_flag);
uint64_t SHARED_SYMBOL(test_value);

int cnt = 0;
//ローカル環境でprintfデバッグしたいときは，
//以下の行とhakase.ccの7行目をコメントアウトを外してください
//int (*EXPORTED_SYMBOL(printf))(const char *format, ...);
//
//<printfの使い方>
//OFFLOAD_FUNC(printf,args...);
//のようにOFFLOAD_FUNC経由で呼び出します

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


  uint64_t *kadai_a_virt_addr = (uint64_t*) 0x71230000UL;
  uint64_t kadai_a_phys_addr = 0x70000000UL;

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

  *(uint64_t*)0x70000000ULL = 0x12345678;

  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //add codes here
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  asm volatile("movq %0, %%cr3"::"r"((uint64_t)&pml4t ));

  SHARED_SYMBOL(test_value) = *(uint64_t*)0x71230000ULL;

  __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
}
