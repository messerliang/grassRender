#include "functions.h"


std::string utf8_to_ansi(const std::string& utf8_str) {
    // 第一步：UTF-8 转换为 UTF-16（宽字符）
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    if (wide_len == 0) return "";

    std::wstring wide_str(wide_len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);

    // 第二步：UTF-16（宽字符） 转 ANSI（GBK 编码）
    int ansi_len = WideCharToMultiByte(CP_ACP, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ansi_len == 0) return "";

    std::string ansi_str(ansi_len, 0);
    WideCharToMultiByte(CP_ACP, 0, wide_str.c_str(), -1, &ansi_str[0], ansi_len, nullptr, nullptr);

    return ansi_str;
}

float uniformFloatDist(float lower, float upper) {
    static std::random_device randomDevice;
    static std::mt19937 gen(randomDevice());

    std::uniform_real_distribution<> floatDist(lower, upper);
    return floatDist(gen);
}

void checkPointer(void* ptr, const char* msg) {
    if (!ptr) {
        std::cout << msg << std::endl;
        exit(-1);
    }

}