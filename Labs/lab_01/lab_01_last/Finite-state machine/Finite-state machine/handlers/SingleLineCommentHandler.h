#ifndef SINGLELINECOMMENTHANDLER_H
#define SINGLELINECOMMENTHANDLER_H

#include "CommentHandler.h"

class SingleLineCommentHandler : public CommentHandler {
public:
    void ProcessComments(const std::string& input, std::string& output) override;
};

#endif