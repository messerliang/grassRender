#pragma once

#include <iostream>
#include <string>
#include <random>
#include "Windows.h"

std::string utf8_to_ansi(const std::string& utf8_str);

float uniformFloatDist(float lower, float upper);

void checkPointer(void* ptr, const char* msg);