/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:31:12 by ttas              #+#    #+#             */
/*   Updated: 2026/02/17 09:43:12 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Parsing.hpp"

#include <cstdlib> 

// ---------------- Static Helper ----------------

static void error_message(const std::string &message)
{
    std::cout << message << std::endl;
    exit(1);
}

static bool isRegularFile(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return false;
    return S_ISREG(st.st_mode);
}

// ---------------- Constructor ----------------

void Parsing::verify_validity()
{
    if (_name.empty())
        error_message("Server name is not set");
    if (_host.empty())
        error_message("Server host is not set");
    if (_port <= 0 || _port > 65535)
        error_message("Server port is invalid");
    if (_maxClientBodySize < 0)
        error_message("Max client body size is invalid");
}

// file constructor
Parsing::Parsing(std::string conf)
    : _name(""),
      _host(""),
      _port(0),
      _root(""),
      _maxClientBodySize(-1)
{
    if (conf.size() < 5 || conf.substr(conf.size() - 5) != ".conf")
        error_message("Invalid config file extension. Expected .conf");

    if (!isRegularFile(conf))
        error_message("Config path must be a regular file");

    std::ifstream file(conf.c_str());
    if (!file.is_open())
        error_message("Empty or non-existent config file");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        int pos = line.find_first_of(" ");
        if (pos <= 0)
            error_message("Invalid config line: " + line);

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key.empty() || value.empty())
            error_message("Key or value is empty in line: " + line);

        assign(key, value);
    }
    verify_validity();
    file.close();
}

static std::string trim(const std::string& s)
{
    size_t start = 0;
    size_t end = s.size();

    while (start < end && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;

    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;

    return s.substr(start, end - start);
}

// stringstream constructor
Parsing::Parsing(std::istream& stream)
    : _name(""),
      _host(""),
      _port(0),
      _root(""),
      _maxClientBodySize(-1)
{
	_routes.clear();
	
    std::string line;
	std::stringstream block;
	bool inside = false;
	int braceCount = 0;
	
    while (std::getline(stream, line))
    {
		line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;

		if (!inside) {
            // Allow variations: "route {", "route{", "route   {", etc.
            if (line.find("route") != std::string::npos &&
                line.find("{") != std::string::npos) {
                inside = true;
                braceCount = 1; // found the first '{'
				block.str("");  // clear previous content
                block.clear();  // reset flags
                continue;       // don't include this line
            }
        } 
        else
		{
			if (line.find("{") != std::string::npos)
				braceCount++;
			if (line.find("}") != std::string::npos)
				braceCount--;

			if (braceCount == 0) {
				inside = false;
				_routes.push_back(ParsingRoutes(block));
				continue; 
			}

			block << line << "\n";
			// std::cout << block.str() << std::endl;
        }
		if (!inside)
		{
			int pos = line.find_first_of(' ');
			if (pos <= 0)
				error_message("Invalid config line: " + line);
	
			// std::cout << "pos : " << pos << "\nsize : " << line.size() << std::endl;
			
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
		
			if (key.empty() || value.empty())
				error_message("Key or value is empty in line: " + line);
		
			// std::cout << "key : " + key + "\n value : " + value << std::endl;
			assign(key, value);

		}
   }
    verify_validity();
}

// ---------------- Destructor ----------------

Parsing::~Parsing()
{
}

// ---------------- Getters ----------------

const std::string &Parsing::getName() const { return _name; }
const std::string &Parsing::getHost() const { return _host; }
int Parsing::getPort() const { return _port; }
const std::string &Parsing::getRoot() const { return _root; }
std::map<int, std::string> &Parsing::getErrorPages() { return _error_pages; }
int Parsing::getMaxClientBodySize() const { return _maxClientBodySize; }

// ---------------- Setters ----------------

void Parsing::setName(const std::string &name) 
{
    if(!_name.empty())
        error_message("Duplicate: Server name already set to: " + _name);
    _name = name; 
}
void Parsing::setHost(const std::string &host) 
{ 
    if(!_host.empty())
        error_message("Duplicate: Server host already set to: " + _host);
    _host = host; 
}
void Parsing::setPort(const std::string &port) 
{ 
    if(_port != 0)
        error_message("Duplicate: Server port already set");
    _port = atoi(port.c_str()); 
}
void Parsing::setRoot(const std::string &root) 
{ 
    if(!_root.empty())
        error_message("Duplicate: Server root already set to: " + _root);
    _root = root; 
}

void Parsing::setErrorPage(const std::string &errorPage)
{
	int error_code = atoi(errorPage.substr(0, errorPage.find(' ')).c_str());
    this->_error_pages[error_code] = errorPage.substr(errorPage.find(' ') + 1);
}

void Parsing::setMaxClientBodySize(const std::string &size) { _maxClientBodySize = atoi(size.c_str()); }


// ---------------- Map Initialization ----------------

std::map<std::string, Parsing::Setter> Parsing::initMap()
{
    std::map<std::string, Setter> m;
    m["server_name"] = &Parsing::setName;
    m["host"] = &Parsing::setHost;
    m["port"] = &Parsing::setPort;
    m["root"] = &Parsing::setRoot;
    m["error_page"] = &Parsing::setErrorPage;
    m["client_max_body"] = &Parsing::setMaxClientBodySize;
    return m;
}

const std::map<std::string, Parsing::Setter> Parsing::_setters = Parsing::initMap();

// ---------------- assign() ----------------

void Parsing::assign(const std::string &key, const std::string &value)
{
    std::map<std::string, Setter>::const_iterator it = _setters.find(key);
    if (it != _setters.end())
        (this->*(it->second))(value);
    else
        error_message("Unknown server config key: " + key);
}