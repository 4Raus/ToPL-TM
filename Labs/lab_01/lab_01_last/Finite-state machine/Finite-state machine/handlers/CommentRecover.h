#ifndef COMMENTRECOVER_H
#define COMMENTRECOVER_H

#include "SingleLineCommentHandler.h"
#include "MultiLineCommentHandler.h"
#include <string>
#include <vector>

class CommentRecover {
private:
    SingleLineCommentHandler singleLineHandler;
    MultiLineCommentHandler multiLineHandler;

public:
    void ProcessDeletingComments(const std::string& input, std::string& output);
    void ProcessDeletingCommentSimple(const std::string& filename, const std::vector<std::string>& searchPaths);
};

#endif
