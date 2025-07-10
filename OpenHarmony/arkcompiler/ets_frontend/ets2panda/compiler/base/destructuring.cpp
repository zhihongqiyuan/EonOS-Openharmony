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

#include "destructuring.h"

#include "util/helpers.h"
#include "compiler/base/iterators.h"
#include "compiler/base/lreference.h"
#include "compiler/base/catchTable.h"
#include "compiler/core/pandagen.h"
#include "ir/base/property.h"
#include "ir/base/spreadElement.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/objectExpression.h"

namespace ark::es2panda::compiler {
static void GenRestElement(PandaGen *pg, const ir::SpreadElement *restElement,
                           const DestructuringIterator &destIterator, bool isDeclaration)
{
    VReg array = pg->AllocReg();
    VReg index = pg->AllocReg();

    auto *next = pg->AllocLabel();
    auto *done = pg->AllocLabel();

    DestructuringRestIterator iterator(destIterator);

    // create left reference for rest element
    auto lref = JSLReference::Create(pg, restElement, isDeclaration);

    // create an empty array first
    pg->CreateEmptyArray(restElement);
    pg->StoreAccumulator(restElement, array);

    // index = 0
    pg->LoadAccumulatorInt(restElement, 0);
    pg->StoreAccumulator(restElement, index);

    pg->SetLabel(restElement, next);

    iterator.Step(done);
    pg->StoreObjByValue(restElement, array, index);

    // index++
    pg->LoadAccumulatorInt(restElement, 1);
    pg->Binary(restElement, lexer::TokenType::PUNCTUATOR_PLUS, index);
    pg->StoreAccumulator(restElement, index);

    pg->Branch(restElement, next);

    pg->SetLabel(restElement, done);
    pg->LoadAccumulator(restElement, array);

    lref.SetValue();
}

static void GenElement(const ir::ArrayExpression *array, DestructuringIterator &iterator, PandaGen *pg)
{
    for (const auto *element : array->Elements()) {
        RegScope ers(pg);

        if (element->IsRestElement()) {
            GenRestElement(pg, element->AsRestElement(), iterator, array->IsDeclaration());
            break;
        }

        // if a hole exist, just let the iterator step ahead
        if (element->IsOmittedExpression()) {
            iterator.Step();
            continue;
        }

        const ir::Expression *init = nullptr;
        const ir::Expression *target = element;

        if (element->IsAssignmentPattern()) {
            target = element->AsAssignmentPattern()->Left();
            init = element->AsAssignmentPattern()->Right();
        }

        auto lref = JSLReference::Create(pg, target, array->IsDeclaration());
        iterator.Step();

        if (init != nullptr) {
            auto *assignValue = pg->AllocLabel();
            auto *defaultInit = pg->AllocLabel();
            pg->BranchIfUndefined(element, defaultInit);
            pg->LoadAccumulator(element, iterator.Result());
            pg->Branch(element, assignValue);

            pg->SetLabel(element, defaultInit);
            init->Compile(pg);
            pg->SetLabel(element, assignValue);
        }

        lref.SetValue();
    }
}

static void GenArray(PandaGen *pg, const ir::ArrayExpression *array)
{
    DestructuringIterator iterator(pg, array);

    if (array->Elements().empty()) {
        iterator.Close(false);
        return;
    }

    TryContext tryCtx(pg);
    const auto &labelSet = tryCtx.LabelSet();
    pg->SetLabel(array, labelSet.TryBegin());

    GenElement(array, iterator, pg);

    pg->SetLabel(array, labelSet.TryEnd());

    // Normal completion
    pg->LoadAccumulator(array, iterator.Done());
    pg->BranchIfTrue(array, labelSet.CatchEnd());
    iterator.Close(false);

    pg->Branch(array, labelSet.CatchEnd());

    Label *end = pg->AllocLabel();
    pg->SetLabel(array, labelSet.CatchBegin());
    pg->StoreAccumulator(array, iterator.Result());
    pg->LoadAccumulator(array, iterator.Done());

    pg->BranchIfTrue(array, end);
    pg->LoadAccumulator(array, iterator.Result());
    iterator.Close(true);
    pg->SetLabel(array, end);
    pg->LoadAccumulator(array, iterator.Result());
    pg->EmitThrow(array);
    pg->SetLabel(array, labelSet.CatchEnd());
}

static std::tuple<const ir::Expression *, const ir::Expression *> GetAssignmentTarget(const ir::Property *propExpr)
{
    const ir::Expression *init = nullptr;
    const ir::Expression *target = propExpr->Value();

    if (target->IsAssignmentPattern()) {
        init = target->AsAssignmentPattern()->Right();
        target = target->AsAssignmentPattern()->Left();
    }

    return {init, target};
}

static void GenDefaultInitializer(PandaGen *pg, const ir::Expression *element, const ir::Expression *init)
{
    if (init == nullptr) {
        return;
    }

    RegScope rs(pg);
    VReg loadedValue = pg->AllocReg();
    pg->StoreAccumulator(element, loadedValue);

    auto *getDefault = pg->AllocLabel();
    auto *store = pg->AllocLabel();

    pg->BranchIfUndefined(element, getDefault);
    pg->LoadAccumulator(element, loadedValue);
    pg->Branch(element, store);

    // load default value
    pg->SetLabel(element, getDefault);
    init->Compile(pg);

    pg->SetLabel(element, store);
}

static void GenObjectWithRest(PandaGen *pg, const ir::ObjectExpression *object, VReg rhs)
{
    const auto &properties = object->Properties();

    RegScope rs(pg);
    VReg propStart = pg->NextReg();

    for (const auto *element : properties) {
        if (element->IsRestElement()) {
            RegScope restScope(pg);
            auto lref = JSLReference::Create(pg, element, object->IsDeclaration());
            pg->CreateObjectWithExcludedKeys(element, rhs, propStart, properties.size() - 1);
            lref.SetValue();
            break;
        }

        VReg propReg = pg->AllocReg();

        RegScope propScope(pg);

        const ir::Property *propExpr = element->AsProperty();
        const ir::Expression *key = propExpr->Key();
        const auto [init, target] = GetAssignmentTarget(propExpr);

        if (key->IsIdentifier()) {
            pg->LoadAccumulatorString(key, key->AsIdentifier()->Name());
        } else {
            key->Compile(pg);
        }

        pg->StoreAccumulator(key, propReg);

        auto lref = JSLReference::Create(pg, target, object->IsDeclaration());

        pg->LoadAccumulator(element, propReg);
        pg->LoadObjByValue(element, rhs);

        GenDefaultInitializer(pg, element, init);

        lref.SetValue();
    }
}

static void GenObject(PandaGen *pg, const ir::ObjectExpression *object, VReg rhs)
{
    const auto &properties = object->Properties();

    if (properties.empty() || properties.back()->IsRestElement()) {
        auto *notNullish = pg->AllocLabel();

        pg->LoadAccumulator(object, rhs);
        pg->BranchIfCoercible(object, notNullish);
        pg->ThrowObjectNonCoercible(object);

        pg->SetLabel(object, notNullish);

        if (!properties.empty()) {
            return GenObjectWithRest(pg, object, rhs);
        }
    }

    for (const auto *element : properties) {
        RegScope propScope(pg);

        const ir::Property *propExpr = element->AsProperty();
        const ir::Expression *key = propExpr->Key();
        const auto [init, target] = GetAssignmentTarget(propExpr);

        Operand propOperand = pg->ToOwnPropertyKey(key, propExpr->IsComputed());

        auto lref = JSLReference::Create(pg, target, object->IsDeclaration());

        if (std::holds_alternative<VReg>(propOperand)) {
            pg->LoadAccumulator(element, std::get<VReg>(propOperand));
            pg->LoadObjByValue(element, rhs);
        } else {
            pg->LoadAccumulator(element, rhs);
            pg->LoadObjProperty(element, propOperand);
        }

        GenDefaultInitializer(pg, element, init);

        lref.SetValue();
    }
}

void Destructuring::Compile(PandaGen *pg, const ir::Expression *pattern)
{
    RegScope rs(pg);

    VReg rhs = pg->AllocReg();
    pg->StoreAccumulator(pattern, rhs);

    if (pattern->IsArrayPattern()) {
        GenArray(pg, pattern->AsArrayPattern());
    } else {
        GenObject(pg, pattern->AsObjectPattern(), rhs);
    }

    pg->LoadAccumulator(pattern, rhs);
}
}  // namespace ark::es2panda::compiler
