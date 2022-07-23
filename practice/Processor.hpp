#ifndef WEBSERV_PRACTICE_PROCESSOR_HPP_
#define WEBSERV_PRACTICE_PROCESSOR_HPP_

#include <map>
#include <string>
#include <vector>

const std::pair<int, std::string> kStatusCodeArray[] = {
    std::make_pair(200, "OK"),
    std::make_pair(201, "Created"),
    std::make_pair(204, "No Content"),
    std::make_pair(205, "Reset Content"),
    std::make_pair(301, "Moved Permanently"),
    std::make_pair(303, "See Other"),
    std::make_pair(307, "Temporary Redirect"),
    std::make_pair(400, "Bad Request"),
    std::make_pair(401, "Unauthorized"),
    std::make_pair(403, "Forbidden"),
    std::make_pair(404, "Not Found"),
    std::make_pair(405, "Method Not Allowed"),
    std::make_pair(408, "Request Timeout"),
    std::make_pair(410, "Gone"),
    std::make_pair(411, "Length Required"),
    std::make_pair(413, "Payload Too Large"),
    std::make_pair(414, "URI Too Long"),
    std::make_pair(500, "Internal Server Error"),
    std::make_pair(503, "Service Unavailable"),
    std::make_pair(504, "Gateway Timeout"),
    std::make_pair(505, "HTTP Version Not Supported"),
};

const std::map<int, std::string> kStatusCode(kStatusCodeArray,
                                             kStatusCodeArray + sizeof(kStatusCodeArray) /
                                                                    sizeof(kStatusCodeArray[0]));

#endif