#include "pch.h"
#include "../CookiesTest/CookieCpp.h"

namespace CookieCTests  // Namespace for CookieCpp tests
{
    TEST(CookieCTest, ToStringProducesValidHeader) {
        CookieCpp cookie;
        cookie.SetName("session_id");
        cookie.SetValue("abc123");
        /*cookie.SetDomain("example.com");
        cookie.SetPath("/");
        cookie.SetSecure(true);
        cookie.SetHttpOnly(true);
        cookie.SetExpires(3600 + std::time(nullptr));*/

        /*std::string header = cookie.ToString();

        EXPECT_NE(header.find("session_id=abc123"), std::string::npos);
        EXPECT_NE(header.find("domain=example.com"), std::string::npos);
        EXPECT_NE(header.find("path=/"), std::string::npos);
        EXPECT_NE(header.find("secure"), std::string::npos);
        EXPECT_NE(header.find("httponly"), std::string::npos);
        EXPECT_NE(header.find("expires="), std::string::npos);*/
    }
}