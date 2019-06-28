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

uint64_t tsc_counter(void) {
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //add codes here
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void friend_main() {
  //printf sample
  //OFFLOAD_FUNC(printf, "hello\n");

  // コア0以外は即座に終了
  if(__sync_fetch_and_add(&cnt, 1) != 0 ){
    __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
    return;
  }

  for(int i = 0; i < 0x20; i++) {
    SHARED_SYMBOL(test_value[i]) = tsc_counter();
  }

  __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
}
