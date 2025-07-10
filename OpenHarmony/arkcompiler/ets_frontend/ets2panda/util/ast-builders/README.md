# DSL for build AST tree
 
## Usage

All nodes of AST tree builder is header only.

For use DSL for AST tree builder:

1. Include header with needed node
2. Create builder:
    
    1. All builder methods, except method `Build`, return `ASTBuilder` class.
    2. Builder constructor have `Allocator` argument, that must be sended.
    3. For set arguments for construct node we have methods that starts with `Set` or `Add`. For all necessary fields for constructor we have methods for set them.
    4. All builders have method `SetParent`, it can be used for set parent node.
    5. To get node from builder you must to use method `Build`

    Example:
    ```cpp
    // Create NumberLiteralBuildr, set value and build it. Variable `left` is NumberLiteral after call `Build` method.
    auto left = NumberLiteralBuilder(Allocator()).SetValue("10").Build();
    auto right = NumberLiteralBuilder(Allocator()).SetValue("5").Build();
    // Create BinaryExpressionBuilder, set left operand, set right operand, set operator and build BinaryExpression node.
    auto binaryExpr = BinaryExpressionBuilder(Allocator())
                          .SetLeft(left)
                          .SetRight(right)
                          .SetOperator(ark::es2panda::lexer::TokenType::PUNCTUATOR_PLUS)
                          .Build();
    ```

    *Note: More examples can be found in `ets_frontend/ets2panda/test/unit/public/ast_builder_test.cpp`*

## List of implemented nodes

<details>
    <summary>Implemented nodes</summary>

        * AwaitExpression
        * BigintLiteral
        * BinaryExpression
        * BlockExpression
        * BlockStatement
        * BooleanLiteral
        * BreakStatement
        * CallExpression
        * CharLiteral
        * ClassDeclaration
        * ClassDefenition
        * ClassPropery
        * ETSTypeReference
        * ETSTypeReferencePart
        * ExportDefaultDeclaration
        * ExpressionStatement
        * FunctionExpression
        * Identifier
        * IfStatement
        * MemberExpression
        * MethodDefinition
        * NullLiteral
        * NumberLiteral
        * OpaqueTypeNode
        * ScriptFunction
        * Statement
        * StringLiteral
        * SuperExpression
        * SwitchCaseStatement
        * SwitchStatement
        * ThisExpression
        * TSClassImplements
        * TSEnumDeclaration
        * TSEnumMember
        * TSTypeParameterInstantiation
        * UnaryExpression
        * UndefinedLiteral
        * UpdateExpression
        * VariableDeclaration
        * VariableDeclarator
        * WhileStatement
        * YieldExpression

</details>

*Note: list of implemented nodes actual by 2024.07.03*