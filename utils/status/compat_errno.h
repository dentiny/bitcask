// This file implements linux errno to StatusCode.
//
// Adapted from abseil status implementation:
// https://github.com/abseil/abseil-cpp/blob/master/absl/status/status.h

#pragma once

#include <errno.h>

#include "utils/status/status_code.h"

namespace ghost {

inline StatusCode ConvertErrnoToStatusCode(int linux_errno) {
  switch (linux_errno) {
    case 0:
      return StatusCode::kOk;
    case EINVAL:        // Invalid argument
    case ENAMETOOLONG:  // Filename too long
    case E2BIG:         // Argument list too long
    case EDESTADDRREQ:  // Destination address required
    case EDOM:          // Mathematics argument out of domain of function
    case EFAULT:        // Bad address
    case EILSEQ:        // Illegal byte sequence
    case ENOPROTOOPT:   // Protocol not available
    case ENOTSOCK:      // Not a socket
    case ENOTTY:        // Inappropriate I/O control operation
    case EPROTOTYPE:    // Protocol wrong type for socket
    case ESPIPE:        // Invalid seek
      return StatusCode::kInvalidArgument;
    case ETIMEDOUT:  // Connection timed out
      return StatusCode::kDeadlineExceeded;
    case ENODEV:  // No such device
    case ENOENT:  // No such file or directory
#ifdef ENOMEDIUM
    case ENOMEDIUM:  // No medium found
#endif
    case ENXIO:  // No such device or address
    case ESRCH:  // No such process
      return StatusCode::kNotFound;
    case EEXIST:         // File exists
    case EADDRNOTAVAIL:  // Address not available
    case EALREADY:       // Connection already in progress
#ifdef ENOTUNIQ
    case ENOTUNIQ:  // Name not unique on network
#endif
      return StatusCode::kAlreadyExists;
    case EPERM:   // Operation not permitted
    case EACCES:  // Permission denied
#ifdef ENOKEY
    case ENOKEY:  // Required key not available
#endif
    case EROFS:  // Read only file system
      return StatusCode::kPermissionDenied;
    case ENOTEMPTY:   // Directory not empty
    case EISDIR:      // Is a directory
    case ENOTDIR:     // Not a directory
    case EADDRINUSE:  // Address already in use
    case EBADF:       // Invalid file descriptor
#ifdef EBADFD
    case EBADFD:  // File descriptor in bad state
#endif
    case EBUSY:    // Device or resource busy
    case ECHILD:   // No child processes
    case EISCONN:  // Socket is connected
#ifdef EISNAM
    case EISNAM:  // Is a named type file
#endif
#ifdef ENOTBLK
    case ENOTBLK:  // Block device required
#endif
    case ENOTCONN:  // The socket is not connected
    case EPIPE:     // Broken pipe
#ifdef ESHUTDOWN
    case ESHUTDOWN:  // Cannot send after transport endpoint shutdown
#endif
    case ETXTBSY:  // Text file busy
#ifdef EUNATCH
    case EUNATCH:  // Protocol driver not attached
#endif
      return StatusCode::kFailedPrecondition;
    case ENOSPC:  // No space left on device
#ifdef EDQUOT
    case EDQUOT:  // Disk quota exceeded
#endif
    case EMFILE:   // Too many open files
    case EMLINK:   // Too many links
    case ENFILE:   // Too many open files in system
    case ENOBUFS:  // No buffer space available
    case ENOMEM:   // Not enough space
    case ENOSR:    // No STREAM resources
    case EAGAIN:   // No more message to access
#ifdef EUSERS
    case EUSERS:  // Too many users
#endif
      return StatusCode::kResourceExhausted;
#ifdef ECHRNG
    case ECHRNG:  // Channel number out of range
#endif
    case EFBIG:      // File too large
    case EOVERFLOW:  // Value too large to be stored in data type
    case ERANGE:     // Result too large
      return StatusCode::kOutOfRange;
#ifdef ENOPKG
    case ENOPKG:  // Package not installed
#endif
    case ENOSYS:        // Function not implemented
    case ENOTSUP:       // Operation not supported
    case EAFNOSUPPORT:  // Address family not supported
#ifdef EPFNOSUPPORT
    case EPFNOSUPPORT:  // Protocol family not supported
#endif
    case EPROTONOSUPPORT:  // Protocol not supported
#ifdef ESOCKTNOSUPPORT
    case ESOCKTNOSUPPORT:  // Socket type not supported
#endif
    case EXDEV:  // Improper link
      return StatusCode::kUnimplemented;
#ifdef ECOMM
    case ECOMM:  // Communication error on send
#endif
    case ECONNREFUSED:  // Connection refused
    case ECONNABORTED:  // Connection aborted
    case ECONNRESET:    // Connection reset
    case EINTR:         // Interrupted function call
#ifdef EHOSTDOWN
    case EHOSTDOWN:  // Host is down
#endif
    case EHOSTUNREACH:  // Host is unreachable
    case ENETDOWN:      // Network is down
    case ENETRESET:     // Connection aborted by network
    case ENETUNREACH:   // Network unreachable
    case ENOLCK:        // No locks available
    case ENOLINK:       // Link has been severed
#ifdef ENONET
    case ENONET:  // Machine is not on the network
#endif
      return StatusCode::kUnavailable;
    case EDEADLK:  // Resource deadlock avoided
#ifdef ESTALE
    case ESTALE:  // Stale file handle
#endif
      return StatusCode::kAborted;
    case ECANCELED:  // Operation cancelled
      return StatusCode::kCancelled;
    case EIO:
      return StatusCode::kInternal;
    default:
      return StatusCode::kUnknown;
  }
}

}  // namespace ghost
