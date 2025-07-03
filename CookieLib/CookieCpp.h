#pragma once

#include <time.h>
#include <sstream>
#include <string>
#include <cstring>
#include <cassert>

class CookieCpp {
public:
    static CookieCpp* Create(const std::string& name,
        const std::string& value,
        const std::string& domain,
        const std::string& path,
        const std::string& secure,
        time_t expires,
        const std::string& sameSite = "");

    CookieCpp() = default;
    CookieCpp(const CookieCpp&) = default;
    CookieCpp& operator=(const CookieCpp&) = default;
    ~CookieCpp() = default;

    const std::string& GetName() const { return mName; }
    const std::string& GetValue() const { return mValue; }
    const std::string& GetDomain() const { return mDomain; }
    const std::string& GetPath() const { return mPath; }
    const std::string& GetExpires() const { return mExpires; }
    const std::string& GetSameSite() const { return mSameSite.size() == 0 ? mSameSite : "(NULL)"; }

    void SetName(const std::string& name) { mName = name; }
    void SetValue(const std::string& value) { mValue = value; }
    void SetDomain(const std::string& domain);
    void SetPath(const std::string& path);
    void SetExpires(const std::string& expires) { mExpires = expires; }
    void SetExpires(time_t Expires);
    void SetSecure(const std::string& secure);
    void SetSecure(bool secure) { mSecure = secure; }
    void SetHttpOnly(bool httpOnly) { mHttpOnly = httpOnly; }
    void SetSameSite(const std::string& sameSite) { mSameSite = sameSite; }

    bool IsSecure() const { return mSecure; }
    bool IsHttpOnly() const { return mHttpOnly; }
    bool IsSessionCookie() const { return mExpires.empty(); }

    bool FromString(const std::string& cookieStr, const std::string& domain = "");
    const std::string& ToString() const;

private:
    std::string mName;
    std::string mValue;
    std::string mDomain;
    std::string mPath;
    std::string mExpires;
    mutable std::string mHeaderFormat;
    std::string mSameSite;
    bool mSecure = false;
    bool mHttpOnly = false;

    bool Init(const std::string& name,
        const std::string& value,
        const std::string& domain,
        const std::string& path,
        const std::string& secure,
        time_t expires,
        const std::string& sameSite);
};