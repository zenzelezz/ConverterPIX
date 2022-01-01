/*
 * This code is a very abbreviated version of
 *   https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170
 * only to look for SSE 4.2
 */
#include <bitset>
#include <array>
#include <fstream>
#include <intrin.h>
#include <iostream>
int main(int argc, char* argv[]) {
    std::array<int, 4> cpui;
    __cpuid(cpui.data(), 0);
    if (cpui[0] < 1) {
        return 0;
    }
    __cpuidex(cpui.data(), 1, 0);
    std::fstream config_h(argv[1], std::ios_base::out | std::ios_base::trunc);
    if (std::bitset<32>(cpui[2])[20]) {
        config_h << "#define __SSE4_2__\n";
    } else {
        config_h << "nope\n";
    }
    config_h.close();
    return 0;
}
