#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

// message types
enum class MessageType 
{
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN
};

// log message class
class LogMessage 
{
private:
    MessageType __type;
    std::string __message;

public:
    LogMessage(MessageType type, const std::string& message)
        : __type(type), __message(message) {
    }

    MessageType type() const 
    {
        return __type;
    }

    const std::string& message() const 
    {
        return __message;
    }
};

// base handler
class Handler 
{
protected:
    std::unique_ptr<Handler> __next;

public:
    virtual ~Handler() = default;

    void setNext(std::unique_ptr<Handler> next) 
    {
        __next = std::move(next);
    }

    void handle(const LogMessage& message) 
    {
        if (canHandle(message)) 
        {
            process(message);
        }
        else if (__next) 
        {
            __next->handle(message);
        }
    }

protected:
    virtual bool canHandle(const LogMessage& message) const = 0;
    virtual void process(const LogMessage& message) = 0;
};

// fatal error handler (throw exception)
class FatalErrorHandler : public Handler 
{
protected:
    bool canHandle(const LogMessage& message) const override 
    {
        return message.type() == MessageType::FATAL_ERROR;
    }

    void process(const LogMessage& message) override 
    {
        throw std::runtime_error("FATAL ERROR: " + message.message());
    }
};

// error handler (file)
class ErrorHandler : public Handler 
{
private:
    std::string filePath;

public:
    explicit ErrorHandler(const std::string& path) : filePath(path) {}

protected:
    bool canHandle(const LogMessage& message) const override
    {
        return message.type() == MessageType::ERROR;
    }

    void process(const LogMessage& message) override 
    {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "[ERROR]: " << message.message() << std::endl;
            file.close();
            std::cout << "[LOG] error was written to the file: " << filePath << std::endl;
        }
        else {
            std::cerr << "Opening file error!" << filePath << std::endl;
        }
    }
};

// warning handler (console)
class WarningHandler : public Handler 
{
protected:
    bool canHandle(const LogMessage& message) const override 
    {
        return message.type() == MessageType::WARNING;
    }

    void process(const LogMessage& message) override 
    {
        std::cout << "[WARNING]: " << message.message() << std::endl;
    }
};

// unknown message handler (throw exception)
class UnknownHandler : public Handler 
{
protected:
    bool canHandle(const LogMessage& message) const override 
    {
        return message.type() == MessageType::UNKNOWN;
    }

    void process(const LogMessage& message) override 
    {
        throw std::runtime_error("UNKNOWN MESSAGE TYPE: " + message.message());
    }
};

// function to create handler chain
std::unique_ptr<Handler> createHandlerChain(const std::string& errorLogPath) 
{
    // create handlers
    auto fatalHandler = std::make_unique<FatalErrorHandler>();
    auto errorHandler = std::make_unique<ErrorHandler>(errorLogPath);
    auto warningHandler = std::make_unique<WarningHandler>();
    auto unknownHandler = std::make_unique<UnknownHandler>();

    // create chain: fatal -> error -> warning -> unknown
    errorHandler->setNext(std::move(warningHandler));
    fatalHandler->setNext(std::move(errorHandler));
    fatalHandler->setNext(std::move(unknownHandler));

    return fatalHandler;
}

int main() {
 
    auto handlerChain = createHandlerChain("errors.log");

    // test messages
    std::vector<LogMessage> messages = {
        LogMessage(MessageType::WARNING, "warining message 1"),
        LogMessage(MessageType::WARNING, "warining message 2"),
        LogMessage(MessageType::ERROR, "error message 1"),
        LogMessage(MessageType::ERROR, "error message 1"),
        LogMessage(MessageType::FATAL_ERROR, "fatal error"),
        LogMessage(MessageType::UNKNOWN, "unknown message")
    };


    for (const auto& msg : messages) 
    {
        std::cout << "\n --- Process message : " << msg.message() << " ---" << std::endl;

        try 
        {
            handlerChain->handle(msg);
            std::cout << "message was processed successfully" << std::endl;
        }
        catch (const std::runtime_error& e) 
        {
            std::cerr << "Exception: " << e.what() << std::endl;

            // stop running if it is a fatal error
            if (msg.type() == MessageType::FATAL_ERROR) 
            {
                std::cerr << "Fatal error!" << std::endl;
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}