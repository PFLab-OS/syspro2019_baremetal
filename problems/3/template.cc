#include <toshokan/friend/export.h>
#include <toshokan/friend/offload.h>
#include "shared.h"

int64_t SHARED_SYMBOL(sync_flag);
uint64_t SHARED_SYMBOL(test_value);

#define HEAP_MAX_NODE_NUM 0x200 

int heap_array[HEAP_MAX_NODE_NUM];
int heap_size = 0;

int cnt = 0;
//ローカル環境でprintfデバッグしたいときは，
//以下の行とhakase.ccの7行目をコメントアウトを外してください
//int (*EXPORTED_SYMBOL(printf))(const char *format, ...);
//
//<printfの使い方>
//OFFLOAD_FUNC(printf,args...);
//のようにOFFLOAD_FUNC経由で呼び出します

static volatile int _lock = 0;
 
volatile void lock() {
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //add codes here
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
volatile void unlock() {
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //add codes here
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


// 参考https://www.sanfoundry.com/c-program-implement-heap/
void heap_insert(int element) {
  lock();
  heap_size++;
  heap_array[heap_size] = element;

  int now = heap_size;
  while (heap_array[now / 2] < element) {
    heap_array[now] = heap_array[now / 2];
    now /= 2;
  }
  heap_array[now] = element;
  unlock();
}

int heap_delete_max() {
  lock();
  int maxElement, lastElement, child, now; 
  maxElement = heap_array[1];
  lastElement = heap_array[heap_size--];

  for (now = 1; now * 2 <= heap_size; now = child) {
    child = now * 2;

    if (child != heap_size && heap_array[child + 1] > heap_array[child]) {
      child++;
    }

    if (lastElement < heap_array[child]) {
      heap_array[now] = heap_array[child];
    } else {
      break;
    }
  }

  heap_array[now] = lastElement;
  unlock();
  return maxElement;
}

void friend_main() {
  heap_array[0] = 0xfffffff;

  for(int i = 0; i < 0x20; i++) {
    heap_insert(i);
  }
  for(int i = 0; i < 0x10000; i++) {
    heap_insert(i);
    heap_delete_max();
  }

  // コア番号取得
  int c = __sync_fetch_and_add(&cnt, 1);
  
  // 全てのCPUがココに到達するまで待機
  while(cnt != 4) {
    __asm__ volatile("":::"memory");
  }

  // コア0のCPUがheapのサイズを確認
  if(c == 0) {
    if(heap_size == 0x20 * 4) {
      __sync_fetch_and_add(&SHARED_SYMBOL(test_value), 1);
    }

  }
  __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
}
