#include "webserver.h"
#include "Cgi.hpp"

int execute_request(Connection &connection) {

	//[INFO] Preparing response, error checking
	connection._response.set_client_socket(connection._socket);
	connection.set_location();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}
	connection.check_method();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}

	if (connection._request.get_method() == "GET") {
		if (is_cgiable(connection._request._url, connection._server))
			cgi_get_request(connection);
		else
			get_request(connection);
	}
	else if (connection._request.get_method() == "POST") {
		post_request(connection);
	}
	else if (connection._request.get_method() == "DELETE") {
		delete_request(connection);
	}
	return 0;
}
