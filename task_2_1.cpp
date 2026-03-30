#include <string>
#include <iostream>

class Text {
public:
    virtual void render(const std::string& data) const {
        std::cout << data;
    }
};


class DecoratedText : public Text {
public:
    explicit DecoratedText(Text* text) : text_(text) {}
    Text* text_;
};

class ItalicText : public DecoratedText {
public:
    explicit ItalicText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data)  const {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    explicit BoldText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};

class Paragraph : public DecoratedText {
public:
    explicit Paragraph(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
        std::cout << std::endl;
    }
};

class Reversed : public DecoratedText {
public:
    explicit Reversed(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::string reversed_data = data;
        std::reverse(reversed_data.begin(), reversed_data.end());
        text_->render(reversed_data);
    }
};

class Link : public DecoratedText {
public:
    explicit Link(Text* text) : DecoratedText(text) {}
    void render(const std::string& link_data, const std::string& text_data) const {
        std::cout << "<a href=";
        text_->render(link_data);
        std::cout << ">";
        text_->render(text_data);
        std::cout << "</a>";
    }
};

int main() {
    // paragraph
    auto text_block1 = new Paragraph(new Text());
    text_block1->render("Hello world");
    std::cout << std::endl;
    // reversed
    auto text_block2 = new Reversed(new Text());
    text_block2->render("Hello world");
    std::cout << std::endl;
    // link
    auto text_block3 = new Link(new Text());
    text_block3->render("netology.ru", "Hello world");
    std::cout << std::endl;
}