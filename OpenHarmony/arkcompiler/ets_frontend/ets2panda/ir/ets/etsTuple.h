/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_ETS_TUPLE_H
#define ES2PANDA_IR_ETS_TUPLE_H

#include "checker/ETSchecker.h"
#include "ir/typeNode.h"

namespace ark::es2panda::ir {

class ETSTuple : public TypeNode {
public:
    using TupleSizeType = uint32_t;

    explicit ETSTuple(ArenaAllocator *const allocator)
        : TypeNode(AstNodeType::ETS_TUPLE), typeAnnotationList_(allocator->Adapter())
    {
    }

    explicit ETSTuple(ArenaAllocator *const allocator, const TupleSizeType size)
        : TypeNode(AstNodeType::ETS_TUPLE), typeAnnotationList_(allocator->Adapter()), size_(size)
    {
    }
    explicit ETSTuple(const ArenaVector<ir::TypeNode *> &typeList)
        : TypeNode(AstNodeType::ETS_TUPLE),
          typeAnnotationList_(typeList),
          size_(static_cast<TupleSizeType>(typeList.size()))
    {
    }

    [[nodiscard]] TupleSizeType GetTupleSize() const
    {
        return size_;
    }

    [[nodiscard]] ArenaVector<ir::TypeNode *> GetTupleTypeAnnotationsList() const
    {
        return typeAnnotationList_;
    }

    [[nodiscard]] bool HasSpreadType() const
    {
        return spreadType_ != nullptr;
    }

    void SetSpreadType(TypeNode *const newSpreadType)
    {
        spreadType_ = newSpreadType;
    }

    void SetTypeAnnotationsList(ArenaVector<TypeNode *> &&typeNodeList)
    {
        typeAnnotationList_ = std::move(typeNodeList);
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile([[maybe_unused]] compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

    // NOTE(vpukhov): hide in TypeCreation
    static checker::Type *CalculateLUBForTuple(checker::ETSChecker *checker, ArenaVector<checker::Type *> &typeList,
                                               checker::Type **spreadTypePtr);

    [[nodiscard]] ETSTuple *Clone(ArenaAllocator *allocator, AstNode *parent) override;

private:
    ArenaVector<TypeNode *> typeAnnotationList_;
    TypeNode *spreadType_ {};
    TupleSizeType size_ {0};
};

}  // namespace ark::es2panda::ir

#endif /* ES2PANDA_IR_ETS_TUPLE_H */
