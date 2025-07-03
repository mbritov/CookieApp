#pragma once

#include <time.h>
#include <sstream>
#include <string>
#include <cstring>
#include <cassert>


class CookieC
{
public:
    static CookieC* Create(const char* Name,
        const char* Value,
        const char* Domain,
        const char* Path,
        const char* Secure,
        time_t      Expires,
        const char* SameSite = nullptr);

    CookieC();
    CookieC(const CookieC&);
    CookieC& operator=(const CookieC&);
    ~CookieC();

    const char* GetName() const;
    const char* GetValue() const;
    const char* GetDomain() const;
    const char* GetPath() const;
    const char* GetExpires() const;
    const char* GetSameSite() const;
    bool        IsSecure() const;
    bool        IsHttpOnly() const;
    bool        IsSessionCookie() const;
    bool        FromString(const char* Str, const char* Domain = nullptr);
    const char* ToString() const;

private:
    bool Init(const char* Name,
        const char* Value,
        const char* Domain,
        const char* Path,
        const char* Secure,
        time_t      Expires,
        const char* SameSite);

    void Assign(const CookieC& rhs);
    void Free();

    void SetName(const char* Name);
    void SetValue(const char* Value);
    void SetDomain(const char* Domain);
    void SetPath(const char* Path);
    void SetExpires(time_t Expires);
    void SetExpires(const char* Expires);
    void SetSecure(const char* Secure);
    void SetSecure(bool Secure);
    void SetHttpOnly(bool HttpOnly);
    void SetSameSite(const char* SameSite);

    char* mName, * mValue, * mDomain, * mPath, * mExpires;
    mutable char* mHeaderFormat;
    bool          mSecure, mHttpOnly;
    char* mSameSite;
};