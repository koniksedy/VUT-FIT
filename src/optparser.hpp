#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <limits.h>

#pragma once


struct OptParser {
    class Help {};
    enum class Mode {nothing, code, decode};

    char *input = nullptr;
    char *output = nullptr;
    Mode mode = Mode::nothing;
    bool adaptive = false;
    bool with_model = false;
    uint16_t width = 0;

    static void print_help();
    void parse(int argc, char **argv);
};
