/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HIPERF_HASHLIST_H
#define HIPERF_HASHLIST_H

#include <unordered_map>

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class Link {
public:
    Link() = default;
    ~Link() = default;
    Link(const Link &link) : prev_ {link.prev_}, next_ {link.next_} {}
    Link(Link &&link) : prev_ {link.prev_}, next_ {link.next_}
    {
        link.prev_ = nullptr;
        link.next_ = nullptr;
    }
    Link &operator=(const Link &link)
    {
        prev_ = link.prev_;
        next_ = link.next_;
        return *this;
    }
    Link &operator=(Link &&link)
    {
        prev_ = link.prev_;
        link.prev_ = nullptr;
        next_ = link.next_;
        link.next_ = nullptr;
        return *this;
    }
    Link *prev_ {nullptr};
    Link *next_ {nullptr};
};

template<typename Key, typename Val>
class LinkNode {
public:
    Link link_ {};
    Key key_ {};
    Val val_ {};

    explicit LinkNode() = default;
    ~LinkNode() = default;
    explicit LinkNode(const Key &key);
    explicit LinkNode(const Key &key, const Val &val);
    explicit LinkNode(const Key &key, Val &&val);
    LinkNode(const LinkNode &node);
    LinkNode(LinkNode &&node);
    LinkNode &operator=(const LinkNode &node);
    LinkNode &operator=(LinkNode &&node);
    static LinkNode<Key, Val> *GetLinkNode(Val *pval);
    static LinkNode<Key, Val> *GetLinkNode(Link *plink);
};

template<typename Key, typename Val>
class HashList {
public:
    class Iterator {
    public:
        Iterator() = default;
        ~Iterator() = default;
        explicit Iterator(LinkNode<Key, Val> *pnode, HashList *phashList);
        explicit Iterator(const LinkNode<Key, Val> *pnode, const HashList *phashList);
        Iterator(const Iterator &itr);
        Iterator(Iterator &&itr);
        Iterator &operator=(const Iterator &itr);
        Iterator &operator=(Iterator &&itr);
        Iterator &operator++() noexcept;
        Iterator operator++(int) noexcept;
        Iterator &operator--() noexcept;
        Iterator operator--(int) noexcept;
        bool operator<(const Iterator &itr) const noexcept;
        bool operator==(const Iterator &itr) const noexcept;
        Val &operator*();
        const Val &operator*() const;
        Val *operator->();
        const Val *operator->() const;
        void swap(HashList<Key, Val>::Iterator &other);
        LinkNode<Key, Val> *GetNode() const
        {
            return pnode_;
        }

    private:
        bool IsDangled() const noexcept
        {
            return phashList_ == nullptr;
        }

        LinkNode<Key, Val> *pnode_ {nullptr};
        HashList *phashList_ {nullptr};
    };

    class ReverseIterator {
    public:
        ReverseIterator() = default;
        ~ReverseIterator() = default;
        explicit ReverseIterator(LinkNode<Key, Val> *pnode, HashList *phashList);
        explicit ReverseIterator(const LinkNode<Key, Val> *pnode, const HashList *phashList);
        ReverseIterator(const ReverseIterator &itr);
        ReverseIterator(ReverseIterator &&itr);
        ReverseIterator &operator=(const ReverseIterator &itr);
        ReverseIterator &operator=(ReverseIterator &&itr);
        ReverseIterator &operator++() noexcept;
        ReverseIterator operator++(int) noexcept;
        ReverseIterator &operator--() noexcept;
        ReverseIterator operator--(int) noexcept;
        bool operator<(const ReverseIterator &itr) const noexcept;
        bool operator==(const ReverseIterator &itr) const noexcept;
        Val &operator*();
        const Val &operator*() const;
        Val *operator->();
        const Val *operator->() const;
        void swap(HashList<Key, Val>::ReverseIterator &other);

        LinkNode<Key, Val> *GetNode()
        {
            return pnode_;
        }

    private:
        bool IsDangled() const noexcept
        {
            return phashList_ == nullptr;
        }

        LinkNode<Key, Val> *pnode_ {nullptr};
        HashList *phashList_ {nullptr};
    };

public:
    explicit HashList(const std::size_t numItem = 0);
    ~HashList();

    HashList(const HashList &source) = delete;
    HashList &operator=(const HashList &source) = delete;
    HashList(HashList &&source);
    HashList &operator=(HashList &&source);

    // capacity
    inline std::size_t size() const
    {
        return valueTab_.size();
    }
    inline bool empty() const
    {
        return (dataHead_.next_ == &dataHead_) and (dataHead_.prev_ == &dataHead_);
    }
    inline std::size_t capacity() const
    {
        return numItem_;
    }
    inline bool IsFull() const
    {
        return freeHead_.next_ == &freeHead_;
    }
    inline std::size_t count(const Key &key) const
    {
        return valueTab_.count(key);
    }

    int reserve(const std::size_t numItem);
    // iterators
    Iterator begin();
    const Iterator cbegin() const;
    Iterator end();
    const Iterator cend() const;
    ReverseIterator rbegin();
    const ReverseIterator crbegin() const;
    ReverseIterator rend();
    const ReverseIterator crend() const;
    // element access
    Val &front();
    const Val &front() const;
    Val &back(bool prepend = false);
    Val &operator[](const Key &key);
    // lookup
    Iterator find(const Key &key);
    // modifiers
    void push_front(const Key &key, const Val &val);
    void push_front(const Key &key, Val &&val);
    void push_back(const Key &key, const Val &val);
    void push_back(const Key &key, Val &&val);
    void pop_front();
    void pop_back();
    void clear();
    Iterator erase(const Key &key);
    Iterator erase(const Iterator pos);
    Iterator erase(const Iterator first, const Iterator last);

private:
    void MoveToHead(LinkNode<Key, Val> *&pnode);
    void MoveToTail(LinkNode<Key, Val> *&pnode);
    bool MoveNode(const Iterator &pos, LinkNode<Key, Val> *&pnode);
    LinkNode<Key, Val> *AllocateNode(const Key &key);
    LinkNode<Key, Val> *AllocateNode(const Key &key, const Val &val);
    LinkNode<Key, Val> *AllocateNode(const Key &key, Val &&val);
    void ReclaimNode(LinkNode<Key, Val> *&pnode);

    std::size_t numItem_ {0};
    LinkNode<Key, Val> *pData_ {nullptr};
    Link dataHead_ {};
    Link freeHead_ {};
    std::unordered_map<Key, LinkNode<Key, Val> *> valueTab_ {};
};

// implementation of template class LinkNode
template<typename Key, typename Val>
LinkNode<Key, Val>::LinkNode(const Key &key) : key_ {key} {}

template<typename Key, typename Val>
LinkNode<Key, Val>::LinkNode(const Key &key, const Val &val) : key_ {key}, val_ {val} {}

template<typename Key, typename Val>
LinkNode<Key, Val>::LinkNode(const Key &key, Val &&val) : key_ {key}, val_ {std::move(val)} {}

template<typename Key, typename Val>
LinkNode<Key, Val>::LinkNode(const LinkNode& node)
    :link_ {node.link_},
    key_ {node.key_},
    val_ {node.val_}
{}

template<typename Key, typename Val>
LinkNode<Key, Val>::LinkNode(LinkNode&& node)
    :link_ {std::move(node.link_)},
    key_ {std::move(node.key_)},
    val_ {std::move(node.val_)}
{}

template<typename Key, typename Val>
auto LinkNode<Key, Val>::operator=(const LinkNode& node)
-> LinkNode<Key, Val>&
{
    link_ = node.link_;
    key_ = node.key_;
    val_ = node.val_;
}

template<typename Key, typename Val>
auto LinkNode<Key, Val>::operator=(LinkNode&& node)
-> LinkNode<Key, Val>&
{
    link_ = std::move(node.link_);
    key_ = std::move(node.key_);
    val_ = std::move(node.val_);
}

template<typename Key, typename Val>
auto LinkNode<Key, Val>::GetLinkNode(Val *pval)
-> LinkNode<Key, Val>*
{
    if (pval) {
        LinkNode<Key, Val> *pnode {nullptr};
        Val* offset = &pnode->val_;
        auto nodeAddr = reinterpret_cast<char*>(pval) - reinterpret_cast<char*>(offset);
        return reinterpret_cast<LinkNode<Key, Val>*>(nodeAddr);
    }
    return nullptr;
}

template<typename Key, typename Val>
auto LinkNode<Key, Val>::GetLinkNode(Link *plink)
-> LinkNode<Key, Val>*
{
    if (plink) {
        LinkNode<Key, Val> *pnode {nullptr};
        Link* offset = &pnode->link_;
        auto  nodeAddr = reinterpret_cast<char*>(plink) - reinterpret_cast<char*>(offset);
        return reinterpret_cast<LinkNode<Key, Val>*>(nodeAddr);
    }
    return nullptr;
}
// end of LinkNode

// implementation of template class Iterator
template<typename Key, typename Val>
HashList<Key, Val>::Iterator::Iterator(LinkNode<Key, Val> *pnode, HashList *phashList)
    : pnode_ {pnode}, phashList_ {phashList}
{
    if (phashList_ == nullptr) {
        pnode_ = nullptr;
    }
}

template<typename Key, typename Val>
HashList<Key, Val>::Iterator::Iterator(const LinkNode<Key, Val> *pnode, const HashList *phashList)
    : pnode_ {const_cast<LinkNode<Key, Val>*>(pnode)},
      phashList_ {const_cast<HashList*>(phashList)}
{
    if (phashList_ == nullptr) {
        pnode_ = nullptr;
    }
}

template<typename Key, typename Val>
HashList<Key, Val>::Iterator::Iterator(const Iterator& itr)
    : pnode_ {itr.pnode_}, phashList_ {itr.phashList_}
{}

template<typename Key, typename Val>
HashList<Key, Val>::Iterator::Iterator(Iterator&& itr)
    : pnode_ {itr.pnode_}, phashList_ {itr.phashList_}
{
    itr.pnode_ = nullptr;
    itr.phashList_ = nullptr;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator=(const Iterator& itr)
-> HashList<Key, Val>::Iterator&
{
    Iterator temp {itr};
    swap(temp);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator=(Iterator&& itr)
-> HashList<Key, Val>::Iterator&
{
    Iterator temp {std::move(itr)};
    swap(temp);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator++() noexcept
-> HashList<Key, Val>::Iterator &
{
    if (pnode_ == nullptr or phashList_ == nullptr) {
        phashList_ = nullptr;
        return *this;
    }
    Link* plink = pnode_->link_.next_;
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        return *this;
    }
    auto pnode = LinkNode<Key, Val>::GetLinkNode(plink);
    pnode_ = pnode;
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator++(int) noexcept
-> HashList<Key, Val>::Iterator
{
    Iterator res {*this};
    if (pnode_ == nullptr or phashList_ == nullptr) {
        phashList_ = nullptr;
        return res;
    }
    Link* plink = pnode_->link_.next_;
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        return res;
    }
    auto pnode = LinkNode<Key, Val>::GetLinkNode(plink);
    pnode_ = pnode;
    return res;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator--() noexcept
-> HashList<Key, Val>::Iterator &
{
    if (phashList_ == nullptr) {
        return *this;
    }
    Link* plink {nullptr};
    if (pnode_ == nullptr) {
        plink = phashList_->dataHead_.prev_;
    } else {
        plink = pnode_->link_.prev_;
    }
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        phashList_ = nullptr;
        return *this;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::Iterator::operator--(int) noexcept
-> HashList<Key, Val>::Iterator
{
    Iterator res {*this};
    if (phashList_ == nullptr) {
        return res;
    }
    Link* plink {nullptr};
    if (pnode_ == nullptr) {
        plink = phashList_->dataHead_.prev_;
    } else {
        plink = pnode_->link_.prev_;
    }
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        phashList_ = nullptr;
        return res;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return res;
}

template<typename Key, typename Val>
bool HashList<Key, Val>::Iterator::operator<(const HashList<Key, Val>::Iterator &itr) const noexcept
{
    if (IsDangled() or itr.IsDangled()) {
        return false;
    }
    if (phashList_ != itr.phashList_) {
        return false;
    }
    Iterator tempItr {*this};
    if (tempItr == itr) {
        return false;
    }
    while (!tempItr.IsDangled()) {
        tempItr++;
        if (tempItr == itr) {
            return true;
        }
    }
    return false;
}

template<typename Key, typename Val>
bool HashList<Key, Val>::Iterator::operator==(const HashList<Key, Val>::Iterator &itr) const noexcept
{
    if (IsDangled() or itr.IsDangled()) {
        return false;
    }
    if (phashList_ != itr.phashList_) {
        return false;
    }
    return pnode_ == itr.pnode_;
}

template<typename Key, typename Val>
Val& HashList<Key, Val>::Iterator::operator*()
{
    return pnode_->val_;
}

template<typename Key, typename Val>
const Val& HashList<Key, Val>::Iterator::operator*() const
{
    return pnode_->val_;
}

template<typename Key, typename Val>
Val* HashList<Key, Val>::Iterator::operator->()
{
    return &pnode_->val_;
}

template<typename Key, typename Val>
const Val* HashList<Key, Val>::Iterator::operator->() const
{
    return &pnode_->val_;
}

template<typename Key, typename Val>
void HashList<Key, Val>::Iterator::swap(HashList<Key, Val>::Iterator& other)
{
    using std::swap;
    swap(pnode_, other.pnode_);
    swap(phashList_, other.phashList_);
}
// end of Iterator

// Implementation of ReverseIterator
template<typename Key, typename Val>
HashList<Key, Val>::ReverseIterator::ReverseIterator(LinkNode<Key, Val> *pnode, HashList *phashList)
    : pnode_ {pnode}, phashList_ {phashList}
{
    if (phashList_ == nullptr) {
        pnode_ = nullptr;
    }
}

template<typename Key, typename Val>
HashList<Key, Val>::ReverseIterator::ReverseIterator(const LinkNode<Key, Val> *pnode, const HashList *phashList)
    : pnode_ {const_cast<LinkNode<Key, Val> *>(pnode)},
    phashList_ {const_cast<HashList *>(phashList)}
{
    if (phashList_ == nullptr) {
        pnode_ = nullptr;
    }
}

template<typename Key, typename Val>
HashList<Key, Val>::ReverseIterator::ReverseIterator(const ReverseIterator &itr)
    : pnode_ {itr.pnode_}, phashList_ {itr.phashList_}
{}

template<typename Key, typename Val>
HashList<Key, Val>::ReverseIterator::ReverseIterator(ReverseIterator &&itr)
    : pnode_ {itr.pnode_}, phashList_ {itr.phashList_}
{
    itr.pnode_ = nullptr;
    itr.phashList_ = nullptr;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator=(const ReverseIterator& itr)
-> HashList<Key, Val>::ReverseIterator&
{
    ReverseIterator temp {itr};
    swap(temp);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator=(ReverseIterator&& itr)
-> HashList<Key, Val>::ReverseIterator&
{
    ReverseIterator temp {std::move(itr)};
    swap(temp);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator++() noexcept
-> HashList<Key, Val>::ReverseIterator &
{
    if (pnode_ == nullptr or phashList_ == nullptr) {
        phashList_ = nullptr;
        return *this;
    }
    Link* plink = &pnode_->link_;
    plink = plink->prev_;
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        return *this;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator++(int) noexcept
-> HashList<Key, Val>::ReverseIterator
{
    ReverseIterator res {*this};
    if (pnode_ == nullptr or phashList_ == nullptr) {
        phashList_ = nullptr;
        return res;
    }
    Link* plink = &pnode_->link_;
    plink = plink->prev_;
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        return res;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return res;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator--() noexcept
-> HashList<Key, Val>::ReverseIterator &
{
    if (phashList_ == nullptr) {
        return *this;
    }
    Link* plink {nullptr};
    if (pnode_ == nullptr) {
        plink = phashList_->dataHead_.next_;
    } else {
        plink = pnode_->link_.next_;
    }
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        phashList_ = nullptr;
        return *this;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::ReverseIterator::operator--(int) noexcept
-> HashList<Key, Val>::ReverseIterator
{
    ReverseIterator res {*this};
    if (phashList_ == nullptr) {
        return res;
    }
    Link* plink {nullptr};
    if (pnode_ == nullptr) {
        plink = phashList_->dataHead_.next_;
    } else {
        plink = pnode_->link_.next_;
    }
    if (plink == &phashList_->dataHead_) {
        pnode_ = nullptr;
        phashList_ = nullptr;
        return res;
    }
    pnode_ = LinkNode<Key, Val>::GetLinkNode(plink);
    return res;
}

template<typename Key, typename Val>
bool HashList<Key, Val>::ReverseIterator::operator<(
    const HashList<Key, Val>::ReverseIterator &itr) const noexcept
{
    if (IsDangled() or itr.IsDangled()) {
        return false;
    }
    if (phashList_ != itr.phashList_) {
        return false;
    }
    HashList<Key, Val>::ReverseIterator tempItr {*this};
    if (tempItr == itr) {
        return false;
    }
    while (!tempItr.IsDangled()) {
        tempItr++;
        if (tempItr == itr) {
            return true;
        }
    }
    return false;
}

template<typename Key, typename Val>
bool HashList<Key, Val>::ReverseIterator::operator==(
    const HashList<Key, Val>::ReverseIterator &itr) const noexcept
{
    if (IsDangled() or itr.IsDangled()) {
        return false;
    }
    if (phashList_ != itr.phashList_) {
        return false;
    }
    return pnode_ == itr.pnode_;
}

template<typename Key, typename Val>
Val& HashList<Key, Val>::ReverseIterator::operator*()
{
    return pnode_->val_;
}

template<typename Key, typename Val>
const Val& HashList<Key, Val>::ReverseIterator::operator*() const
{
    return pnode_->val_;
}

template<typename Key, typename Val>
Val* HashList<Key, Val>::ReverseIterator::operator->()
{
    return &pnode_->val_;
}

template<typename Key, typename Val>
const Val* HashList<Key, Val>::ReverseIterator::operator->() const
{
    return &pnode_->val_;
}

template<typename Key, typename Val>
void HashList<Key, Val>::ReverseIterator::swap(HashList<Key, Val>::ReverseIterator& other)
{
    using std::swap;
    swap(pnode_, other.pnode_);
    swap(phashList_, other.phashList_);
}
// end of ReverseIterator

// implementation of template class HashList
template<typename Key, typename Val>
HashList<Key, Val>::HashList(const std::size_t numItem) : numItem_ {numItem}
{
    dataHead_.next_ = &dataHead_;
    dataHead_.prev_ = &dataHead_;
    if (numItem_) {
        valueTab_.reserve(numItem_);
        pData_ = new(std::nothrow) LinkNode<Key, Val>[numItem_];
        if (pData_) {
            freeHead_.next_ = &(pData_[0].link_);
            std::size_t last {numItem_ - 1};
            for (std::size_t index = 0; index < last;) {
                LinkNode<Key, Val> &curNnode = pData_[index];
                curNnode.link_.next_ = &(pData_[++index].link_);
            }
            pData_[last].link_.next_ = &freeHead_;
        } else {
            numItem_ = 0;
            freeHead_.next_ = &freeHead_;
            freeHead_.prev_ = &freeHead_;
        }
    }
}

template<typename Key, typename Val>
int HashList<Key, Val>::reserve(const std::size_t numItem)
{
    if (numItem_ != 0) {
        return -1;
    }
    if (numItem) {
        numItem_ = numItem;
        valueTab_.reserve(numItem_);
        pData_ = new(std::nothrow) LinkNode<Key, Val>[numItem_];
        dataHead_.next_ = &dataHead_;
        dataHead_.prev_ = &dataHead_;
        if (pData_) {
            freeHead_.next_ = &(pData_[0].link_);
            std::size_t last {numItem_ - 1};
            for (std::size_t index = 0; index < last;) {
                LinkNode<Key, Val> &curNnode = pData_[index];
                curNnode.link_.next_ = &(pData_[++index].link_);
            }
            pData_[last].link_.next_ = &freeHead_;
        } else {
            numItem_ = 0;
            freeHead_.next_ = &freeHead_;
            freeHead_.prev_ = &freeHead_;
        }
    }
    return numItem_;
}

template<typename Key, typename Val>
HashList<Key, Val>::~HashList()
{
    if (pData_) {
        delete[] pData_;
        pData_ = nullptr;
    }
    valueTab_.clear();
    dataHead_.next_ = &dataHead_;
    dataHead_.prev_ = &dataHead_;
    freeHead_.next_ = nullptr;
    freeHead_.prev_ = nullptr;
    numItem_ = 0;
}

template<typename Key, typename Val>
HashList<Key, Val>::HashList(HashList<Key, Val> &&source)
    : numItem_ {source.numItem_},
    pData_ {source.pData_},
    dataHead_ {std::move(source.dataHead_)},
    freeHead_ {std::move(source.freeHead_)},
    valueTab_ {std::move(source.valueTab_)}
{
    source.pData_ = nullptr;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::operator=(HashList &&source)
-> HashList<Key, Val>&
{
    if (this == &source) {
        return *this;
    }
    if (pData_) {
        delete[] pData_;
        pData_ = nullptr;
    }
    numItem_ = source.numItem_;
    pData_ = source.pData_;
    source.pData_ = nullptr;
    dataHead_ = std::move(source.dataHead_);
    freeHead_ = std::move(source.freeHead_);
    valueTab_ = std::move(source.valueTab_);
    return *this;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::begin()
-> HashList<Key, Val>::Iterator
{
    if (empty()) {
        return end();
    }
    return Iterator(LinkNode<Key, Val>::GetLinkNode(dataHead_.next_), this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::cbegin() const
-> const HashList<Key, Val>::Iterator
{
    if (empty()) {
        return cend();
    }
    return Iterator(LinkNode<Key, Val>::GetLinkNode(dataHead_.next_), this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::end()
-> HashList<Key, Val>::Iterator
{
    return Iterator(nullptr, this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::cend() const
-> const HashList<Key, Val>::Iterator
{
    return Iterator(nullptr, this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::rbegin()
-> HashList<Key, Val>::ReverseIterator
{
    if (empty()) {
        return rend();
    }
    return ReverseIterator(LinkNode<Key, Val>::GetLinkNode(dataHead_.prev_), this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::crbegin() const
-> const HashList<Key, Val>::ReverseIterator
{
    if (empty()) {
        return crend();
    }
    return ReverseIterator(LinkNode<Key, Val>::GetLinkNode(dataHead_.prev_), this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::rend()
-> HashList<Key, Val>::ReverseIterator
{
    return ReverseIterator(nullptr, this);
}

template<typename Key, typename Val>
auto HashList<Key, Val>::crend() const
-> const HashList<Key, Val>::ReverseIterator
{
    return ReverseIterator(nullptr, this);
}

template<typename Key, typename Val>
Val& HashList<Key, Val>::front()
{
    LinkNode<Key, Val> *pnode = LinkNode<Key, Val>::GetLinkNode(dataHead_.next_);
    return pnode->val_;
}

template<typename Key, typename Val>
const Val& HashList<Key, Val>::front() const
{
    return front();
}

template<typename Key, typename Val>
Val& HashList<Key, Val>::back(bool prepend)
{
    auto pnode = LinkNode<Key, Val>::GetLinkNode(dataHead_.prev_);
    if (prepend) {
        MoveToHead(pnode);
    }
    return pnode->val_;
}

template<typename Key, typename Val>
Val& HashList<Key, Val>::operator[](const Key &key)
{
    LinkNode<Key, Val> *pnode {nullptr};
    if (valueTab_.find(key) == valueTab_.end()) {
        pnode = AllocateNode(key);
        valueTab_[key] = pnode;
    } else {
        pnode = valueTab_[key];
    }
    if (pnode == nullptr) {
        static Val val = Val();
        return val;
    } else {
        MoveToHead(pnode);
    }
    return pnode->val_;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::find(const Key &key)
-> HashList<Key, Val>::Iterator
{
    const auto &itr = valueTab_.find(key);
    if (itr == valueTab_.end()) {
        return end();
    }
    return Iterator(itr->second, this);
}

template<typename Key, typename Val>
void HashList<Key, Val>::push_front(const Key& key, const Val& val)
{
    if (valueTab_.find(key) == valueTab_.end()) {
        LinkNode<Key, Val>* pnode = AllocateNode(key, val);
        MoveToHead(pnode);
        valueTab_[pnode->key_] = pnode;
    } else {
        MoveToHead(valueTab_[key]);
        this->operator[](key) =  val;
    }
}

template<typename Key, typename Val>
void HashList<Key, Val>::push_front(const Key& key, Val&& val)
{
    if (valueTab_.find(key) == valueTab_.end()) {
        LinkNode<Key, Val>* pnode = AllocateNode(key, std::move(val));
        MoveToHead(pnode);
        valueTab_[pnode->key_] = pnode;
    } else {
        MoveToHead(valueTab_[key]);
        this->operator[](key) =  val;
    }
}

template<typename Key, typename Val>
void HashList<Key, Val>::push_back(const Key& key, const Val& val)
{
    if (valueTab_.find(key) == valueTab_.end()) {
        LinkNode<Key, Val>* pnode = AllocateNode(key, val);
        MoveToTail(pnode);
        valueTab_[pnode->key_] = pnode;
    } else {
        MoveToTail(valueTab_[key]);
        this->operator[](key) =  val;
    }
}

template<typename Key, typename Val>
void HashList<Key, Val>::push_back(const Key& key, Val&& val)
{
    if (valueTab_.find(key) == valueTab_.end()) {
        LinkNode<Key, Val>* pnode = AllocateNode(key, std::move(val));
        MoveToTail(pnode);
        valueTab_[pnode->key_] = pnode;
    } else {
        MoveToTail(valueTab_[key]);
        this->operator[](key) =  val;
    }
}

template<typename Key, typename Val>
void HashList<Key, Val>::pop_front()
{
    if (empty()) {
        return;
    }
    LinkNode<Key, Val>* pnode = LinkNode<Key, Val>::GetLinkNode(dataHead_.next_);
    valueTab_.erase(pnode->key_);
    ReclaimNode(pnode);
}

template<typename Key, typename Val>
void HashList<Key, Val>::pop_back()
{
    if (empty()) {
        return;
    }
    LinkNode<Key, Val>* pnode = LinkNode<Key, Val>::GetLinkNode(dataHead_.prev_);
    if (pnode != nullptr) {
        valueTab_.erase(pnode->key_);
        ReclaimNode(pnode);
    }
}

template<typename Key, typename Val>
void HashList<Key, Val>::clear()
{
    Iterator curPos = begin();
    LinkNode<Key, Val> *curNode = curPos.GetNode();
    while (curNode != nullptr) {
        curPos = erase(curPos);
        curNode = curPos.GetNode();
    }
}

template<typename Key, typename Val>
auto HashList<Key, Val>::erase(const Key& key)
-> HashList<Key, Val>::Iterator
{
    if (valueTab_.find(key) == valueTab_.end()) {
        return end();
    }
    LinkNode<Key, Val> *pnode = valueTab_[key];
    valueTab_.erase(key);
    Link* plink = pnode->link_.next_;
    Iterator tempItr {LinkNode<Key, Val>::GetLinkNode(plink), this};
    ReclaimNode(pnode);
    return tempItr;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::erase(const Iterator pos)
-> HashList<Key, Val>::Iterator
{
    // assume pos is valid, otherwise the result is undefined
    Iterator tempItr {pos};
    ++tempItr;
    LinkNode<Key, Val> *pnode = pos.GetNode();
    valueTab_.erase(pnode->key_);
    ReclaimNode(pnode);
    return tempItr;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::erase(const Iterator first, const Iterator last)
-> HashList<Key, Val>::Iterator
{
    // assume pos is valid, otherwise the result is undefined
    if (first <= last) {
        Iterator curPos {first};
        while (curPos < last) {
            curPos = erase(curPos);
        }
        return last;
    }
    return end();
}

template<typename Key, typename Val>
bool HashList<Key, Val>::MoveNode(const Iterator& pos, LinkNode<Key, Val> *&pnode)
{
    LinkNode<Key, Val> *curNode = pos.GetNode();
    if (curNode == pnode) {
        return true;
    }
    if (pnode->link_.next_ == &curNode->link_) {
        return true;
    }
    Link* prevLink = pnode->link_.prev_;
    Link* nextLink = pnode->link_.next_;
    if (prevLink and nextLink) {
        prevLink->next_ = nextLink;
        nextLink->prev_ = prevLink;
    }
    Link *currLink = &curNode->link_;
    prevLink = currLink->prev_;
    nextLink = &pnode->link_;
    prevLink->next_ = nextLink;
    nextLink->prev_ = prevLink;
    nextLink->next_ = currLink;
    currLink->prev_ = nextLink;
    return true;
}

template<typename Key, typename Val>
void HashList<Key, Val>::MoveToHead(LinkNode<Key, Val> *&pnode)
{
    if (pnode->link_.prev_ and pnode->link_.next_) {
        Link* prev = pnode->link_.prev_;
        Link* next = pnode->link_.next_;
        prev->next_ = next;
        next->prev_ = prev;
    }
    pnode->link_.next_ = dataHead_.next_;
    dataHead_.next_->prev_ = &pnode->link_;
    dataHead_.next_ = &pnode->link_;
    pnode->link_.prev_ = &dataHead_;
}

template<typename Key, typename Val>
void HashList<Key, Val>::MoveToTail(LinkNode<Key, Val> *&pnode)
{
    if (pnode->link_.prev_ and pnode->link_.next_) {
        Link* prev = pnode->link_.prev_;
        Link* next = pnode->link_.next_;
        prev->next_ = next;
        next->prev_ = prev;
    }
    pnode->link_.prev_ = dataHead_.prev_;
    dataHead_.prev_->next_ = &pnode->link_;
    pnode->link_.next_ = &dataHead_;
    dataHead_.prev_ = &pnode->link_;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::AllocateNode(const Key &key)
->LinkNode<Key, Val> *
{
    if (IsFull()) {
        pop_back();
    }
    LinkNode<Key, Val> *pnode = LinkNode<Key, Val>::GetLinkNode(freeHead_.next_);
    freeHead_.next_ = freeHead_.next_->next_;
    if (pnode != nullptr) {
        pnode->link_.next_ = nullptr;
        pnode->link_.prev_ = nullptr;
        pnode->key_ = key;
        pnode->val_ = Val();
    }
    return pnode;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::AllocateNode(const Key &key, const Val &val)
->LinkNode<Key, Val> *
{
    if (IsFull()) {
        pop_back();
    }
    LinkNode<Key, Val> *pnode = LinkNode<Key, Val>::GetLinkNode(freeHead_.next_);
    freeHead_.next_ = freeHead_.next_->next_;
    pnode->link_.next_ = nullptr;
    pnode->link_.prev_ = nullptr;
    pnode->key_ = key;
    pnode->val_ = val;
    return pnode;
}

template<typename Key, typename Val>
auto HashList<Key, Val>::AllocateNode(const Key &key, Val &&val)
->LinkNode<Key, Val> *
{
    if (IsFull()) {
        pop_back();
    }
    LinkNode<Key, Val> *pnode = LinkNode<Key, Val>::GetLinkNode(freeHead_.next_);
    freeHead_.next_ = freeHead_.next_->next_;
    pnode->link_.next_ = nullptr;
    pnode->link_.prev_ = nullptr;
    pnode->key_ = key;
    pnode->val_ = std::move(val);
    return pnode;
}

template<typename Key, typename Val>
void HashList<Key, Val>::ReclaimNode(LinkNode<Key, Val> *&pnode)
{
    Link *prevLink = pnode->link_.prev_;
    Link *nextLink = pnode->link_.next_;
    prevLink->next_ = nextLink;
    nextLink->prev_ = prevLink;
    pnode->link_.prev_ = nullptr;
    pnode->link_.next_ = freeHead_.next_;
    freeHead_.next_ = &pnode->link_;
    return;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_HASHLIST_H
