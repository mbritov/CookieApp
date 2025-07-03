#pragma once
#include "pch.h"
#include "CookieCpp.h"
#include "Helpers.h"
#include <string>
#include <sstream>
#include <ctime>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace helpers;

// CookieCpp class implementation
CookieCpp* CookieCpp::Create(const std::string& name,
    const std::string& value,
    const std::string& domain,
    const std::string& path,
    const std::string& secure,
    time_t expires,
    const std::string& sameSite)
{
    CookieCpp* c = new CookieCpp();
    if (c && !c->Init(name, value, domain, path, secure, expires, sameSite)) {
        delete c;
        return nullptr;
    }
    return c;
}

bool CookieCpp::Init(const std::string& name,
    const std::string& value,
    const std::string& domain,
    const std::string& path,
    const std::string& secure,
    time_t expires,
    const std::string& sameSite) {
    SetName(name);
    SetValue(value);
    SetDomain(domain);
    SetPath(path);
    SetSecure(secure);
    SetExpires(expires);
    SetSameSite(sameSite);
    return true;
}

// CookieCpp class implementation
bool CookieCpp::FromString(const std::string& cookieStr, const std::string& domain) {
    bool isNameSet = false;
    auto parameters = SplitString(cookieStr, ';');

    if (!domain.empty()) 
        SetDomain(domain);

    for (std::string param : parameters) {
        std::string nameValue = TrimSpaces(param);
        auto delimPos = nameValue.find('=');

        std::string name = (delimPos != std::string::npos) ? nameValue.substr(0, delimPos) : nameValue;
        std::string value = (delimPos != std::string::npos) ? nameValue.substr(delimPos + 1) : "";

        if (name.empty()) continue;

        if (!isNameSet) {
            SetName(name);
            SetValue(value);
            isNameSet = true;
            continue;
        }

        char initialChar = std::toupper(name[0]);
        switch (initialChar) 
        {
            case 'D':
                if (StrCaseEq(name, "Domain")) SetDomain(value);
                break;
            case 'E':
                if (StrCaseEq(name, "Expires")) SetExpires(value);
                break;
            case 'H':
                if (StrCaseEq(name, "HttpOnly")) SetHttpOnly(true);
                break;
            case 'M':
                if (StrCaseEq(name, "Max-Age") && std::stoi(value) > 0) 
                    SetExpires(std::time(nullptr) + std::stoi(value));
                break;
            case 'P':
                if (StrCaseEq(name, "Path")) SetPath(value);
                break;
            case 'S':
                if (StrCaseEq(name, "Secure")) 
                    SetSecure(true);
                else if (StrCaseEq(name, "SameSite")) {
                        SetSameSite(value);
                    if (StrCaseEq(value, "None")) 
                        SetSecure(true);
                }
                break;
        }
    }
    return isNameSet;
}

const std::string& CookieCpp::ToString() const {
    if (mHeaderFormat.empty()) {
        std::ostringstream oss;
        oss << mName << "=" << mValue;

        if (!mExpires.empty()) oss << "; expires=" << mExpires;
        if (!mDomain.empty()) oss << "; domain=" << mDomain;
        if (!mPath.empty()) oss << "; path=" << mPath;
        if (mSecure) oss << "; secure";
        if (mHttpOnly) oss << "; httponly";

        mHeaderFormat = oss.str();
    }
    return mHeaderFormat;
}

void CookieCpp::SetDomain(const std::string& domain) {
    if (domain.find("#HttpOnly_") == 0) {
        mDomain = domain.substr(10);
        mHttpOnly = true;
    }
    else {
        mDomain = domain;
    }
}

void CookieCpp::SetPath(const std::string& path) {
    if (path != "unknown") mPath = path;
}

void CookieCpp::SetExpires(time_t expires) {
    if (expires == 0) return;
    static const char* DAYS[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    static const char* MONS[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    struct tm tmBuf {};
    gmtime_r(&expires, &tmBuf);

    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "xxx, %d xxx %Y %H:%M:%S GMT", &tmBuf);
    std::memcpy(buffer, DAYS[tmBuf.tm_wday], 3);
    std::memcpy(buffer + 8, MONS[tmBuf.tm_mon], 3);

    mExpires = buffer;
}

void CookieCpp::SetSecure(const std::string& secure) {
    mSecure = StrCaseEq(secure, "TRUE");
}
