#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// base observer
class Observer 
{
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

// observable class
class Observable 
{
private:
    std::vector<Observer*> observers;

public:
    // add observers
    void addObserver(Observer* observer) {
        if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) 
        {
            observers.push_back(observer);
        }
    }

    // remove observer
    void removeObserver(Observer* observer) 
    {
        auto it = std::remove(observers.begin(), observers.end(), observer);
         observers.erase(it, observers.end());
    }

    // warning
    void warning(const std::string& message) const 
    {
        for (Observer* observer : observers) {
            if (observer) {
                observer->onWarning(message);
            }
        }
    }

    // error
    void error(const std::string& message) const 
    {
        for (Observer* observer : observers) {
            if (observer) {
                observer->onError(message);
            }
        }
    }

    // fatal error
    void fatalError(const std::string& message) const 
    {
        for (Observer* observer : observers) {
            if (observer) {
                observer->onFatalError(message);
            }
        }
    }
};

// warning observer (console)
class WarningObserver : public Observer 
{
public:
    void onWarning(const std::string& message) override 
    {
        std::cout << "[WARNING]: " << message << std::endl;
    }
};

// error observer (file)
class ErrorObserver : public Observer 
{
private:
    std::string filePath;

public:
    explicit ErrorObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override 
    {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) 
        {
            file << "[ERROR]: " << message << std::endl;
            file.close();
        }
        else 
        {
            std::cerr << "Opening file error!" << filePath << std::endl;
        }
    }
};

// fatal observer (console and file)
class FatalErrorObserver : public Observer 
{
private:
    std::string filePath;

public:
    explicit FatalErrorObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override
    {
        // console
        std::cout << "[FATAL ERROR]: " << message << std::endl;

        // file
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) 
        {
            file << "[FATAL ERROR]: " << message << std::endl;
            file.close();
        }
        else 
        {
            std::cerr << "Opening file error!" << filePath << std::endl;
        }
    }
};


int main() {
    
    Observable logger;

    WarningObserver warningLogger;
    ErrorObserver errorLogger("errors.txt");
    FatalErrorObserver fatalLogger("fatal_errors.txt");

    
    logger.addObserver(&warningLogger);
    logger.addObserver(&errorLogger);
    logger.addObserver(&fatalLogger);

    std::cout << "*** warnings ***" << std::endl;

    logger.warning("low level of fuel");
    logger.warning("too high temperature");

    std::cout << std::endl;

    std::cout << "*** errors ***" << std::endl;

    logger.error("opening file error");
    logger.error("some runtime error");

    std::cout << std::endl;

    std::cout << "*** fatal errors ***" << std::endl;

    logger.fatalError("Some fatal error 1");
    logger.fatalError("Some fatal error 2");

    std::cout << std::endl;

    std::cout << "*** remove observer ***" << std::endl;

    logger.removeObserver(&warningLogger);
    logger.warning("some test warning");

    return 0;
}