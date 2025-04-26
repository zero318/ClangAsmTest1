#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#if _WIN32
#include <windows.h>
#endif

#define USE_STACK_LIMIT 0

#if !_WIN32
#undef USE_STACK_LIMIT
#define USE_STACK_LIMIT 1
#endif

#if USE_STACK_LIMIT
#define STACK_LIMIT_ARG , size_t depth = 0
#define STACK_LIMIT_NEXT , depth + 1
#else
#define STACK_LIMIT_ARG
#define STACK_LIMIT_NEXT
#endif

static inline constexpr size_t MAX_DEPTH = 1000;

enum TokenType : uint8_t {
    QuotationStart = '(',
    QuotationEnd = ')',
    Swap = '~',
    Duplicate = '+',
    Nuke = '-',
    Ungroup = '<',
    Group = '>',
    Merge = ','
};

static constexpr uint8_t token_chars[256] = {
    [0] = QuotationStart,
    [QuotationStart] = QuotationEnd,
    [QuotationEnd] = Swap,
    [Swap] = Duplicate,
    [Duplicate] = Nuke,
    [Nuke] = Ungroup,
    [Ungroup] = Group,
    [Group] = Merge,
    [Merge] = 0,
};

enum ParseResult {
    ErrorParse,
    NormalParse,
    IrreducibleParse
};

struct Token {
    size_t length;
    char buffer[];

    inline bool is_parenthesized() const {
        return this->buffer[0] == '(' && this->buffer[this->length - 1] == ')';
    }
};

struct TokenList {
    size_t count;
    Token* temp;
    Token* tokens[];
};

inline void cleanup_tokens(const TokenList* list) {
    size_t list_length = list->count;
    for (size_t i = 0; i < list_length; ++i) {
        free(list->tokens[i]);
    }
    if (Token* temp = list->temp) {
        free(temp);
    }
}
inline Token* allocate_token(const char* start, size_t length) {
    if (Token* token = (Token*)malloc(sizeof(Token) + length)) {
        token->length = length;
        memcpy(token->buffer, start, length);
        return token;
    }
    return NULL;
}

ParseResult tokenize_inner(TokenList*& master_token_list, TokenList*& master_source_list STACK_LIMIT_ARG) {

#if USE_STACK_LIMIT
    if (depth == MAX_DEPTH) {
        return ErrorParse;
    }
#endif

    TokenList* source_list = master_source_list;
    Token* source_token = source_list->tokens[source_list->count - 1];
    size_t length = source_token->length;
    const char* string = source_token->buffer;

    {
        // Early pass to avoid recursion
        // into invalid patterns
        size_t paren_depth = 0;
        for (size_t i = 0; i < length; ++i) {
            switch (string[i]) {
                case QuotationStart:
                    ++paren_depth;
                    break;
                case QuotationEnd:
                    if (!paren_depth--) return ErrorParse;
                    break;
            }
        }
        if (paren_depth) return ErrorParse;
    }

    TokenList* token_list = master_token_list;

#if !USE_STACK_LIMIT
    // Evil hack to make sure the stack
    // won't overflow in the middle of a
    // heap management libcall
#if __x86_64__
    __asm TEST BYTE PTR [RSP-0x1000], 0
#else
    __asm TEST BYTE PTR [ESP-0x1000], 0
#endif
#endif

    for (size_t i = 0; i < length; ++i) {
        switch (string[i]) {
            case QuotationStart: {
                size_t paren_depth = 1;
                for (size_t j = i + 1; j < length; ++j) {
                    switch (string[j]) {
                        case QuotationStart:
                            ++paren_depth;
                            break;
                        case QuotationEnd:
                            if (!--paren_depth) {
                                if (Token* new_token = allocate_token(&string[i], (j - i) + 1)) {
                                    size_t prev_count = token_list->count;
                                    token_list->temp = new_token;
                                    if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
                                        master_token_list = token_list = new_list;
                                        token_list->temp = NULL;
                                        token_list->tokens[prev_count] = new_token;
                                        token_list->count = prev_count + 1;
                                        i = j;
                                        goto next_char;
                                    }
                                    token_list->temp = NULL;
                                    free(new_token);
                                }
                                cleanup_tokens(token_list);
                                free(token_list);
                                master_token_list = NULL;
                                return ErrorParse;
                            }
                            break;
                    }
                }
            }
            case QuotationEnd:
                cleanup_tokens(token_list);
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            case Swap:
                switch (size_t count = token_list->count) {
                    default:
                        std::swap(token_list->tokens[count - 1], token_list->tokens[count - 2]);
                        continue;
                    case 1:
                        free(token_list->tokens[0]);
                    case 0:
                        free(token_list);
                        master_token_list = NULL;
                        return ErrorParse;
                }
            case Duplicate:
                if (size_t prev_count = token_list->count) {
                    Token* token = token_list->tokens[prev_count - 1];
                    if (Token* new_token = allocate_token(token->buffer, token->length)) {
                        token_list->temp = new_token;
                        if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
                            master_token_list = token_list = new_list;
                            token_list->temp = NULL;
                            token_list->tokens[prev_count] = new_token;
                            token_list->count = prev_count + 1;
                            continue;
                        }
                        token_list->temp = NULL;
                        free(new_token);
                    }
                    cleanup_tokens(token_list);
                }
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            case Nuke:
                if (size_t count = token_list->count) {
                    --count;
                    free(token_list->tokens[count]);
                    token_list->count = count;
                    continue;
                }
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            case Ungroup:
                if (size_t count = token_list->count) {
                    Token* token = token_list->tokens[count - 1];
                    if (!token->is_parenthesized()) {
                        goto irreducible;
                    }

                    size_t source_count = source_list->count;
                    if (TokenList* new_list = (TokenList*)realloc(source_list, sizeof(TokenList) + sizeof(Token*) * (source_count + 1))) {
                        master_source_list = source_list = new_list;
                        token_list->count = count - 1;
                        source_list->temp = NULL;
                        source_list->tokens[source_count] = token;
                        source_list->count = source_count + 1;

                        size_t old_size = token->length;
                        token->length -= 2;
                        size_t new_size = old_size - 2;
                        for (size_t i = 0; i < new_size; ++i) {
                            token->buffer[i] = token->buffer[i + 1];
                        }

                        switch (ParseResult result = tokenize_inner(master_token_list, master_source_list STACK_LIMIT_NEXT)) {
                            default: // ErrorParse
                                // no cleanup here because the inner call
                                // will already have freed everything
                                return result;
                            case NormalParse:
                                token_list = master_token_list;
                                source_list = master_source_list;
                                continue;
                            case IrreducibleParse:
                                token_list = master_token_list;
                                source_list = master_source_list;
                                ++i;
                                goto irreducible;
                        }
                    }
                    cleanup_tokens(token_list);
                }
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            case Group:
                if (size_t count = token_list->count) {
                    Token* token = token_list->tokens[count - 1];
                    size_t old_size = token->length;
                    if (Token* bigger_token = (Token*)realloc(token, sizeof(Token) + old_size + 2)) {
                        token_list->tokens[count - 1] = bigger_token;
                        bigger_token->length += 2;
                        size_t write_offset = old_size + 1;
                        bigger_token->buffer[write_offset] = ')';
                        while (--write_offset) {
                            bigger_token->buffer[write_offset] = bigger_token->buffer[write_offset - 1];
                        }
                        bigger_token->buffer[0] = '(';
                        continue;
                    }
                    cleanup_tokens(token_list);
                }
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            case Merge:
                switch (size_t count = token_list->count) {
                    default: {
                        Token* left = token_list->tokens[count - 2];
                        Token* right = token_list->tokens[count - 1];
                        if (!left->is_parenthesized() || !right->is_parenthesized()) {
                            goto irreducible;
                        }
                        size_t old_size = left->length;
                        size_t expansion = right->length;
                        if (Token* bigger_left = (Token*)realloc(left, sizeof(Token) + old_size + expansion - 2)) {
                            token_list->tokens[count - 2] = bigger_left;
                            bigger_left->length += expansion - 2;
                            memcpy(bigger_left->buffer + old_size - 1, right->buffer + 1, expansion - 1);
                            free(right);
                            token_list->count = count - 1;
                            continue;
                        }
                        cleanup_tokens(token_list);
                        break;
                    }
                    case 1:
                        free(token_list->tokens[0]);
                    case 0:;
                }
                free(token_list);
                master_token_list = NULL;
                return ErrorParse;
            default:
                goto irreducible;
        }
    irreducible:
        if (!(length - i)) {
            return IrreducibleParse;
        }
        if (Token* new_token = allocate_token(&string[i], length - i)) {
            size_t prev_count = token_list->count;
            token_list->temp = new_token;
            if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
                master_token_list = token_list = new_list;
                token_list->temp = NULL;
                token_list->tokens[prev_count] = new_token;
                token_list->count = prev_count + 1;
                return IrreducibleParse;
            }
            token_list->temp = NULL;
            free(new_token);
        }
        cleanup_tokens(token_list);
        free(token_list);
        master_token_list = NULL;
        return ErrorParse;
    next_char:;
    }
    return NormalParse;
}

Token* tokenize_whatever(const char* string, size_t length) {
    if (Token* source_token = allocate_token(string, length)) {
        if (TokenList* master_token_list = (TokenList*)malloc(sizeof(TokenList))) {
            master_token_list->count = 0;
            master_token_list->temp = NULL;
            if (TokenList* master_source_list = (TokenList*)malloc(sizeof(TokenList) + sizeof(Token*))) {
                master_source_list->count = 1;
                master_source_list->temp = NULL;
                master_source_list->tokens[0] = source_token;

                Token* ret = NULL;
#if !USE_STACK_LIMIT
                __try {
#endif
                    if (tokenize_inner(master_token_list, master_source_list) != ErrorParse) {
                        if (size_t token_count = master_token_list->count) {
                            size_t buffer_size = 0;
                            for (size_t i = 0; i < token_count; ++i) {
                                buffer_size += master_token_list->tokens[i]->length;
                            }
                            if (Token* return_token = (Token*)malloc(sizeof(Token) + buffer_size)) {
                                ret = return_token;
                                return_token->length = buffer_size;
                                char* token_write = return_token->buffer;
                                for (size_t i = 0; i < token_count; ++i) {
                                    Token* token = master_token_list->tokens[i];
                                    size_t length = token->length;
                                    token_write = (char*)memcpy(token_write, token->buffer, length);
                                    token_write += length;
                                }
                            }
                        }
                    }
#if !USE_STACK_LIMIT
                }
                __except (
                    GetExceptionCode() == STATUS_STACK_OVERFLOW ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH
                ) {
                    // At least fix the stack but otherwise ignore it
                    _resetstkoflw();
                    if (ret) {
                        free(ret);
                        ret = NULL;
                    }
                }
#endif
                if (master_token_list) {
                    cleanup_tokens(master_token_list);
                    free(master_token_list);
                }
                if (master_source_list) {
                    cleanup_tokens(master_source_list);
                    free(master_source_list);
                }

                return ret;
            }
            free(master_token_list);
        }
        free(source_token);
    }
    return NULL;
}

void find_shortest_patterns(std::string_view prefix, std::string_view known_good) {
    char buffer[prefix.length() + known_good.length() + 1];
    buffer[prefix.length() + known_good.length()] = '\0';
    memcpy(buffer, prefix.data(), prefix.length());
    char* test = &buffer[prefix.length()];
    memcpy(test, known_good.data(), known_good.length());
    
    Token* correct = tokenize_whatever(buffer, prefix.length() + known_good.length());
    if (!correct) {
        printf("Parse error in known good pattern");
        return;
    }

    memset(test, 0, known_good.length());

#if !USE_STACK_LIMIT
    ULONG stack_handler_size = 1024;
    SetThreadStackGuarantee(&stack_handler_size);
#endif

    std::vector<std::string> matches;
    size_t min_length = known_good.length();

    size_t test_length = 1;
    for (;;) {
    next_permutation:
        size_t i = 0;
        for (;;) {
            uint8_t next = token_chars[test[i]];
            if (next) {
                switch (next) {
                    case QuotationStart:
                        // Skip patterns that end with an
                        // open/empty group since those are
                        // always invalid.
                        if (
                            i + 1 == test_length ||
                            test[i + 1] == QuotationEnd
                        ) {
                            next = QuotationEnd;
                        } //else {
                            break;
                        /* I don't fully trust this transform,
                        }  replaced with lower loop
                        // fallthrough is intentional
                    case QuotationEnd:
                        for (
                            size_t j = i;
                            j && test[j - 1] == QuotationStart;
                            --j
                        ) {
                            test[j - 1] = QuotationEnd;
                        }
                        break;
                        */
                    case Swap:
                        // Double swap *should* be a nop
                        if (test[i + 1] != Swap) {
                            break;
                        }
                        next = Duplicate;
                        // fallthrough is intentional
                    case Duplicate:
                        if (test[i + 1] != Nuke) {
                            break;
                        }
                        next = Nuke;
                        break;
                    case Group:
                        if (test[i + 1] == Ungroup) {
                            next = Merge;
                        }
                }
                test[i] = next;
                break;
            } else {
                test[i++] = QuotationStart;
                if (i == min_length) {
                    goto end_testing;
                }
                test_length += i == test_length;
            }
        }

        // Filter out empty parentheses
        for (size_t j = 0; j < test_length - 1; ++j) {
            if (test[j] == '(' & test[j + 1] == ')') {
                goto next_permutation;
            }
        }
        
        if (Token* token = tokenize_whatever(buffer, prefix.length() + test_length)) {
            size_t length = token->length;
            if (
                length == correct->length &&
                !memcmp(token->buffer, correct->buffer, length)
            ) {
                if (test_length < min_length) {
                    min_length = test_length;
                    matches.clear();
                }
                matches.emplace_back(test, test_length);
            }
            else {
                free(token);
            }
        }
    }
end_testing:

    for (auto& match : matches) {
        printf("%s\n", match.data());
    }
}