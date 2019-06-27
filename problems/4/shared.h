#pragma once

#include <stdint.h>
#include <toshokan/symbol.h>

// initialized by hakase.cc (before friend app is executed)
extern int64_t SHARED_SYMBOL(sync_flag);
extern uint32_t SHARED_SYMBOL(test_value[0x20]);
extern void* SHARED_SYMBOL(hpet_addr);
