
#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include <string>
#include <vector>

#define PLATFORM_AUTO    0
#define PLATFORM_WINDOWS 1
#define PLATFORM_OSX     2
#define PLATFORM_LINUX   3
#define PLATFORM_DEBIAN  4
#define PLATFORM_JVM     5

struct Configuration {
    std::string output_filename = "";
    std::vector<std::string> filename_stack;
    unsigned int platform = 0;
    bool console = 0;
    bool optimize = 0;
    bool run = 0;
    bool wait = 0;
    bool package = 0;
};

Configuration configure(int*, char***);

#endif // OPTIONS_H_INCLUDED
