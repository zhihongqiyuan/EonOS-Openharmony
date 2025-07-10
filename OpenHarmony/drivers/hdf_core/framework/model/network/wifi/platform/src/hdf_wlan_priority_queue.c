/*
 * Copyright (c) 2020-2022 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */
#include "hdf_wlan_priority_queue.h"
#include "hdf_core_log.h"
#include "hdf_wlan_queue.h"
#include "securec.h"
#include "osal_sem.h"
#include "osal_mem.h"

#define MAX_PRIORITY_LEVEL 8

#define HDF_LOG_TAG HDF_WIFI_CORE

typedef struct {
    PriorityQueue priorityQueue;
    uint8_t priorityLevelCount;
    OSAL_DECLARE_SEMAPHORE(messageSemaphore);
    HdfWlanQueue *queues[0];
} PriorityQueueImpl;

PriorityQueue *CreatePriorityQueue(uint16_t queueSize, uint8_t priorityLevelCount)
{
    uint8_t i;
    uint32_t queueMemSize;
    PriorityQueueImpl *priorityQueue = NULL;
    uint32_t ret = HDF_SUCCESS;
    HDF_STATUS status;
    if (priorityLevelCount > MAX_PRIORITY_LEVEL || priorityLevelCount == 0) {
        HDF_LOGE("%s:priorityLevelCount must in 1 to 8", __func__);
        return NULL;
    }
    queueMemSize = sizeof(PriorityQueueImpl) + (priorityLevelCount * sizeof(HdfWlanQueue *));
    priorityQueue = (PriorityQueueImpl *)OsalMemCalloc(queueMemSize);
    if (priorityQueue == NULL) {
        HDF_LOGE("%s: Request memory failed", __func__);
        return NULL;
    }
    priorityQueue->priorityLevelCount = priorityLevelCount;
    for (i = 0; i < priorityLevelCount; i++) {
        priorityQueue->queues[i] = CreateQueue(queueSize);
        if (priorityQueue->queues[i] == NULL) {
            ret = HDF_FAILURE;
            HDF_LOGE("%s:Init message queue failed!QueueID=%d,ret=%d", __func__, i, ret);
            break;
        }
    }
    if (ret != HDF_SUCCESS) {
        DestroyPriorityQueue((PriorityQueue *)priorityQueue);
        return NULL;
    }
    status = OsalSemInit(&priorityQueue->messageSemaphore, 0);
    if (status != HDF_SUCCESS) {
        HDF_LOGE("%s: OsalSemInit failed! status=%d", __func__, status);
        DestroyPriorityQueue((PriorityQueue *)priorityQueue);
        return NULL;
    }
    HDF_LOGD("%s: CreatePriorityQueue finished!", __func__);
    return (PriorityQueue *)priorityQueue;
}

void DestroyPriorityQueue(PriorityQueue *queue)
{
    uint8_t i;
    HDF_STATUS status;
    PriorityQueueImpl *queueImpl = (PriorityQueueImpl *)queue;
    if (queue == NULL) {
        HDF_LOGE("%s: Input param is null", __func__);
        return;
    }

    for (i = 0; i < queueImpl->priorityLevelCount; i++) {
        if (queueImpl->queues[i] == NULL) {
            continue;
        }
        DestroyQueue(queueImpl->queues[i]);
        queueImpl->queues[i] = NULL;
    }
    status = OsalSemDestroy(&queueImpl->messageSemaphore);
    if (status != HDF_SUCCESS) {
        HDF_LOGE("%s:Destroy message queue semaphore failed! status=%d", __func__, status);
    }

    OsalMemFree(queueImpl);
    HDF_LOGD("%s: DestroyPriorityQueue finished!", __func__);
}

int32_t PushPriorityQueue(PriorityQueue *queue, const uint8_t priority, void *context)
{
    uint32_t ret;
    uint8_t pri;
    PriorityQueueImpl *queueImpl = NULL;
    if (queue == NULL || context == NULL) {
        HDF_LOGE("%s: Input param is null!", __func__);
        return HDF_FAILURE;
    }
    queueImpl = (PriorityQueueImpl *)queue;
    pri = priority;
    if (priority >= queueImpl->priorityLevelCount) {
        pri = queueImpl->priorityLevelCount - 1;
    }

    ret = PushQueue(queueImpl->queues[pri], context);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s:Write queue failed! ret=%d", __func__, ret);
        return ret;
    }

    (void)OsalSemPost(&queueImpl->messageSemaphore);
    HDF_LOGD("%s:PushPriorityQueue successful!", __func__);
    return HDF_SUCCESS;
}

static void *PopQueueByPri(PriorityQueueImpl *queue)
{
    void *context = NULL;
    uint8_t i;
    for (i = 0; i < queue->priorityLevelCount; i++) {
        context = PopQueue(queue->queues[i]);
        if (context != NULL) {
            return context;
        }
    }
    return NULL;
}

void *PopPriorityQueue(PriorityQueue *queue, uint32_t waitInMS)
{
    PriorityQueueImpl *queueImpl = (PriorityQueueImpl *)queue;
    void *context = NULL;
    HDF_STATUS status;
    if (queue == NULL) {
        HDF_LOGE("%s: Input param is null!", __func__);
        return NULL;
    }

    context = PopQueueByPri(queueImpl);
    if (context != NULL || waitInMS == 0) {
        HDF_LOGD("%s: Context isn't null or waitInMs is zero!", __func__);
        return context;
    }

    status = OsalSemWait(&queueImpl->messageSemaphore, waitInMS);
    if (status != HDF_SUCCESS) {
        return NULL;
    }
    return PopQueueByPri(queueImpl);
}
