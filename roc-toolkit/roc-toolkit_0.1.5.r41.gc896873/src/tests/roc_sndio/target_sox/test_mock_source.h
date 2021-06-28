/*
 * Copyright (c) 2015 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ROC_SNDIO_TARGET_SOX_TEST_MOCK_SOURCE_H_
#define ROC_SNDIO_TARGET_SOX_TEST_MOCK_SOURCE_H_

#include <CppUTest/TestHarness.h>

#include "roc_sndio/isource.h"

namespace roc {
namespace sndio {

class MockSource : public ISource {
public:
    MockSource()
        : pos_(0)
        , size_(0) {
    }

    virtual size_t sample_rate() const {
        return 0;
    }

    virtual bool has_clock() const {
        return false;
    }

    virtual State state() const {
        if (pos_ >= size_) {
            return Inactive;
        } else {
            return Active;
        }
    }

    virtual void wait_active() const {
        FAIL("not implemented");
    }

    virtual bool read(audio::Frame& frame) {
        size_t ns = frame.size();
        if (ns > size_ - pos_) {
            ns = size_ - pos_;
        }

        if (ns > 0) {
            memcpy(frame.data(), samples_ + pos_, ns * sizeof(audio::sample_t));
            pos_ += ns;
        }

        if (ns < frame.size()) {
            memset(frame.data() + ns, 0, (frame.size() - ns) * sizeof(audio::sample_t));
        }

        return true;
    }

    void add(size_t sz) {
        CHECK(size_ + sz <= MaxSz);

        for (size_t n = 0; n < sz; n++) {
            samples_[size_] = nth_sample_(size_);
            size_++;
        }
    }

    size_t num_returned() const {
        return pos_;
    }

private:
    enum { MaxSz = 256 * 1024 };

    audio::sample_t nth_sample_(size_t n) {
        return audio::sample_t(uint8_t(n)) / audio::sample_t(1 << 8);
    }

    audio::sample_t samples_[MaxSz];
    size_t pos_;
    size_t size_;
};

} // namespace sndio
} // namespace roc

#endif // ROC_SNDIO_TARGET_SOX_TEST_MOCK_SOURCE_H_
