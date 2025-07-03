#include "../CookieLib/CookieC.h"
#include "../CookieLib/CookieCpp.h"
#include <iostream>

int main()
{
	// Example usage of CookieC 
    std::cout << "********************************" << std::endl;
	std::cout << "CookieC Example:" << std::endl;
    const char* CookieStr = "name=value; domain=example.com; path=/; expires=Wed, 21 Oct 2023 07:28:00 GMT; secure; httponly";
    CookieC* Cookie = new CookieC();
    if (Cookie)
    {
        Cookie->FromString(CookieStr);
        printf("Cookie Name: %s\n", Cookie->GetName());
        printf("Cookie Value: %s\n", Cookie->GetValue());
        printf("Cookie Domain: %s\n", Cookie->GetDomain());
        printf("Cookie Path: %s\n", Cookie->GetPath());
        printf("Cookie Expires: %s\n", Cookie->GetExpires());
        printf("Cookie Secure: %d\n", Cookie->IsSecure());
        printf("Cookie HttpOnly: %d\n", Cookie->IsHttpOnly());
        printf("Cookie SameSite: %s\n", Cookie->GetSameSite() ? Cookie->GetSameSite() : "(NULL)");

        delete Cookie;
    }

    CookieC* Cookie2 = CookieC::Create("name", "value", "example.com", "/", "secure", time(nullptr), "Lax");
    if (Cookie2)
    {
        printf("%s\n", Cookie2->ToString());
        delete Cookie2;
    }

	// Example usage of CookieCpp
    std::cout << "********************************" << std::endl;
    std::cout << "CookieCpp Example:" << std::endl;
    CookieCpp* cookieCpp = new CookieCpp();
    if (cookieCpp)
    {
        cookieCpp->FromString(CookieStr);
        std::cout << "Cookie Name: " << cookieCpp->GetName() << std::endl;
        std::cout << "Cookie Value: " << cookieCpp->GetValue() << std::endl;
        std::cout << "Cookie Domain: " << cookieCpp->GetDomain() << std::endl;
        std::cout << "Cookie Path: " << cookieCpp->GetPath() << std::endl;
        std::cout << "Cookie Expires: " << cookieCpp->GetExpires() << std::endl;
        std::cout << "Cookie Secure: " << cookieCpp->IsSecure() << std::endl;
        std::cout << "Cookie HttpOnly: " << cookieCpp->IsHttpOnly() << std::endl;
        std::cout << "Cookie SameSite: " << cookieCpp->GetSameSite() << std::endl;

        delete cookieCpp;
    }

    CookieCpp* Cookie4 = CookieCpp::Create("name", "value", "example.com", "/", "secure", time(nullptr), "Lax");
    if (Cookie4)
    {
		std::cout << Cookie4->ToString() << std::endl;
        delete Cookie4;
    }
}