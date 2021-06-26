/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_address/io_uri.h
//! @brief Audio file or device URI.

#ifndef ROC_ADDRESS_IO_URI_H_
#define ROC_ADDRESS_IO_URI_H_

#include "roc_core/stddefs.h"

namespace roc {
namespace address {

//! Audio file or device URI.
struct IoURI {
    //! Initialize empty URI.
    IoURI();

    //! Returns true if the URI is empty.
    bool is_empty() const;

    //! Returns true if the scheme is "file".
    bool is_file() const;

    //! Returns true if the scheme is "file" and the path is "-".
    bool is_special_file() const;

    enum {
        // An estimate maximum length of encoded URI.
        MaxLength = 1280
    };

    //! URI scheme.
    //! May be "file" or device type, e.g. "alsa".
    char scheme[16];

    //! URI path.
    //! May be device name or file path depending on scheme.
    char path[1024];
};

//! Parse IoURI from string.
//!
//! The URI should be in one of the following forms:
//!
//!  - DEVICE_TYPE://DEVICE_NAME  (audio device)
//!
//!  - file:///ABS/PATH           (file, absolute path)
//!  - file://localhost/ABS/PATH  (equivalent to the above)
//!  - file:/ABS/PATH             (equivalent to the above)
//!
//!  - file:REL/PATH              (file, relative path)
//!
//!  - file://-                   (stdin or stdout)
//!  - file:-                     (equivalent to the above)
//!
//! Where:
//!  - DEVICE_TYPE specifies the audio system name, e.g. "alsa" or "pulse"
//!  - DEVICE_NAME specifies the audio device name, e.g. ALSA card name
//!  - /ABS/PATH specifies an absolute file path
//!  - REL/PATH specifies a relative file path
//!
//! Examples:
//!  - alsa://card0
//!  - file:///home/user/somefile.wav
//!  - file://localhost/home/user/somefile.wav
//!  - file:/home/user/somefile.wav
//!  - file:./somefile.wav
//!  - file:somefile.wav
//!  - file://-
//!  - file:-
//!
//! The URI syntax is defined by RFC 8089 and RFC 3986.
//!
//! The path part of the URI is percent-decoded.
//!
//! The RFC allows usages of file:// URIs both for local and remote files. Local files
//! should use either empty or special "localhost" hostname. This parser only recognizes
//! these two variants; other hostnames will be considered as a parsing error.
//!
//! The RFC allows only absolute paths in file:// URIs. This parser additionally allows
//! relative paths, but only in the "file:" form (without "//"). Relative paths are not
//! allowed in the "file://" form (with "//") because it would lead to an ambiguity.
//!
//! This parser also allows a non-standard "-" path for stdin/stdout.
//!
//! This parser does not try to perform full URI validation. For example, it does not
//! check that path contains only allowed symbols. If it can be parsed, it will be.
bool parse_io_uri(const char* str, IoURI& result);

//! Format IoURI to string.
//!
//! Formats a normalized form of the URI.
//!
//! The path part of the URI is percent-encoded if necessary.
//!
//! This function always uses the "file:" form (without "//") for files because this is
//! the only form that supports both absolute and relative paths.
//!
//! @returns
//!  true on success or false if the buffer is too small.
bool format_io_uri(const IoURI& uri, char* buf, size_t buf_size);

} // namespace address
} // namespace roc

#endif // ROC_ADDRESS_IO_URI_H_
