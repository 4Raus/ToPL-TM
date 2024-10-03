#ifndef COMMENTRECOVER_H
#define COMMENTRECOVER_H

#include "CommentHandler.h"
#include <string>
#include <vector>

class CommentRecover : public CommentHandler {
public:
    CommentRecover();
    void ProcessDeletingCommentSimple(const std::string& filename, const std::vector<std::string>& searchPaths);
    void ProcessDeletingComments(const std::string& filename, const std::string& driveLetter);
};

#endif