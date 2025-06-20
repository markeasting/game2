
#include "util/Filesystem.h"

#include <fstream>
#include <stdexcept>
#include <string>

#ifdef __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif _WIN32
#include <Windows.h>
#endif

#include <iostream>

Filesystem::Filesystem() {
    basePath = Filesystem::getBasePath();
}

Filesystem& Filesystem::instance() {
    static Filesystem instance;
    return instance;
}

std::string Filesystem::getBasePath() {
      
    #ifdef __linux__
        char c_path[1024];

        readlink("/proc/self/exe", c_path, 1024);
        
        std::string path(c_path);
        // std::string executable = "main";

    #elif __APPLE__
        char c_path[1024];
        uint32_t size = sizeof(c_path);
        if (_NSGetExecutablePath(c_path, &size) == 0)
            std::string path(c_path);
        else
            printf("Could not get executable path, I need %u characters\n", size);
        
        std::string path(c_path);
        // std::string executable = "main.app";

    #elif _WIN32
        // Copied from Boost library
        typedef std::vector<char> char_vector;
        typedef std::vector<char>::size_type size_type;
        char_vector buf(1024, 0);
        size_type size = buf.size();
        // bool havePath = false; // commented out at some point, untested
        bool shouldContinue = true;
        do {
            DWORD result = GetModuleFileNameA(nullptr, &buf[0], size);
            DWORD lastError = GetLastError();
            if (result == 0)
            {
                shouldContinue = false;
            } else if (result < size) {
                // havePath = true;
                shouldContinue = false;
            }
            else if (
                result == size
                && (lastError == ERROR_INSUFFICIENT_BUFFER || lastError == ERROR_SUCCESS)
            ) {
                size *= 2;
                buf.resize(size);
            } else {
                shouldContinue = false;
            }
        } while (shouldContinue);

        std::string path = &buf[0];

    #endif

    // the path contains the executable name, so we need to remove it
    // it can be any name, just remove the last part after the last slash
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        path = path.substr(0, lastSlash + 1); // include the slash
    } else {
        // if no slash is found, we assume the path is just the current directory
        path = "./";
    }

    // log 
    std::cout << "[Filesystem] base path: " << path << std::endl;

    return path;
}

std::string Filesystem::getFileContents(const std::string& relativePath) {
    
    // MacOS "main.app.dSYM/Contents/..."
    std::ifstream stream(basePath + relativePath);
    std::string contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    
    if (!stream.is_open()) {
        throw std::runtime_error(
            std::string("[Filesystem] file could not be opened: ") + basePath + relativePath
        );
    }

    return contents;
}
