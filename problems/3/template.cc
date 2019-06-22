#include <toshokan/friend/export.h>
#include <toshokan/friend/offload.h>
#include "shared.h"

int64_t SHARED_SYMBOL(sync_flag);
uint64_t SHARED_SYMBOL(test_value);

#define HEAP_MAX_NODE_NUM 0x6

int heap_array[HEAP_MAX_NODE_NUM];
int heap_size = 0;

int cnt = 0;
//ローカル環境でprintfデバッグしたいときは，
//以下の行とhakase.ccの7行目をコメントアウトを外してください
int (*EXPORTED_SYMBOL(printf))(const char *format, ...);
//
//<printfの使い方>
//OFFLOAD_FUNC(printf,args...);
//のようにOFFLOAD_FUNC経由で呼び出します
 
void lock() {
}
void unlock() {
}


// 参考https://www.sanfoundry.com/c-program-implement-heap/
void heap_insert(int element) {
  lock();
  heap_size++;
  heap_array[heap_size] = element;

  int now = heap_size;
  while (now != 1 && heap_array[now / 2] < element) {
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
  if(__sync_fetch_and_add(&cnt, 1) != 0 ){
    __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
    return;
  }
  heap_array[0] = 0x10000000;

  heap_insert(2);
  heap_insert(5);
  heap_insert(1);
  heap_insert(3);
  heap_insert(4);
  heap_delete_max();
  heap_delete_max();
  heap_delete_max();
  heap_delete_max();
  heap_delete_max();

  __sync_fetch_and_add(&SHARED_SYMBOL(sync_flag), 1);
}
