/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.entity;

/**
 * Pair of two elements
 *
 * @since 2021-12-13
 */
public final class Pair<A, B> {
    private final A mFirst;

    private final B mSecond;

    private Pair(A first, B second) {
        mFirst = first;
        mSecond = second;
    }

    /**
     * create a pair with key of type A and value of type B
     *
     * @param first key of pair
     * @param second value of pair
     * @param <A> type of key
     * @param <B> type of value
     * @return a pair with key of type A and value of type B
     */
    public static <A, B> Pair<A, B> create(A first, B second) {
        return new Pair<A, B>(first, second);
    }

    /**
     * get key of pair
     *
     * @return key of pair
     */
    public A getFirst() {
        return mFirst;
    }

    /**
     * get value of pair
     *
     * @return value of pair
     */
    public B getSecond() {
        return mSecond;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((mFirst == null) ? 0 : mFirst.hashCode());
        result = prime * result + ((mSecond == null) ? 0 : mSecond.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if ((obj == null) || (getClass() != obj.getClass()) || (!(obj instanceof Pair))) {
            return false;
        }
        Pair other = (Pair) obj;
        return compare(mFirst, other.mFirst) && compare(mSecond, other.mSecond);
    }

    private <C> boolean compare(C value1, C value2) {
        if (value1 == null) {
            return value2 == null;
        }
        return value1.equals(value2);
    }
}
