ParseResult tokenize_inner(std::vector<Token>& tokens, std::string_view string) {
    const char* str = string.data();

    size_t length = string.length();

    {
        // Early pass to avoid recursion
        // into invalid patterns
        size_t paren_depth = 0;
        for (size_t i = 0; i < length; ++i) {
            switch (str[i]) {
                case QuotationStart:
                    ++paren_depth;
                    break;
                case QuotationEnd:
                    if (!paren_depth--) return ErrorParse;
                    break;
            }
        }
        if (paren_depth) {
            return ErrorParse;
        }
    }

    for (size_t i = 0; i < length; ++i) {
        switch (string[i]) {
            case QuotationStart: {
                size_t paren_depth = 1;
                for (size_t j = i + 1; j < length; ++j) {
                    switch (str[j]) {
                        case QuotationStart:
                            ++paren_depth;
                            break;
                        case QuotationEnd:
                            if (!--paren_depth) {
                                tokens.emplace_back(&str[i], (j - i) + 1);
                                i = j;
                                goto next_char;
                            }
                            break;
                    }
                }
            }
            case QuotationEnd:
                return ErrorParse;
            case Swap:
                if (tokens.size() <= 1) return ErrorParse;
                std::swap(tokens.back(), tokens.end()[-2]);
                break;
            case Duplicate:
                if (tokens.size() == 0) return ErrorParse;
                tokens.reserve(tokens.size() + 1);
                tokens.push_back(tokens.back());
                break;
            case Nuke:
                if (tokens.size() == 0) return ErrorParse;
                tokens.pop_back();
                break;
            case Ungroup: {
                if (tokens.size() == 0) return ErrorParse;
                if (tokens.back().starts_with(QuotationStart) && tokens.back().ends_with(QuotationEnd)) {
                    Token prev_token = tokens.back();
                    tokens.pop_back();
                    std::string_view token_view = prev_token;
                    token_view.remove_prefix(1);
                    token_view.remove_suffix(1);
                    switch (tokenize_inner(tokens, token_view)) {
                        default:
                            unreachable;
                        case NormalParse:
                            break;
                        case IrreducibleParse:
                            if (i + 1 != length) {
                                tokens.reserve(tokens.size() + 1);
                                tokens.emplace_back(&str[i + 1], length - (i + 1));
                            }
                            return IrreducibleParse;
                        case ErrorParse:
                            return ErrorParse;
                    }
                    break;
                }
            }
            default:
                tokens.emplace_back(&str[i], length - i);
                return IrreducibleParse;
            case Group:
                if (tokens.size() == 0) return ErrorParse;
                tokens.back().insert(0, 1, '(').push_back(')');
                break;
            case Merge:
                if (tokens.size() <= 1) return ErrorParse;
                if (
                    tokens.back().starts_with(QuotationStart) && tokens.back().ends_with(QuotationEnd) &&
                    tokens.end()[-2].starts_with(QuotationStart) && tokens.end()[-2].ends_with(QuotationEnd)
                ) {
                    Token prev_token = tokens.back();
                    tokens.pop_back();
                    tokens.back().pop_back();
                    tokens.back().append(prev_token.data() + 1, prev_token.size() - 1);
                    break;
                }
                return ErrorParse;
        }
    next_char:;
    }
    return NormalParse;
}