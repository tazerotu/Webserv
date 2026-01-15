#include "../includes/ServerConfigRoutes.hpp"

static void error_message(const std::string &message)
{
    std::cout << message << std::endl;
    exit(1);
}

void ServerConfigRoutes::verify_validity()
{
	if (_routeLoc.empty())
		error_message("Route has no name");
    if (_defaultFile.empty())
        error_message("default page of route " + _routeLoc + " is not set");
	if (_rootPath.empty())
		error_message("Route " + _routeLoc + " has no root");
    if (_autoIndex != 0 && _autoIndex != 1)
        error_message("Autoindex must be 1 or 0");
    if (_configMethods.empty())
        error_message("Allowed methods are not set");
	if (_upload != 1 || _upload != 0)
		error_message("Upload must be 1 or 0");
	if (_upload == 1 && _uploadDirectory.empty())
		error_message("Missing upload directory in upload available route");
	if ((_cgi_path.empty() || _cgi_ext.empty()) && !(_cgi_path.empty() || _cgi_ext.empty()))
		error_message("CGI Error, extention or path empty\n Required both be set or left empty");
	
}

ServerConfigRoutes::ServerConfigRoutes(std::istream& stream)
	:_cgi_ext(""),
	 _cgi_path(""),
	 _routeLoc(""),
	 _rootPath(""),
	 _defaultFile(""),
	 _uploadDirectory(""),
	 _redirection("", ""),
	 _upload(-1),
	 _autoIndex(-1)
{
	std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        int pos = line.find_first_of(' ');
        if (pos <= 0)
            error_message("Invalid config line: " + line);

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
	
		if (key.empty() || value.empty())
			error_message("Key or value is empty in line: " + line);
	
		assign(key, value);
   }

    verify_validity();
}


// ---------------- Map Initialization ----------------

std::map<std::string, ServerConfigRoutes::Setter> ServerConfigRoutes::initMap()
{
    std::map<std::string, Setter> m;
	m["name"] = &ServerConfigRoutes::setRouteLoc;
    m["index"] = &ServerConfigRoutes::setDefaultFile;
	m["root"] = &ServerConfigRoutes::setRootPath;
    m["autoindex"] = &ServerConfigRoutes::setAutoIndex;
    m["allowed_methods"] = &ServerConfigRoutes::setConfigMethods;
	m["upload"] = &ServerConfigRoutes::setUpload;
	m["upload_directory"] = &ServerConfigRoutes::setUploadDirectory;
    m["cgi_path"] = &ServerConfigRoutes::setCGIPath;
    m["cgi_ext"] = &ServerConfigRoutes::setCGIExt;
	m["redirection"] = &ServerConfigRoutes::setRedirection;

    return m;
}

const std::map<std::string, ServerConfigRoutes::Setter> ServerConfigRoutes::_setters = ServerConfigRoutes::initMap();

// ---------------- assign() ----------------

void ServerConfigRoutes::assign(const std::string &key, const std::string &value)
{
    std::map<std::string, Setter>::const_iterator it = _setters.find(key);
    if (it != _setters.end())
        (this->*(it->second))(value);
    else
        error_message("Unknown config key: " + key);
}