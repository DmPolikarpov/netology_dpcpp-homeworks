#include <fstream>
#include <string>

// html interface
class IHTMLPrintable
{
public:
    virtual ~IHTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

// text interface
class ITextPrintable
{
public:
    virtual ~ITextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

// json interfase
class IJSONPrintable
{
public:
    virtual ~IJSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// html implementation
class HTMLData : public IHTMLPrintable
{
public:
    explicit HTMLData(std::string data) : data_(std::move(data)) {}

    std::string printAsHTML() const override
    {
        return "<html>" + data_ + "<html/>";
    }

private:
    std::string data_;
};

// text implementation
class TextData : public ITextPrintable
{
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}

    std::string printAsText() const override
    {
        return data_;
    }

private:
    std::string data_;
};

// json impleentation
class JSONData : public IJSONPrintable
{
public:
    explicit JSONData(std::string data) : data_(std::move(data)) {}

    std::string printAsJSON() const override
    {
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
};

// save html function
void saveToAsHTML(std::ofstream& file, const IHTMLPrintable& printable)
{
    file << printable.printAsHTML();
}

// save text function
void saveToAsText(std::ofstream& file, const ITextPrintable& printable)
{
    file << printable.printAsText();
}

// save json function
void saveToAsJSON(std::ofstream& file, const IJSONPrintable& printable)
{
    file << printable.printAsJSON();
}

int main()
{
    HTMLData htmlData("Hello");
    TextData textData("World");
    JSONData jsonData("Sample");

    std::ofstream htmlFile("out.html");
    saveToAsHTML(htmlFile, htmlData);

    std::ofstream textFile("out.txt");
    saveToAsText(textFile, textData);

    std::ofstream jsonFile("out.json");
    saveToAsJSON(jsonFile, jsonData);

    return EXIT_SUCCESS;
}