#include <stdio.h>
#include <toshokan/hakase/export.h>
#include <toshokan/hakase/hakase.h>
#include <unistd.h>
#include "shared.h"

//EXPORT_SYMBOL(printf);

int test_main() {
  int r;
  r = setup();
  if (r != 0) {
    return r;
  }

  SHARED_SYMBOL(sync_flag) = 0;
  SHARED_SYMBOL(hpet_addr) = (void*)0xfed00000;

  printf("starting up friend cores...");
  fflush(stdout);
  int cpunum = boot(0);
  printf("done\n");
  fflush(stdout);

  while (SHARED_SYMBOL(sync_flag) != cpunum) {
    offloader_tryreceive();
    usleep(1000);
  }
  return 1;
}

int main(int argc, const char **argv) {
  if (!(test_main() > 0)) {
    printf("SystemJudge:WA\n");
    return 255;
  } 

  for (int i = 0; i < 0x20 - 1; i++) {
    if (!(SHARED_SYMBOL(test_value[i]) < SHARED_SYMBOL(test_value[i+1]))) {
      printf("SystemJudge:WA\n");
      return 255;
    }
  }
  printf("SystemJudge:AC\n");
  return 0;
  
}
