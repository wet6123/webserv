#ifndef REQUEST_STRUCT_HPP
#define REQUEST_STRUCT_HPP

#include <iostream>
#include "Request.hpp"
#include "../../common/String.hpp"

/**
 * @brief Request 구조체
 * @note
	GET /index.html HTTP/1.1 -  method, path, version
	Host: www.example.com - 요청하는 자의 호스트명, 포트 번호
	User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36 - 클라이언트 소프트웨어 정보
	Referer: https://www.google.com/ - 이전 페이지의 URL
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*//*;q=0.8 - 클라이언트가 받아들일 수 있는 미디어 타입
	Accept-Charset: UTF-8,*;q=0.5 - 클라이언트가 받아들일 수 있는 문자 인코딩
	Accept-Language: en-US,en;q=0.9,ko;q=0.8 - 클라이언트가 받아들일 수 있는 언어
	Accept-Encoding: gzip, deflate, br - 클라이언트가 받아들일 수 있는 인코딩
	If-Modified-Since: Wed, 21 Oct 2015 07:28:00 GMT - 리소스가 마지막 수정된 날짜
	Authorization: == - 클라이언트 인증 정보
	Origin: https://www.example.com - 요청이 어디서 시작되었는지
	Cookie: session_id=abc123; user_preference=dark_mode - 쿠키 정보

	Date: Wed, 21 Jul 2023 07:28:00 GMT - 메시지가 생성된 날짜
	Cache-Control: no-cache - 캐시 제어 옵션
	Connection: keep-alive - 클라이언트와 서버의 연결 상태
	Transfer-Encoding: chunked - 메시지 바디의 전송 코딩 형식 (대용량 데이터 전송시 사용)

	Content-Type: application/x-www-form-urlencoded - 메시지 바디의 미디어 타입
	Content-Length: 0 - 메시지 바디의 길이 get 요청시 0
	Content-Language: en-US - 메시지 바디의 언어
	Content-Disposition: inline - 메시지 바디의 표시 방법
	Content-Encoding: gzip - 메시지 바디의 인코딩
	Last-Modified: Tue, 20 Jul 2023 07:28:00 GMT - 리소스가 마지막 수정된 날짜
	Allow: GET, POST, HEAD - 리소스에 대한 허용 가능한 메서드
	ETag: "686897696a7c876b7e" - 리소스의 버전을 식별하기 위한 태그
	Expires: Thu, 22 Jul 2023 07:28:00 GMT - 리소스의 만료 날짜


	IF-Modified-Since vs Last-Modified = If-Modified-Since는 Last-Modified와 비교하여 리소스가 수정되었는지 확인
	ETag vs Last-Modified = ETag는 리소스의 버전을 식별하기 위한 태그, Last-Modified는 리소스가 마지막 수정된 날짜

	추후 추가 및 수정될 수 있음
*/
/*
{
	Host,
	User_Agent,
	Referer,
	Accept,
	Accept_Charset,
	Accept_Language,
	Accept_Encoding,
	If_Modified_Since,
	Authorization,
	Origin,
	Cookie,
	Date,
	Cache_Control,
	Connection,
	Transfer_Encoding,
	Content_Type,
	Content_Length,
	Content_Language,
	Content_Location,
	Content_Disposition,
	Content_Encoding,
	Last_Modified,
	Allow,
	ETag,
	Expires
};
*/
namespace RequestData {
	std::string method;
	std::string uri;
	std::string version;
	// request header
	std::string host;
	std::string port;
	std::string ip;
	std::string user_agent;
	std::string referer;
	std::string accept;
	std::string accept_charset;
	std::string accept_language;
	std::string accept_encoding;
	std::string if_modified_since;
	std::string if_none_match;
	std::string authorization;
	std::string origin;
	std::string cookie;
	// common header
	std::string date;
	std::string cache_control;
	std::string connection;
	std::string transfer_encoding;
	// entity header
	std::string content_type;
	int content_length;
	std::string content_language;
	std::string content_location;
	std::string content_disposition;
	std::string content_encoding;
	std::string last_modified;
	std::string allow;
	std::string etag;
	std::string expires;

	void init(const Request& _request) {
		method = _request.getHeader("Method");
		uri = _request.getHeader("URI");
		version = _request.getHeader("Version");
		host = _request.getHeader("Host");
		port = host.substr(host.find(":") + 1);
		ip = host.substr(0, host.find(":"));
		user_agent = _request.getHeader("User-Agent");
		referer = _request.getHeader("Referer");
		accept = _request.getHeader("Accept");
		accept_charset = _request.getHeader("Accept-Charset");
		accept_language = _request.getHeader("Accept-Language");
		accept_encoding = _request.getHeader("Accept-Encoding");
		if_modified_since = _request.getHeader("If-Modified-Since");
		if_none_match = _request.getHeader("If-None-Match");
		authorization = _request.getHeader("Authorization");
		origin = _request.getHeader("Origin");
		cookie = _request.getHeader("Cookie");
		date = _request.getHeader("Date");
		cache_control = _request.getHeader("Cache-Control");
		connection = _request.getHeader("Connection");
		transfer_encoding = _request.getHeader("Transfer-Encoding");
		content_type = _request.getHeader("Content-Type");
		content_length = String::ToInt(_request.getHeader("Content-Length"));
		content_language = _request.getHeader("Content-Language");
		content_location = _request.getHeader("Content-Location");
		content_disposition = _request.getHeader("Content-Disposition");
		content_encoding = _request.getHeader("Content-Encoding");
		last_modified = _request.getHeader("Last-Modified");
		allow = _request.getHeader("Allow");
		etag = _request.getHeader("ETag");
		expires = _request.getHeader("Expires");
	}
};

#endif