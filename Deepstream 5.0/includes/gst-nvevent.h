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
 * <b>NVIDIA GStreamer DeepStream: Custom Events</b>
 *
 * @b Description: This file specifies the NVIDIA DeepStream GStreamer custom
 * event functions, used to map events to individual sources which
 * are batched together by Gst-nvstreammux.
 *
 */

/**
 * @defgroup  gstreamer_nvevent  Events: Custom Events API
 *
 * Specifies GStreamer custom event functions, used to map events
 * to individual sources which are batched together by Gst-nvstreammux.
 *
 * @ingroup gst_mess_evnt_qry
 * @{
 */

#ifndef __GST_NVEVENT_H__
#define __GST_NVEVENT_H__

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLAG(name) GST_EVENT_TYPE_##name

/** Defines supported types of custom events. */
typedef enum {
  /** Specifies a custom event to indicate Pad Added. */
  GST_NVEVENT_PAD_ADDED
    = GST_EVENT_MAKE_TYPE (400, FLAG(DOWNSTREAM) | FLAG(SERIALIZED)),
  /** Specifies a custom event to indicate Pad Deleted. */
  GST_NVEVENT_PAD_DELETED
    = GST_EVENT_MAKE_TYPE (401, FLAG(DOWNSTREAM) | FLAG(SERIALIZED)),
  /** Specifies a custom event to indicate EOS of a particular stream
   in a batch. */
  GST_NVEVENT_STREAM_EOS
    = GST_EVENT_MAKE_TYPE (402, FLAG(DOWNSTREAM) | FLAG(SERIALIZED)),
  /** Specifies a custom event to indicate a stream segment. */
  GST_NVEVENT_STREAM_SEGMENT
    = GST_EVENT_MAKE_TYPE (403, FLAG(DOWNSTREAM) | FLAG(SERIALIZED))
} GstNvEventType;
#undef FLAG

/**
 * Creates a "custom pad added" event for the specified source.
 *
 * @param[in] source_id     Source ID of the stream to be added to the pipeline;
 *                          also the pad ID of the sinkpad of the
 *                          Gst-nvstreammux plugin for which the source
 *                          is configured.
 * @return  A pointer to the event corresponding to the request if successful,
 *  or NULL otherwise.
 */
GstEvent * gst_nvevent_new_pad_added (guint source_id);

/**
 * Creates a "custom pad deleted" event for the specified source.
 *
 * @param[in] source_id     Source ID of the stream to be removed
 *                          from the pipeline; also the pad ID of the sinkpad
 *                          of the Gst-nvstreammux plugin for which
 *                          the source is configured.
 * @return  A pointer to the event corresponding to the request if successful,
 *  or NULL otherwise.
 */
GstEvent * gst_nvevent_new_pad_deleted (guint source_id);

/**
 * Creates a "custom EOS" event for the specified source.
 *
 * @param[in] source_id     Source ID of the stream for which EOS is to be sent;
 *                          also the pad ID  of the sinkpad of the
 *                          Gst-nvstreammux plugin for which
 *                          the source is configured.
 * @return  A pointer to the event corresponding to request if sucxessful,
 *  or NULL otherwise.
 */
GstEvent * gst_nvevent_new_stream_eos (guint source_id);

/**
 * Creates a "custom segment" event for the specified source.
 *
 * @param[in] source_id     Source ID of the stream for which a segment event
 *                          is to be sent; also the pad ID of the sinkpad
 *                          of the Gst-nvstreammux plugin for which the source
 *                          is configured.
 * @param[in] segment       A pointer to a copy of the segment to be sent
 *                          with the event; corresponds to the original segment
 *                          for the specified source.
 * @return  A pointer to the event corresponding to the request if successful,
 *  or NULL otherwise.
 */
GstEvent * gst_nvevent_new_stream_segment (guint source_id, GstSegment *segment);

/**
 * Parses a "pad added" event received on the sinkpad.
 *
 * @param[in] event         A pointer to the event received on the sinkpad
 *                          when the pad is added to Gst-nvstreammux.
 * @param[out] source_id    A pointer to the parsed source ID for the event.
 */
void gst_nvevent_parse_pad_added (GstEvent * event, guint * source_id);

/**
 * Parses a "pad deleted" event received on the sinkpad.
 *
 * @param[in] event         A pointer to the event received on the sinkpad
 *                          when the pad is deleted from Gst-nvstreammux.
 * @param[out] source_id    A pointer to the parsed source ID for the event.
 */
void gst_nvevent_parse_pad_deleted (GstEvent * event, guint * source_id);

/**
 * Parses a "stream EOS" event received on the sinkpad.
 *
 * @param[in] event         A pointer to the event received on the sinkpad
 *                          when the source ID sends the EOS event.
 * @param[out] source_id    A pointer to the parsed source ID for the event.
 */
void gst_nvevent_parse_stream_eos (GstEvent * event, guint * source_id);

/**
 * Parses a "stream segment" event received on the sinkpad.
 *
 * @param[in] event         The event received on the sinkpad
 *                          when the source ID sends a segment event.
 * @param[out] source_id    A pointer to the parsed source ID for which
 *                          the event is sent.
 * @param[out] segment      A double pointer to the parsed segment
 *                          corresponding to source ID for the event.
 */
void gst_nvevent_parse_stream_segment (GstEvent * event, guint * source_id,
    GstSegment **segment);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
