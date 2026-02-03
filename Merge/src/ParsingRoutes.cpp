#include "./ParsingRoutes.hpp"

static void error_message(const std::string &message)
{
    std::cout << message << std::endl;
    exit(1);
}

void ParsingRoutes::verify_route()
{
	if(_routeLoc.empty())
	{
		error_message("Route has no name");
	}
    if(_defaultFile.empty())
	{
    	error_message("default page of route " + _routeLoc + " is not set");
	}
	if(_rootPath.empty())
	{
		error_message("Route " + _routeLoc + " has no root");
    }
    if(_autoIndex != 0 && _autoIndex != 1)
	{
    	error_message("Autoindex must be 1 or 0");
	}
    if(_configMethods.empty())
	{
    	error_message("Allowed methods are not set");
	}
	if(_upload_status != 1 && _upload_status != 0)
	{
		error_message("Upload must be 1 or 0");
	}
	if((_cgi_path.empty() || _cgi_ext.empty()) && !(_cgi_path.empty() || _cgi_ext.empty()))
	{
		error_message("CGI Error, extention or path empty\n Required both be set or left empty");
	}
	
}

static std::string trim(const std::string& s)
{
    size_t start = 0;
    size_t end = s.size();

    // Trim leading whitespace
    while (start < end && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;

    // Trim trailing whitespace
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;

    return s.substr(start, end - start);
}

ParsingRoutes::ParsingRoutes(std::istream& stream)
	:_routeLoc(""),
	 _configMethods(),
	 _rootPath(""),
	 _autoIndex(-1),
	 _defaultFile(""),
	 _upload_status(-1),
	 _cgi_path(""),
	 _cgi_ext(""),
	 _redirection()
{
	std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
		line = trim(line);

        std::string::size_type pos = line.find_first_of(' ');
        if (pos <= 0)
            error_message("Invalid config line: " + line);

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
	

		if (key.empty() || value.empty())
			error_message("Key or value is empty in line: " + line);
	
		assign(key, value);
   }

    verify_route();
}


// ---------------- Map Initialization ----------------

std::map<std::string, ParsingRoutes::Setter> ParsingRoutes::initMap()
{
    std::map<std::string, Setter> m;
	m["route_name"] = &ParsingRoutes::setRouteLoc;
    m["index"] = &ParsingRoutes::setDefaultFile;
	m["root"] = &ParsingRoutes::setRootPath;
    m["autoindex"] = &ParsingRoutes::setAutoIndex;
    m["allowed_methods"] = &ParsingRoutes::setConfigMethods;
	m["upload_status"] = &ParsingRoutes::setUpload;
    m["cgi_path"] = &ParsingRoutes::setCGIPath;
    m["cgi_ext"] = &ParsingRoutes::setCGIExt;
	m["redirection"] = &ParsingRoutes::setRedirection;

    return m;
}

const std::map<std::string, ParsingRoutes::Setter> ParsingRoutes::_setters = ParsingRoutes::initMap();

// ---------------- assign() ----------------

void ParsingRoutes::assign(const std::string &key, const std::string &value)
{
    std::map<std::string, Setter>::const_iterator it = _setters.find(key);
    if (it != _setters.end())
        (this->*(it->second))(value);
    else
        error_message("Unknown route config key: " + key);
}