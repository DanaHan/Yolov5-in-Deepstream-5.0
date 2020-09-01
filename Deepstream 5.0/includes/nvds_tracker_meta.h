/*
 * Copyright (c) 2020, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 *
 */
/**
 * @file
 * <b>Defines Tracker Metadata</b>
 */
/**
 * @defgroup  ee_tracker_group Tracker Metadata
 *
 * Specifies metadata concerning tracking.
 *
 * @ingroup NvDsMetaApi
 * @{
 */
#ifndef _NVDS_TRACKER_META_H_
#define _NVDS_TRACKER_META_H_

#include <stdint.h>
#include "nvll_osd_struct.h"
#include "nvdsmeta.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _NvDsPastFrameObj
{
    uint32_t frameNum;
    NvOSD_RectParams tBbox;
    float confidence;
    uint32_t age;
} NvDsPastFrameObj;

/**
 * One object in several past frames
 */
typedef struct _NvDsPastFrameObjList
{
    /**< Pointer to past frame info of this object. */
    NvDsPastFrameObj *list;
    /**< Number of frames this object appreared in the past. */
    uint32_t numObj;
    /**< Object tracking id. */
    uint64_t uniqueId;
    /**< Object class id. */
    uint16_t classId;
    /**< An array of the string describing the object class. */
    gchar objLabel[MAX_LABEL_SIZE];
} NvDsPastFrameObjList;

/**
 * List of objects in each stream
 * */
typedef struct _NvDsPastFrameObjStream
{
    NvDsPastFrameObjList *list; /**< Pointer to objects inside this stream. */
    uint32_t streamID;          /**< Stream id the same as frame_meta->pad_index. */
    uint64_t surfaceStreamID;   /**< Stream id used inside tracker plugin. */
    uint32_t numAllocated;      /**< Maximum number of objects allocated. */
    uint32_t numFilled;         /**< Number of objects in this frame. */
} NvDsPastFrameObjStream;

/**
 * Batch of lists of buffered objects
 */
typedef struct _NvDsPastFrameObjBatch
{
    NvDsPastFrameObjStream *list;  /**< Pointer to array of stream lists. */
    uint32_t numAllocated;  /**< Number of blocks allocated for the list. */
    uint32_t numFilled;     /**< Number of filled blocks in the list. */
} NvDsPastFrameObjBatch;

#ifdef __cplusplus
}
#endif

#endif

/** @} */