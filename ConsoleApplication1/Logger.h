#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>

class Simulator;
using namespace std;

class Logger
{
public:
	enum class Level { Info, Debug, Error };
	Logger();
	~Logger();

	void set_level(Level level)
	{
		level_ = level;
	}

	void Info(std::string message, int text_mode, Simulator* simulator);
	void Debug(std::string message, int text_mode, Simulator* simulator);
	void Error(std::string message, int text_mode, Simulator* simulator);

	void save_in_file(string text, Simulator* simulator);

private:
	std::string file_path_;
	Level level_ = Level::Info; // level Info: Info Error, Debug:Info,Debug Error, Error: Error



};

#endif

