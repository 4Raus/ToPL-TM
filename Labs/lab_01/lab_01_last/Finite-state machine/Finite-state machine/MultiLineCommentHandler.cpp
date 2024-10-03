#include "MultiLineCommentHandler.h"

void MultiLineCommentHandler::RemoveMultiLineComments(const std::string& input, std::string& output) {
    for (char cur : input) {
        switch (cur) {
        case '/':
            if (!inComment && maybeStart) {
                inComment = true;
                maybeStart = false;
                output.back() = ' ';
                output += ' ';
            }
            else if (inComment && maybeEnd) {
                inComment = false;
                maybeEnd = false;
                output += ' ';
            }
            else if (!inComment) {
                maybeStart = true;
                output += cur;
            }
            else {
                maybeEnd = true;
            }
            break;

        case '*':
            if (inComment) {
                maybeEnd = true;
            }
            else if (maybeStart) {
                inComment = true;
                maybeStart = false;
                output.back() = ' ';
                output += ' ';
            }
            else {
                output += cur;
            }
            break;

        default:
            if (inComment) {
                if (cur == '\n') {
                    output += '\n';
                }
                else {
                    output += ' ';
                }
            }
            else {
                output += cur;
            }
            maybeStart = false;
            maybeEnd = false;
            break;
        }
    }
}