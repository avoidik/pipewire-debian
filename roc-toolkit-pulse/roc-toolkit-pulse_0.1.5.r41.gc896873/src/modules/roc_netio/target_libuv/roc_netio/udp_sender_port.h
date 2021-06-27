/*
 * Copyright (c) 2015 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_netio/target_libuv/roc_netio/udp_sender_port.h
//! @brief UDP sender.

#ifndef ROC_NETIO_UDP_SENDER_PORT_H_
#define ROC_NETIO_UDP_SENDER_PORT_H_

#include <uv.h>

#include "roc_core/iallocator.h"
#include "roc_core/mutex.h"
#include "roc_core/refcnt.h"
#include "roc_netio/basic_port.h"
#include "roc_netio/iclose_handler.h"
#include "roc_packet/address.h"
#include "roc_packet/iwriter.h"

namespace roc {
namespace netio {

//! UDP sender.
class UDPSenderPort : public BasicPort, public packet::IWriter {
public:
    //! Initialize.
    UDPSenderPort(ICloseHandler& close_handler,
                  const packet::Address&,
                  uv_loop_t& event_loop,
                  core::IAllocator& allocator);

    //! Destroy.
    ~UDPSenderPort();

    //! Get bind address.
    virtual const packet::Address& address() const;

    //! Open sender.
    virtual bool open();

    //! Asynchronously close sender.
    virtual void async_close();

    //! Write packet.
    //! @remarks
    //!  May be called from any thread.
    virtual void write(const packet::PacketPtr&);

private:
    static void close_cb_(uv_handle_t* handle);
    static void write_sem_cb_(uv_async_t* handle);
    static void send_cb_(uv_udp_send_t* req, int status);

    packet::PacketPtr read_();
    void close_();

    ICloseHandler& close_handler_;

    uv_loop_t& loop_;

    uv_async_t write_sem_;
    bool write_sem_initialized_;

    uv_udp_t handle_;
    bool handle_initialized_;

    packet::Address address_;

    core::List<packet::Packet> list_;
    core::Mutex mutex_;

    size_t pending_;
    bool stopped_;
    bool closed_;

    unsigned packet_counter_;
};

} // namespace netio
} // namespace roc

#endif // ROC_NETIO_UDP_SENDER_PORT_H_
