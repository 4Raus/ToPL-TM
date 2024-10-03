#ifndef MULTILINECOMMENTHANDLER_H
#define MULTILINECOMMENTHANDLER_H

#include <string>

class MultiLineCommentHandler {
public:
    void RemoveMultiLineComments(const std::string& input, std::string& output);

private:
    bool inComment = false;
    bool maybeStart = false;
    bool maybeEnd = false;
};

#endif
