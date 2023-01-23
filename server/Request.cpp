#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include "Request.hpp"

// Constructor for the Request class does the initial parsing.
Request::Request(std::stringstream & request_data) {
	// Extract the method, URL, and version from the first line of the request
	request_data >> _method >> _url >> _http_version;

	// Ensure the request is using HTTP/1.1
	if (_http_version != "HTTP/1.1") {
		_valid_request = false;
		_error_log += "Invalid request. Only HTTP/1.1 is supported.\n";
	}

	// Extract the headers and body of the request
	std::string line;
	while (std::getline(request_data, line)) {
		if (line.empty()) {
			break;
		}	
		_headers += line + "\n";
	}

	//CHeck if this IF ESLE is correct
	if (request_data.tellg() == LLONG_MAX || request_data.tellg() < 0) {
		_valid_request = true;
		_error_log +=  "No body in HTTP request.\n";
	}
	else if (static_cast<long long>(request_data.tellg()) < 
			static_cast<long long>(request_data.str().length()))
		_body = request_data.str().substr(request_data.tellg());
	else {
		_valid_request = false;
		_error_log +=  "Invalid request. tellg() out of range\n";
	}
}

// Sort the headers into their respective variables, easier to add other headers. Just add if else.
void Request::sort_headers() {
	std::stringstream headers_stream(_headers);
	std::string line;
	while (std::getline(headers_stream, line)) {
		if (line.find("Content-Type") != std::string::npos) {
			_header_content_type = line.substr(line.find(":") + 2);
		}
		else if (line.find("Content-Length") != std::string::npos) {
			_header_content_length = line.substr(line.find(":") + 2);
		}
	}
}

std::string	Request::get_method() const {return _method;}
std::string	Request::get_path() const {return _url;} 
std::string	Request::get_http_version() const {return _http_version;} 
std::string	Request::get_headers() const {return _headers;} 
std::string	Request::get_body() const {return _body;} 

std::string	Request::get_content_type() const {return _header_content_type;}
std::string	Request::get_content_length() const {return _header_content_length;}

bool	Request::get_valid_request() const {return _valid_request;} 
std::string	Request::get_error_log() const {return _error_log;} 

std::ostream & operator<<(std::ostream &os, const Request &other) {
	os << other.get_method() << " " << other.get_path() << " " << other.get_http_version() 
		<< std::endl << other.get_headers() << "IS VALID:" << other.get_valid_request() << std::endl
		<< "ERROR LOG:" << other.get_error_log() << std::endl;
	return os;
}
