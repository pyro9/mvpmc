/*
 * Copyright (C) 2004 John Honeycutt
 * Copyright (C) 2002 John Todd Larason <jtl@molehill.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 */

#ifndef CRYPT_H
#define CRYPT_H

#include "rtv.h"

int rtv_decrypt(const char   *cyphertext, __u32  cypherlength,
                      char   *plainbuf,   __u32  plainbuflength,
                      __u32  *p_time,     __u32 *plainlen,
                      int     checksum_num);
int rtv_encrypt(const char   *plaintext,  __u32 plaintext_len,
                      char   *cyphertext, __u32 buffer_len, __u32 *cyphertext_len,
                      int     checksum_num);
#endif


