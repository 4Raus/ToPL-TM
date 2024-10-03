#include "SingleLineCommentHandler.h"

void SingleLineCommentHandler::ProcessComments(const std::string& input, std::string& output) {
    char cur;
    for (int i = 0; i < input.length(); i++) {
        cur = input[i];

        if (schand.backslash) {
            schand.backslash = false;
            output += cur;
            continue;
        }

        if (scom.combination) {
            if (cur == '\n') {
                scom.combination = false;
                output += cur;
            }
            continue;
        }

        if (cur == '/') {
            if (i + 1 < input.length() && input[i + 1] == '/') {
                scom.combination = true;
                i++;
                continue;
            }
        }

        output += cur;
    }
}
