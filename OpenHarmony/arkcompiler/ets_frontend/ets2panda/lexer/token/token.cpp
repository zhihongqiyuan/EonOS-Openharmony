/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "token.h"

namespace ark::es2panda::lexer {
bool Token::IsAccessability() const
{
    return (type_ == TokenType::LITERAL_IDENT &&
            (keywordType_ == TokenType::KEYW_PUBLIC || keywordType_ == TokenType::KEYW_PROTECTED ||
             keywordType_ == TokenType::KEYW_PRIVATE || keywordType_ == TokenType::KEYW_INTERNAL) &&
            ((flags_ & TokenFlags::HAS_ESCAPE) == 0));
}

bool Token::IsAsyncModifier() const
{
    return (type_ == TokenType::LITERAL_IDENT && keywordType_ == TokenType::KEYW_ASYNC &&
            (flags_ & TokenFlags::HAS_ESCAPE) == 0);
}

bool Token::IsForInOf() const
{
    return (keywordType_ == TokenType::KEYW_IN || keywordType_ == TokenType::KEYW_OF);
}

bool Token::IsStaticModifier() const
{
    return (type_ == TokenType::LITERAL_IDENT && keywordType_ == TokenType::KEYW_STATIC &&
            (flags_ & TokenFlags::HAS_ESCAPE) == 0);
}

bool Token::IsDeclareModifier() const
{
    return (type_ == TokenType::LITERAL_IDENT && keywordType_ == TokenType::KEYW_DECLARE &&
            (flags_ & TokenFlags::HAS_ESCAPE) == 0);
}

bool Token::IsReadonlyModifier() const
{
    return (type_ == TokenType::LITERAL_IDENT && keywordType_ == TokenType::KEYW_READONLY &&
            (flags_ & TokenFlags::HAS_ESCAPE) == 0);
}

bool Token::IsUpdate() const
{
    return (type_ == TokenType::PUNCTUATOR_MINUS_MINUS || type_ == TokenType::PUNCTUATOR_PLUS_PLUS);
}

bool Token::IsUnary() const
{
    return (type_ == TokenType::PUNCTUATOR_MINUS || type_ == TokenType::PUNCTUATOR_PLUS ||
            type_ == TokenType::PUNCTUATOR_TILDE || type_ == TokenType::PUNCTUATOR_EXCLAMATION_MARK ||
            type_ == TokenType::PUNCTUATOR_PLUS_PLUS || type_ == TokenType::PUNCTUATOR_MINUS_MINUS ||
            type_ == TokenType::KEYW_TYPEOF || type_ == TokenType::KEYW_VOID || type_ == TokenType::KEYW_DELETE ||
            type_ == TokenType::KEYW_AWAIT || type_ == TokenType::PUNCTUATOR_DOLLAR_DOLLAR);
}

bool Token::IsPropNameLiteral() const
{
    return (type_ == TokenType::LITERAL_STRING || type_ == TokenType::LITERAL_NUMBER ||
            type_ == TokenType::LITERAL_TRUE || type_ == TokenType::LITERAL_FALSE);
}

bool Token::IsKeyword() const
{
    return (type_ >= TokenType::FIRST_KEYW);
}

bool Token::IsReservedTypeName() const
{
    switch (keywordType_) {
        case TokenType::KEYW_ANY:
        case TokenType::KEYW_UNKNOWN:
        case TokenType::KEYW_NEVER:
        case TokenType::KEYW_BOOLEAN:
        case TokenType::KEYW_VOID:
        case TokenType::KEYW_OBJECT:
            return true;
        default:
            return false;
    }
}

bool Token::IsDefinableTypeName() const
{
    switch (keywordType_) {
        case TokenType::KEYW_BOOLEAN:
        case TokenType::KEYW_DOUBLE:
        case TokenType::KEYW_BYTE:
        case TokenType::KEYW_FLOAT:
        case TokenType::KEYW_SHORT:
        case TokenType::KEYW_INT:
        case TokenType::KEYW_CHAR:
        case TokenType::KEYW_LONG:
        case TokenType::KEYW_TYPE:
        case TokenType::KEYW_UNDEFINED:
        case TokenType::KEYW_VAR:
        case TokenType::KEYW_IS:
        case TokenType::KEYW_TYPEOF:
        case TokenType::KEYW_YIELD:
        case TokenType::KEYW_STRING:
        case TokenType::KEYW_NUMBER:
        case TokenType::KEYW_BIGINT:
        case TokenType::KEYW_VOID:
            return true;
        default:
            return false;
    }
}

bool Token::IsBinaryToken(TokenType type)
{
    return (type >= TokenType::PUNCTUATOR_NULLISH_COALESCING && type <= TokenType::PUNCTUATOR_EXPONENTIATION);
}

bool Token::IsBinaryLvalueToken(TokenType type)
{
    return (type >= TokenType::PUNCTUATOR_SUBSTITUTION && type <= TokenType::PUNCTUATOR_EXPONENTIATION_EQUAL);
}

bool Token::IsUpdateToken(TokenType type)
{
    return (type == TokenType::PUNCTUATOR_MINUS_MINUS || type == TokenType::PUNCTUATOR_PLUS_PLUS);
}

bool Token::IsPunctuatorToken(TokenType type)
{
    return (type >= TokenType::PUNCTUATOR_NULLISH_COALESCING && type <= TokenType::PUNCTUATOR_ARROW);
}

bool Token::IsTsParamToken(TokenType type)
{
    return (type == TokenType::PUNCTUATOR_COLON || type == TokenType::PUNCTUATOR_QUESTION_MARK);
}

// CC-OFFNXT(huge_method,huge_cyclomatic_complexity,G.FUN.01-CPP) big switch-case, solid logic
const char *TokenToString(TokenType type)  // NOLINT(readability-function-size)
{
    switch (type) {
        case TokenType::PUNCTUATOR_BITWISE_AND:
            return "&";
        case TokenType::PUNCTUATOR_BITWISE_OR:
            return "|";
        case TokenType::PUNCTUATOR_MULTIPLY:
            return "*";
        case TokenType::PUNCTUATOR_DIVIDE:
            return "/";
        case TokenType::PUNCTUATOR_MINUS:
            return "-";
        case TokenType::PUNCTUATOR_EXCLAMATION_MARK:
            return "!";
        case TokenType::PUNCTUATOR_TILDE:
            return "~";
        case TokenType::PUNCTUATOR_MINUS_MINUS:
            return "--";
        case TokenType::PUNCTUATOR_LEFT_SHIFT:
            return "<<";
        case TokenType::PUNCTUATOR_RIGHT_SHIFT:
            return ">>";
        case TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
            return "<=";
        case TokenType::PUNCTUATOR_GREATER_THAN_EQUAL:
            return ">=";
        case TokenType::PUNCTUATOR_MOD:
            return "%";
        case TokenType::PUNCTUATOR_BITWISE_XOR:
            return "^";
        case TokenType::PUNCTUATOR_EXPONENTIATION:
            return "**";
        case TokenType::PUNCTUATOR_MULTIPLY_EQUAL:
            return "*=";
        case TokenType::PUNCTUATOR_EXPONENTIATION_EQUAL:
            return "**=";
        case TokenType::PUNCTUATOR_ARROW:
            return "=>";
        case TokenType::PUNCTUATOR_BACK_TICK:
            return "`";
        case TokenType::PUNCTUATOR_HASH_MARK:
            return "#";
        case TokenType::PUNCTUATOR_DIVIDE_EQUAL:
            return "/=";
        case TokenType::PUNCTUATOR_MOD_EQUAL:
            return "%=";
        case TokenType::PUNCTUATOR_MINUS_EQUAL:
            return "-=";
        case TokenType::PUNCTUATOR_LEFT_SHIFT_EQUAL:
            return "<<=";
        case TokenType::PUNCTUATOR_RIGHT_SHIFT_EQUAL:
            return ">>=";
        case TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT:
            return ">>>";
        case TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL:
            return ">>>=";
        case TokenType::PUNCTUATOR_BITWISE_AND_EQUAL:
            return "&=";
        case TokenType::PUNCTUATOR_BITWISE_OR_EQUAL:
            return "|=";
        case TokenType::PUNCTUATOR_LOGICAL_AND_EQUAL:
            return "&&=";
        case TokenType::PUNCTUATOR_NULLISH_COALESCING:
            return "??";
        case TokenType::PUNCTUATOR_LOGICAL_OR_EQUAL:
            return "||=";
        case TokenType::PUNCTUATOR_LOGICAL_NULLISH_EQUAL:
            return "\?\?=";
        case TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL:
            return "^=";
        case TokenType::PUNCTUATOR_PLUS:
            return "+";
        case TokenType::PUNCTUATOR_PLUS_PLUS:
            return "++";
        case TokenType::PUNCTUATOR_PLUS_EQUAL:
            return "+=";
        case TokenType::PUNCTUATOR_LESS_THAN:
            return "<";
        case TokenType::PUNCTUATOR_GREATER_THAN:
            return ">";
        case TokenType::PUNCTUATOR_EQUAL:
            return "==";
        case TokenType::PUNCTUATOR_NOT_EQUAL:
            return "!=";
        case TokenType::PUNCTUATOR_STRICT_EQUAL:
            return "===";
        case TokenType::PUNCTUATOR_NOT_STRICT_EQUAL:
            return "!==";
        case TokenType::KEYW_INSTANCEOF:
            return "instanceof";
        case TokenType::KEYW_IN:
            return "in";
        case TokenType::PUNCTUATOR_LOGICAL_AND:
            return "&&";
        case TokenType::PUNCTUATOR_LOGICAL_OR:
            return "||";
        case TokenType::PUNCTUATOR_SUBSTITUTION:
            return "=";
        case TokenType::PUNCTUATOR_QUESTION_MARK:
            return "?";
        case TokenType::PUNCTUATOR_QUESTION_DOT:
            return "?.";
        case TokenType::PUNCTUATOR_AT:
            return "@";
        case TokenType::PUNCTUATOR_FORMAT:
            return "@@";
        case TokenType::PUNCTUATOR_RIGHT_PARENTHESIS:
            return ")";
        case TokenType::PUNCTUATOR_LEFT_PARENTHESIS:
            return "(";
        case TokenType::PUNCTUATOR_RIGHT_SQUARE_BRACKET:
            return "]";
        case TokenType::PUNCTUATOR_LEFT_SQUARE_BRACKET:
            return "[";
        case TokenType::PUNCTUATOR_RIGHT_BRACE:
            return "}";
        case TokenType::PUNCTUATOR_PERIOD:
            return ".";
        case TokenType::PUNCTUATOR_PERIOD_PERIOD_PERIOD:
            return "...";
        case TokenType::PUNCTUATOR_PERIOD_QUESTION:
            return ".?";
        case TokenType::PUNCTUATOR_LEFT_BRACE:
            return "{";
        case TokenType::PUNCTUATOR_SEMI_COLON:
            return ";";
        case TokenType::PUNCTUATOR_COLON:
            return ":";
        case TokenType::PUNCTUATOR_COMMA:
            return ",";
        case TokenType::KEYW_GET:
            return "get";
        case TokenType::KEYW_SET:
            return "set";
        case TokenType::KEYW_OF:
            return "of";
        case TokenType::KEYW_FROM:
            return "from";
        case TokenType::KEYW_AS:
            return "as";
        case TokenType::KEYW_META:
            return "meta";
        case TokenType::KEYW_ASSERT:
            return "assert";
        case TokenType::KEYW_REQUIRE:
            return "require";
        case TokenType::KEYW_ABSTRACT:
            return "abstract";
        case TokenType::KEYW_ANY:
            return "any";
        case TokenType::KEYW_AWAIT:
            return "await";
        case TokenType::KEYW_BIGINT:
            return "bigint";
        case TokenType::KEYW_BOOLEAN:
            return "boolean";
        case TokenType::KEYW_BREAK:
            return "break";
        case TokenType::KEYW_BYTE:
            return "byte";
        case TokenType::KEYW_CASE:
            return "case";
        case TokenType::KEYW_CATCH:
            return "catch";
        case TokenType::KEYW_CHAR:
            return "char";
        case TokenType::KEYW_CONST:
            return "const";
        case TokenType::KEYW_CONSTRUCTOR:
            return "constructor";
        case TokenType::KEYW_CONTINUE:
            return "continue";
        case TokenType::KEYW_DEBUGGER:
            return "debugger";
        case TokenType::KEYW_DEFAULT:
            return "default";
        case TokenType::KEYW_DO:
            return "do";
        case TokenType::KEYW_ENUM:
            return "enum";
        case TokenType::KEYW_DOUBLE:
            return "double";
        case TokenType::KEYW_EXPORT:
            return "export";
        case TokenType::KEYW_FINALLY:
            return "finally";
        case TokenType::KEYW_FLOAT:
            return "float";
        case TokenType::KEYW_FOR:
            return "for";
        case TokenType::KEYW_FUNCTION:
            return "function";
        case TokenType::KEYW_IF:
            return "if";
        case TokenType::KEYW_INT:
            return "int";
        case TokenType::KEYW_IMPORT:
            return "import";
        case TokenType::KEYW_LAUNCH:
            return "launch";
        case TokenType::KEYW_LONG:
            return "long";
        case TokenType::KEYW_NEVER:
            return "never";
        case TokenType::KEYW_NEW:
            return "new";
        case TokenType::LITERAL_NULL:
            return "null";
        case TokenType::KEYW_NUMBER:
            return "number";
        case TokenType::KEYW_OBJECT:
            return "object";
        case TokenType::KEYW_REGEXP:
            return "regexp";
        case TokenType::KEYW_RETURN:
            return "return";
        case TokenType::KEYW_STRING:
            return "string";
        case TokenType::KEYW_SHORT:
            return "short";
        case TokenType::KEYW_SUPER:
            return "super";
        case TokenType::KEYW_SWITCH:
            return "switch";
        case TokenType::KEYW_TRAP:
            return "trap";
        case TokenType::KEYW_THIS:
            return "this";
        case TokenType::KEYW_THROW:
            return "throw";
        case TokenType::KEYW_TYPE:
            return "type";
        case TokenType::KEYW_TRY:
            return "try";
        case TokenType::KEYW_TYPEOF:
            return "typeof";
        case TokenType::KEYW_UNDEFINED:
            return "undefined";
        case TokenType::KEYW_UNKNOWN:
            return "unknown";
        case TokenType::KEYW_VAR:
            return "var";
        case TokenType::KEYW_VOID:
            return "void";
        case TokenType::KEYW_WHILE:
            return "while";
        case TokenType::KEYW_WITH:
            return "with";
        case TokenType::KEYW_ASYNC:
            return "async";
        case TokenType::KEYW_READONLY:
            return "readonly";
        case TokenType::KEYW_KEYOF:
            return "keyof";
        case TokenType::KEYW_MODULE:
            return "module";
        case TokenType::KEYW_NAMESPACE:
            return "namespace";
        case TokenType::KEYW_INFER:
            return "infer";
        case TokenType::KEYW_DECLARE:
            return "declare";
        case TokenType::KEYW_ARGUMENTS:
            return "arguments";
        case TokenType::KEYW_EVAL:
            return "eval";
        case TokenType::KEYW_STATIC:
            return "static";
        case TokenType::KEYW_GLOBAL:
            return "global";
        case TokenType::KEYW_IS:
            return "is";
        case TokenType::KEYW_ASSERTS:
            return "asserts";
        case TokenType::KEYW_PRIVATE:
            return "private";
        case TokenType::KEYW_PROTECTED:
            return "protected";
        case TokenType::KEYW_PUBLIC:
            return "public";
        case TokenType::KEYW_INTERNAL:
            return "internal";
        case TokenType::KEYW_IMPLEMENTS:
            return "implements";
        case TokenType::KEYW_INTERFACE:
            return "interface";
        case TokenType::KEYW_PACKAGE:
            return "package";
        case TokenType::KEYW_LET:
            return "let";
        case TokenType::KEYW_YIELD:
            return "yield";
        case TokenType::KEYW_DELETE:
            return "delete";
        case TokenType::KEYW_ELSE:
            return "else";
        case TokenType::EOS:
            return "eos";
        case TokenType::KEYW_OUT:
            return "out";
        case TokenType::PUNCTUATOR_DOLLAR_DOLLAR:
            return "$$";
        case TokenType::KEYW_OVERRIDE:
            return "override";
        case TokenType::KEYW_NATIVE:
            return "native";
        case TokenType::LITERAL_IDENT:
            return "identification literal";
        case TokenType::LITERAL_NUMBER:
            return "number literal";
        case TokenType::LITERAL_STRING:
            return "string literal";
        default:
            return "";
    }
}
}  // namespace ark::es2panda::lexer
