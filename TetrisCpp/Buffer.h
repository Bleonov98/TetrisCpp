#pragma once
#include "stdfix.h"

#define ROWS 50
#define COLS 31

typedef struct wd {
	int size_rows;
	int size_cols;
	char16_t vBuf[ROWS][COLS]{0};
} WD;