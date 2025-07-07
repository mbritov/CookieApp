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
CookieCpp* CookieCpp::Create(
    const std::string& name,
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
    const std::string& sameSite) 
{
	if (!SetName(name)) return false; 
    if (!SetValue(value)) return false; 
    SetDomain(domain);
    SetPath(path);
    SetSecure(secure);
    SetExpires(expires);
    SetSameSite(sameSite);

    return true;
}

bool CookieCpp::SetName(const std::string& name) { 
	if (name.empty()) return false; // Name cannot be empty
    mName = name; 

	// todo - add validation for cookie name
    // can contain any US - ASCII characters except for: 
    // control characters(ASCII characters 0 up to 31 and ASCII character 127) 
    // or separator characters(space, 
    // tab and the characters : () < > @, ; : \ " / [ ] ? = { }

	return true;  
}

bool CookieCpp::SetValue(const std::string& value) { 
    
    if (value.empty()) return false; // Value cannot be empty
    mValue = value;
    
    return true;
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

bool CookieCpp::FromString(const std::string& cookieStr, const std::string& domain) {
    bool isNameSet = false;
    bool isExpireSet = false;
    auto parameters = SplitString(cookieStr, ';');

    if (!domain.empty())
        SetDomain(domain);

    for (std::string param : parameters) {
        std::string nameValue = TrimSpaces(param);
        auto delimPos = nameValue.find('=');

        std::string name = (delimPos != std::string::npos) ? nameValue.substr(0, delimPos) : nameValue;
        std::string value = (delimPos != std::string::npos) ? nameValue.substr(delimPos + 1) : "";

        if (name.empty()) continue;

        // first we set mandatory Name and Value
        if (!isNameSet) {
            SetName(name);
            SetValue(value);
            isNameSet = true;
            continue;
        }

        // second we check for optional parameters
        if (StrCaseEq(name, "Domain")) {
            SetDomain(value);
        }
        else if (StrCaseEq(name, "Expires")) {
            if (!isExpireSet) SetExpires(value);
        }
        else if (StrCaseEq(name, "HttpOnly")) {
            SetHttpOnly(true);
        }
        else if (StrCaseEq(name, "Max-Age")) {
            // Indicates the number of seconds until the cookie expires. 
            // A zero or negative number will expire the cookie immediately. 
            // If both Expires and Max-Age are set, Max-Age has precedence.
            int maxAge = std::stoi(value);

            if (maxAge > 0) {
                // Max-Age is # seconds from now. So expiration will be <Now> + Value                                                      
                SetExpires(std::time(nullptr) + maxAge);
                isExpireSet = true;
            }
            else {
                // Set expiration to current time - it means cookie expires immediately
                SetExpires(std::time(nullptr)); 
            }
        }
        else if (StrCaseEq(name, "Path")) {
            SetPath(value);
        }
        else if (StrCaseEq(name, "Secure")) {
            SetSecure(true);
        }
        else if (StrCaseEq(name, "SameSite")) {
            SetSameSite(value);
            // If SameSite is set to "None", the Secure attribute must also be set.
            if (StrCaseEq(value, "None"))
                SetSecure(true);
        }
        else if (StrCaseEq(name, "Partitioned")) {
			SetPartitioned(true);
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
