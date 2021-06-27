/*
 * Copyright (c) 2017 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ROC_PRIVATE_H_
#define ROC_PRIVATE_H_

#include "roc/address.h"
#include "roc/context.h"
#include "roc/receiver.h"
#include "roc/sender.h"

#include "roc_audio/units.h"
#include "roc_core/atomic.h"
#include "roc_core/buffer_pool.h"
#include "roc_core/heap_allocator.h"
#include "roc_core/mutex.h"
#include "roc_core/unique_ptr.h"
#include "roc_netio/transceiver.h"
#include "roc_packet/address.h"
#include "roc_packet/iwriter.h"
#include "roc_packet/packet_pool.h"
#include "roc_pipeline/receiver.h"
#include "roc_pipeline/sender.h"
#include "roc_rtp/format_map.h"

const roc::packet::Address& get_address(const roc_address* address);
roc::packet::Address& get_address(roc_address* address);

bool make_context_config(roc_context_config& out, const roc_context_config& in);

bool make_sender_config(roc::pipeline::SenderConfig& out, const roc_sender_config& in);
bool make_receiver_config(roc::pipeline::ReceiverConfig& out,
                          const roc_receiver_config& in);

bool make_port_config(roc::pipeline::PortConfig& out,
                      roc_port_type type,
                      roc_protocol proto,
                      const roc::packet::Address& addr);

struct roc_context {
    roc_context(const roc_context_config& cfg);

    roc::core::HeapAllocator allocator;

    roc::packet::PacketPool packet_pool;
    roc::core::BufferPool<uint8_t> byte_buffer_pool;
    roc::core::BufferPool<roc::audio::sample_t> sample_buffer_pool;

    roc::netio::Transceiver trx;

    roc::core::Atomic counter;
};

struct roc_sender {
    roc_sender(roc_context& ctx, roc::pipeline::SenderConfig& cfg);

    roc_context& context;

    roc::fec::CodecMap codec_map;
    roc::rtp::FormatMap format_map;

    roc::pipeline::SenderConfig config;

    roc::pipeline::PortConfig source_port;
    roc::pipeline::PortConfig repair_port;

    roc::core::UniquePtr<roc::pipeline::Sender> sender;
    roc::packet::IWriter* writer;

    roc::packet::Address address;

    roc::core::Mutex mutex;

    size_t num_channels;
};

struct roc_receiver {
    roc_receiver(roc_context& ctx, roc::pipeline::ReceiverConfig& cfg);

    roc_context& context;

    roc::fec::CodecMap codec_map;
    roc::rtp::FormatMap format_map;

    roc::pipeline::Receiver receiver;

    size_t num_channels;
};

#endif // ROC_PRIVATE_H_
