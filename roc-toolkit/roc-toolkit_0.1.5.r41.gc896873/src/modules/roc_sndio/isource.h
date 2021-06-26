/*
 * Copyright (c) 2017 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/isource.h
//! @brief Source interface.

#ifndef ROC_SNDIO_ISOURCE_H_
#define ROC_SNDIO_ISOURCE_H_

#include "roc_audio/frame.h"

namespace roc {
namespace sndio {

//! Source interface.
class ISource {
public:
    virtual ~ISource();

    //! Get source sample rate.
    virtual size_t sample_rate() const = 0;

    //! Check if the source has own clock.
    virtual bool has_clock() const = 0;

    //! Source state.
    enum State {
        //! Source is active and is producing some sound.
        Active,

        //! Source is inactive and is producing silence.
        Inactive
    };

    //! Get current source state.
    virtual State state() const = 0;

    //! Wait until the source state becomes active.
    //! @remarks
    //!  Spurious wakeups are allowed.
    virtual void wait_active() const = 0;

    //! Read frame.
    //! @returns
    //!  false if there is nothing to read anymore.
    virtual bool read(audio::Frame&) = 0;
};

} // namespace sndio
} // namespace roc

#endif // ROC_SNDIO_ISOURCE_H_
