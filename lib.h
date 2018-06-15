
// Lib
// Send SMS notification with Free-Mobile API

// For network lib
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

std::string https_request(const char* hostname, int port, const char* agent, const char* verb, const char* obj_name, char szReq[1024], const char* hearders) // verb = GET/POST/...., obj_name = PATH URL, szReq = POST Param(OBS), hearders = Content-Type: text/html
{
	HINTERNET hIntSession = ::InternetOpenA(agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	HINTERNET hHttpSession = InternetConnectA(hIntSession, hostname, port, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
	HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, verb, obj_name, 0, 0, 0, INTERNET_FLAG_SECURE, 0);
	if (!HttpSendRequestA(hHttpRequest, hearders, strlen(hearders), szReq, strlen(szReq))) {
		DWORD dwErr = GetLastError();
	}
	std::string result = "";
	CHAR szBuffer[1025];
	DWORD dwRead = 0;
	while (::InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead) {
		szBuffer[dwRead] = 0;
		OutputDebugStringA(szBuffer);
		dwRead = 0;
		result += std::string(szBuffer);
	}
	::InternetCloseHandle(hHttpRequest);
	::InternetCloseHandle(hHttpSession);
	::InternetCloseHandle(hIntSession);
	return result;
}

std::string url_encode(const std::string &value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}

void send_notification_SMS(std::string API_USER, std::string API_PASS, std::string TEXT)
{
	const char* url_smsapi = "smsapi.free-mobile.fr";
	const char* useragent_smsapi = "Free-Mobile SMS API C++ by Neosama from Github";
	std::string path_url = "/sendmsg?user=" + API_USER + "&pass=" + API_PASS + "&msg=" + url_encode(TEXT);

	https_request(url_smsapi, 443, useragent_smsapi, "GET", path_url.c_str(), (char*)"", "");
}
