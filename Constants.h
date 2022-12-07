#pragma once

/* header file for keeping all the unchanged values */
/* 
constexpr instead of const because they are values 
given statically and does not need to be called from say a funciton 
*/
constexpr int MAX_ROOMS = 80;
constexpr int MIN_ROOMS = 20;

constexpr int PRICE_S = 100;
constexpr int PRICE_D = 150;