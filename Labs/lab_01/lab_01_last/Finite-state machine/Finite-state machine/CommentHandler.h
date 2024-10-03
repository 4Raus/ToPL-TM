#ifndef COMMENTHANDLER_H
#define COMMENTHANDLER_H

#include <string>
#include <vector>
#include <filesystem>

class CommentHandler {
protected:
    struct Multycom {
        bool combination = false;
        bool slash = false;
        bool star = false;
    };

    struct Simplecom {
        bool combination = false;
    };

    struct QuoteHandler {
        bool inSingleQuotes = false;
        bool inDoubleQuotes = false;
    };

    struct SpecialCharHandler {
        bool backslash = false;
    };

    Multycom mcom;
    Simplecom scom;
    QuoteHandler qhand;
    SpecialCharHandler schand;

public:
    virtual ~CommentHandler() = default;

    void DeleteComTask1(std::string& str, std::string& str_);
    void DeleteComTask2(std::string& str, std::string& str_);
};

#endif