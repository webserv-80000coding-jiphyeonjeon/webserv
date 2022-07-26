#include "Config.hpp"

Config::Config() : servers_() {}

Config::~Config() {}

const Config::ServerType& Config::getServers() const { return servers_; }

void Config::setServers(const Config::ServerType& servers) { servers_ = servers; }
