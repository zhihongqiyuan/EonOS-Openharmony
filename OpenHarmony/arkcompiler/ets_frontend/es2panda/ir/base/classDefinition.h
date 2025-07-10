/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_CLASS_DEFINITION_H
#define ES2PANDA_PARSER_INCLUDE_AST_CLASS_DEFINITION_H

#include <binder/variable.h>
#include <ir/base/classProperty.h>
#include <ir/base/methodDefinition.h>
#include <ir/expressions/privateIdentifier.h>
#include <ir/ts/tsTypeReference.h>
#include <util/bitset.h>

namespace panda::es2panda::compiler {
class PandaGen;
}  // namespace panda::es2panda::compiler

namespace panda::es2panda::checker {
class Checker;
class Type;
}  // namespace panda::es2panda::checker

namespace panda::es2panda::binder {
class LocalScope;
}  // namespace panda::es2panda::binder

namespace panda::es2panda::ir {

enum class FieldType : uint16_t {
    NONE = 0,
    NUMBER = (1 << 0),
    STRING = (1 << 1),
    BOOLEAN = (1 << 2),
    TS_TYPE_REF = (1 << 3),
    BIGINT = (1 << 4),
    GENERIC = (1 << 5), // import type / type parameter
    TS_NULL = (1 << 6),
    TS_UNDEFINED = (1 << 7),
};
DEFINE_BITOPS(FieldType)

class Identifier;
class MethodDefinition;
class TSTypeParameterDeclaration;
class TSTypeParameterInstantiation;
class TSClassImplements;
class TSIndexSignature;

class ClassDefinition : public AstNode {
public:
    explicit ClassDefinition(binder::ClassScope *scope, Identifier *ident, TSTypeParameterDeclaration *typeParams,
                             TSTypeParameterInstantiation *superTypeParams,
                             ArenaVector<TSClassImplements *> &&implements, MethodDefinition *ctor,
                             MethodDefinition *staticInitializer, MethodDefinition *instanceInitializer,
                             Expression *superClass, ArenaVector<Statement *> &&body,
                             ArenaVector<TSIndexSignature *> &&indexSignatures, bool declare, bool abstract)
        : AstNode(AstNodeType::CLASS_DEFINITION),
          scope_(scope),
          ident_(ident),
          typeParams_(typeParams),
          superTypeParams_(superTypeParams),
          implements_(std::move(implements)),
          ctor_(ctor),
          staticInitializer_(staticInitializer),
          instanceInitializer_(instanceInitializer),
          superClass_(superClass),
          body_(std::move(body)),
          indexSignatures_(std::move(indexSignatures)),
          declare_(declare),
          abstract_(abstract),
          exportDefault_(false),
          isClassDecoratorPresent_(false)
    {
    }

    binder::ClassScope *Scope() const
    {
        return scope_;
    }

    const Identifier *Ident() const
    {
        return ident_;
    }

    Identifier *Ident()
    {
        return ident_;
    }

    Expression *Super()
    {
        return superClass_;
    }

    const Expression *Super() const
    {
        return superClass_;
    }

    bool Declare() const
    {
        return declare_;
    }

    bool Abstract() const
    {
        return abstract_;
    }

    void SetAsExportDefault()
    {
        exportDefault_ = true;
    }

    ArenaVector<Statement *> &Body()
    {
        return body_;
    }

    const ArenaVector<Statement *> &Body() const
    {
        return body_;
    }

    void AddToBody(Statement *statement)
    {
        body_.push_back(statement);
    }

    TSTypeParameterDeclaration *TypeParams()
    {
        return typeParams_;
    }

    const TSTypeParameterDeclaration *TypeParams() const
    {
        return typeParams_;
    }

    ArenaVector<TSClassImplements *> &Implements()
    {
        return implements_;
    }

    const ArenaVector<TSClassImplements *> &Implements() const
    {
        return implements_;
    }

    ArenaVector<TSIndexSignature *> &IndexSignatures()
    {
        return indexSignatures_;
    }

    const ArenaVector<TSIndexSignature *> &IndexSignatures() const
    {
        return indexSignatures_;
    }

    MethodDefinition *Ctor()
    {
        ASSERT(ctor_ != nullptr);
        return ctor_;
    }

    MethodDefinition *StaticInitializer() const
    {
        return staticInitializer_;
    }

    MethodDefinition *InstanceInitializer() const
    {
        return instanceInitializer_;
    }

    const TSTypeParameterInstantiation *SuperTypeParams() const
    {
        return superTypeParams_;
    }

    TSTypeParameterInstantiation *SuperTypeParams()
    {
        return superTypeParams_;
    }

    bool NeedStaticInitializer() const
    {
        return needStaticInitializer_;
    }

    bool NeedInstanceInitializer() const
    {
        return needInstanceInitializer_;
    }

    uint32_t GetSlot(const Expression *key) const
    {
        return scope_->GetSlot(key);
    }

    bool HasInstancePrivateMethod() const
    {
        return scope_->instanceMethodValidation_ != 0;
    }

    bool HasStaticPrivateMethod() const
    {
        return scope_->staticMethodValidation_ != 0;
    }

    void SetSendable()
    {
        isSendable_ = true;
    }

    bool IsSendable() const
    {
        return isSendable_;
    }

    void SetClassDecoratorPresent()
    {
        isClassDecoratorPresent_ = true;
    }

    bool IsClassDecoratorPresent() const
    {
        return isClassDecoratorPresent_;
    }

    void IncreasePropertyCount()
    {
        ++classExpectedPropertyCount_;
    }

    size_t ExpectedPropertyCount() const
    {
        return classExpectedPropertyCount_;
    }

    void CalculateClassExpectedPropertyCount();
    void ProcessClassProperty(const ClassProperty *prop,
                              const std::function<void(const util::StringView&)>& addPropertyName);
    void ProcessConstructorBody(const BlockStatement *body,
                                const std::function<void(const util::StringView&)>& addPropertyName);
    void ProcessPropertyKey(const Expression* key,
                            const std::function<void(const util::StringView&)>& addPropertyName);

    const FunctionExpression *Ctor() const;

    util::StringView GetName() const;

    void BuildClassEnvironment(bool useDefineSemantic);

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    checker::Type *Check(checker::Checker *checker) const override;
    void UpdateSelf(const NodeUpdater &cb, binder::Binder *binder) override;
    const ir::AstNode *GetDeclNodeFromIdentifier(const ir::Identifier *identifier) const;

private:
    compiler::VReg CompileHeritageClause(compiler::PandaGen *pg) const;
    void InitializeClassName(compiler::PandaGen *pg) const;
    int32_t CreateClassPublicBuffer(compiler::PandaGen *pg, util::BitSet &compiled, int32_t fieldTypeBufIdx = 0) const;
    int32_t CreateClassPrivateBuffer(compiler::PandaGen *pg) const;
    void CompileMissingProperties(compiler::PandaGen *pg, const util::BitSet &compiled, compiler::VReg classReg) const;
    void StaticInitialize(compiler::PandaGen *pg, compiler::VReg classReg) const;
    void InstanceInitialize(compiler::PandaGen *pg, compiler::VReg classReg) const;
    void CompileComputedKeys(compiler::PandaGen *pg) const;
    void AddFieldType(FieldType &fieldType, const Expression *typeAnnotation, compiler::PandaGen *pg) const;
    void AddFieldTypeForTypeReference(const TSTypeReference *typeReference, FieldType &fieldType,
                                      compiler::PandaGen *pg) const;
    bool IsTypeParam(const util::StringView &propertyName) const;
    int32_t CreateFieldTypeBuffer(compiler::PandaGen *pg) const;
    void CompileSendableClass(compiler::PandaGen *pg) const;
    void CompileGetterOrSetter(compiler::PandaGen *pg, compiler::VReg dest, const MethodDefinition *prop) const;

    binder::ClassScope *scope_;
    Identifier *ident_;
    TSTypeParameterDeclaration *typeParams_;
    TSTypeParameterInstantiation *superTypeParams_;
    ArenaVector<TSClassImplements *> implements_;
    MethodDefinition *ctor_;
    MethodDefinition *staticInitializer_;
    MethodDefinition *instanceInitializer_;
    Expression *superClass_;
    ArenaVector<Statement *> body_;
    ArenaVector<TSIndexSignature *> indexSignatures_;
    bool declare_;
    bool abstract_;
    bool exportDefault_;
    bool needStaticInitializer_ {false};
    bool needInstanceInitializer_ {false};
    bool hasComputedKey_ {false};
    bool hasPrivateElement_ {false};
    bool isSendable_ {false};
    bool isClassDecoratorPresent_ {false};
    size_t classExpectedPropertyCount_ {0};
};

}  // namespace panda::es2panda::ir

#endif
