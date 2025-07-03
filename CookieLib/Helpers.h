#pragma once
#include <string>
#include <vector>
#include <sstream>


namespace helpers
{
    // for the simplicity of the code, the below helper functions are defined as static. 
	// In a larger project, you might want to encapsulate them in a class or a separate utility namespace.
	// also, consider moving the implementation to a .cpp file, as it will optimize the binary size

    static std::vector<std::string> SplitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static std::string TrimSpaces(const std::string& str) {
        size_t start = str.find_first_not_of(' ');
        size_t end = str.find_last_not_of(' ');
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    static bool StrCaseEq(const std::string& s1, const std::string& s2) {
        return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(),
            [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    }

    //align Windows with other platforms
#ifdef _WIN32
#define strtok_r strtok_s

    static struct tm* gmtime_r(const time_t* clock, struct tm* res)
    {
        errno_t err = gmtime_s(res, clock);
        if (err)
            return nullptr;
        return res;
    }
#endif
}
