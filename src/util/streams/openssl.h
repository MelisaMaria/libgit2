/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_streams_openssl_h__
#define INCLUDE_streams_openssl_h__

#include "git2_util.h"
#include "streams/openssl_legacy.h"
#include "streams/openssl_dynamic.h"

#include "git2/sys/stream.h"

extern int git_stream_openssl_global_init(void);

#if defined(GIT_HTTPS_OPENSSL) && !defined(GIT_HTTPS_OPENSSL_DYNAMIC)
# include <openssl/ssl.h>
# include <openssl/err.h>
# include <openssl/x509v3.h>
# include <openssl/bio.h>
# endif

#ifdef GIT_HTTPS_OPENSSL
extern int git_openssl__set_ciphers(const char *ciphers);
extern int git_openssl__set_cert_location(const char *file, const char *path);
extern int git_stream_openssl_new(git_stream **out);
#endif

#endif