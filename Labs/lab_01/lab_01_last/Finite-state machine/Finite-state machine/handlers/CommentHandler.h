#ifndef COMMENTHANDLER_H
#define COMMENTHANDLER_H

#include <string>

struct Multycom {
    bool combination;
    bool slash;
    bool star;
};

struct Simplecom {
    bool combination;
};

struct QuoteHandler {
    bool inSingleQuotes;
    bool inDoubleQuotes;
};

struct SpecialCharHandler {
    bool backslash;
};

class CommentHandler {
protected:
    Multycom mcom;
    Simplecom scom;
    QuoteHandler qhand;
    SpecialCharHandler schand;

public:
    virtual void ProcessComments(const std::string& input, std::string& output) = 0; // Чисто виртуальный метод
};

#endif
