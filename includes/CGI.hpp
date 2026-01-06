#ifndef CGI_HPP
#define CGI_HPP
#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


class CGI {
	public:
		CGI(const std::string& scriptPath, const std::map<std::string, std::string>& envVars,
			const std::string& inputData = "");
		~CGI();
		std::string execute();
	private:
		std::string scriptPath;
		std::map<std::string, std::string> envVars;
		std::string inputData;
		std::string readFromPipe(int fd);
		void writeToPipe(int fd, const std::string& data);
};

#endif // CGI_HPP