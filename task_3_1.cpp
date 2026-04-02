#include <iostream>
#include <fstream>
#include <string>



class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

// show in console
class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override 
    {
        std::cout << "[CONSOLE]: " << message << std::endl;
    }
};

// write to a file
class FileLogCommand : public LogCommand {
private:
    std::string filePath;

public:
    explicit FileLogCommand(const std::string& path) : filePath(path) {}

    void print(const std::string& message) override 
    {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) 
        {
            file << "[FILE]: " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Opening file error!" << filePath << std::endl;
        }
    }
};

// common function to print
void print(LogCommand& command) {
    std::string message = "Some message to print";
    command.print(message);
}


int main() 
{
    // create commands
    ConsoleLogCommand consoleLogger;
    FileLogCommand fileLogger("log.txt");

    // run commands using print function
    std::cout << "*** using print function ***" << std::endl;
    print(consoleLogger);
    print(fileLogger);

    std::cout << std::endl;

    // run commands using print methods of the classes
    std::cout << "*** using print methods of the classes ***" << std::endl;
    consoleLogger.print("Message 1");
    fileLogger.print("Message 2");

    return EXIT_SUCCESS;
}