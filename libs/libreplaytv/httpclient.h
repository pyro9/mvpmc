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

/*
 *  Overview of an HTTP request:
 *
 *  1. call hc_start_request() with an URL
 *
 *         hc = hc_start_request("http://192.168.117.3/index.html");
 *         if (!hc) {error}
 *
 *     Right now the address in the URL must be given as an IP address, not
 *     a hostname.  That's a bug, not a feature.
 *     
 *  2. add any necessary headers with hc_add_req_header; the Host header is
 *     created automatically; any others, including User-Agent, must be
 *     explicitely set
 *
 *         if (!hc_add_req_header(hc, "User-Agent", "Mozilla/4.0 (Isn't everything?)") != 0) {error}
 *         
 *  3. send the request; hc_send_request() uses GET, hc_post_request() uses
 *     POST, with the callback function called repeatedly for the data.  It
 *     should return the number of bytes filled in, and signal end of data by
 *     returning 0.
 *
 *         if (!hc_send_request(hc)) {error}
 *
 *  4. check the returned status; 200 is the normal success code, any 2.. code
 *     is a success of some sort. 3.. codes are redirects, 4.. are errors; see
 *     the HTTP specs for more information
 *
 *         if (hc_get_status(hc)/100 != 2) {exception of some sort}
 *
 *  5. Check any response headers you're interested in.
 *
 *         last_modified = hc_lookup_rsp_header(hc, "Last-Modified");
 *         if (!last_modified) {header wasn't supplied}
 *         
 *  6. read the data; if the item is known to be short, you have the memory for
 *     it (2x the data length; half of that as a contiguous block), and don't
 *     mind waiting for all of it, get it all with hc_read_all().  If one of
 *     conditions doesn't hold, call hc_read_pieces().  The callback function
 *     is given a buffer to a chunk of data, the amount of data there, and
 *     a pointer you supply, usually to a structure where it can store state,
 *     get parameters from the original caller and return information to the
 *     original caller
 *
 *         if (!(data = hc_read_all(hc))) {error}
 *
 *     Either way, you own the data chunk -- free() it when you're done with it
 *
 *  7. Clean up
 *
 *         hc_free(hc);
 */
#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "netclient.h"

#ifdef _WIN32
# define strcasecmp(a, b)  strcmp(a, b)
#endif

#define RTV_CHUNK_SZ (32 *1024)

//+************************************************************
// http read callback fxn prototype
// parms:
//         buf:    Read data from the replay device
//         len:    length of data
//         vd:     callback_data pointer 
// returncode:
//         Return 0 to keep receiving data.
//         Return 1 to to abort the transfer.
//+************************************************************
typedef int (*http_read_chunked_cb_t)(unsigned char *buf, size_t len, void *vd);

struct hc;

extern struct hc*      hc_start_request(char *url);
extern int             hc_add_req_header(struct hc *hc, const char *tag, const char *value);
extern int             hc_send_request(struct hc *hc, const char *append);
extern int             hc_post_request(struct hc *hc,
                                       int (*callback)(unsigned char *, size_t, void *), void *v);
extern int             hc_get_status(const struct hc * hc);
extern char*           hc_lookup_rsp_header(const struct hc *hc, const char *tag);
extern int             hc_read_all(struct hc *hc, char **data_p, unsigned int *len);
extern int             hc_read_pieces(const struct hc *,
                                      http_read_chunked_cb_t,
                                      void *,
                                      int);
extern void            hc_free(struct hc *hc);
#endif
