/*
 * Copyright (c) 2018, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/**
 * @file
 * <b>NVIDIA GStreamer DeepStream: Helper Queries</b>
 *
 * @b Description: This file specifies the NVIDIA DeepStream GStreamer helper
 * query functions.
 *
 */
#ifndef __GST_NVQUERY_H__
#define __GST_NVQUERY_H__

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup gst_query_plugin Query Functions
 * Gets information such as the batch size and the number of streams.
 * @ingroup gst_mess_evnt_qry
 * @{
 */

/**
 * Creates a new batch-size query, which can be used by elements to query
 * the number of buffers in upstream elements' batched buffers.
 *
 * @return A pointer to the new batch size query.
 */
GstQuery * gst_nvquery_batch_size_new (void);

/**
 * Determines whether a query is a batch size query.
 *
 * params[in] query     A pointer to the query to be checked.
 *
 * @return  True if the query is a batch size query.
 */
gboolean gst_nvquery_is_batch_size (GstQuery * query);

/**
 * Sets the batch size, used by the elements responding to the batch size query.
 *
 * This function fails if the query is not a batch size query.
 *
 * params[in] query         A pointer to a batch size query.
 * params[in] batch_size    The batch size to be set.
 */
void gst_nvquery_batch_size_set (GstQuery * query, guint batch_size);

/**
 * Parses batch size from a batch size query.
 *
 * params[in] query         A pointer to a batch size query.
 * params[out] batch_size   A pointer to an unsigned integer in which the
 *                          batch size is stored.
 *
 * @return  True if the query was successfully parsed.
 */
gboolean gst_nvquery_batch_size_parse (GstQuery * query, guint * batch_size);

/**
 * Creates a number of streams query, used by elements to query
 * upstream the number of input sources.
 *
 * @return  A pointer to the new query.
 */
GstQuery * gst_nvquery_numStreams_size_new (void);

/**
 * Determines whether a query is a number-of-streams query.
 *
 * params[in] query     A pointer to the query to be checked.
 *
 * @return  A Boolean; true if the query is a number of streams query.
 */
gboolean gst_nvquery_is_numStreams_size (GstQuery * query);

/**
 * \brief  Sets the number of input sources.
 *
 * This function is used by elements responding to
 * a number of streams query. It fails if the query is not of the correct type.
 *
 * params[in] query             A pointer to a number-of-streams query.
 * params[in] numStreams_size   The number of input sources.
 */
void gst_nvquery_numStreams_size_set (GstQuery * query, guint numStreams_size);

/**
 * Parses the number of streams from a number of streams query.
 *
 * params[in] query         A pointer to a number-of-streams query.
 * params[out] batch_size   A pointer to an unsigned integer in which
 *                          the number of streams is stored.
 *
 * @return  True if the query was successfully parsed.
 */
gboolean gst_nvquery_numStreams_size_parse (GstQuery * query, guint * numStreams_size);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
