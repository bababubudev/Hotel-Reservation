#pragma once
#include <cstdlib>
#include <ctime>
#include "Constants.h"

int reserve_id()
{
	srand((int)time(NULL));
	return rand() % 90000 + 10000;
}

int discount_amount()
{
	srand((int)time(NULL));
	return rand() % 3 * 10;
}

int room_division()
{
	srand((int)time(NULL));
	return rand() % (MAX_ROOMS / DIVISIONS) + MIN_ROOMS;
}