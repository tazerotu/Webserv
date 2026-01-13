#include "../includes/CGI.hpp"

CGI::CGI(const std::string& scriptPath, const std::map<std::string, std::string>& envVars, const std::string& inputData)
	: scriptPath(scriptPath), envVars(envVars), inputData(inputData)
{
}

CGI::~CGI()
{
}

std::string execute()
{
	int inputPipe[2];
	int outputPipe[2];

	if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
		throw std::runtime_error("Failed to create pipes");
	}

	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Failed to fork process");
	}

	if (pid == 0) { // Child process
		close(inputPipe[1]);
		close(outputPipe[0]);

		dup2(inputPipe[0], STDIN_FILENO);
		dup2(outputPipe[1], STDOUT_FILENO);

		close(inputPipe[0]);
		close(outputPipe[1]);

		std::vector<char*> env;
		for (const auto& pair : envVars) {
			std::string envVar = pair.first + "=" + pair.second;
			env.push_back(strdup(envVar.c_str()));
		}
		env.push_back(nullptr);

		execle(scriptPath.c_str(), scriptPath.c_str(), nullptr, env.data());
		perror("execle failed");
		exit(EXIT_FAILURE);
	} else { // Parent process
		close(inputPipe[0]);
		close(outputPipe[1]);

		if (!inputData.empty()) {
			writeToPipe(inputPipe[1], inputData);
		}
		close(inputPipe[1]);

		std::string output = readFromPipe(outputPipe[0]);
		close(outputPipe[0]);

		int status;
		waitpid(pid, &status, 0);

		return output;
	}
}

// private methods
std::string readFromPipe(int fd)
{
	std::string result;
	char buffer[4096];
	ssize_t bytesRead;

	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
		result.append(buffer, bytesRead);
	}

	if (bytesRead == -1) {
		throw std::runtime_error("Failed to read from pipe");
	}

	return result;
}

void writeToPipe(int fd, const std::string& data)
{
	ssize_t totalWritten = 0;
	ssize_t dataSize = data.size();

	while (totalWritten < dataSize) {
		ssize_t bytesWritten = write(fd, data.c_str() + totalWritten, dataSize - totalWritten);
		if (bytesWritten == -1) {
			throw std::runtime_error("Failed to write to pipe");
		}
		totalWritten += bytesWritten;
	}
}
