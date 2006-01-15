#ifndef MVPMC_CONFIG_H
#define MVPMC_CONFIG_H

/*
 *  $Id$
 *
 *  Copyright (C) 2005, Jon Gettler
 *  http://mvpmc.sourceforge.net/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <limits.h>

#define CONFIG_MAGIC		0x434f4e46

typedef struct {
	char label[64];
	int hide;
} config_mythtv_rg_t;

/*
 * The bit definitions for the config_t bitmask
 */
#define CONFIG_SCREENSAVER	0x00000001
#define CONFIG_MODE		0x00000002
#define CONFIG_AUDIO_OUTPUT	0x00000004
#define CONFIG_VIDEO_OUTPUT	0x00000008
#define CONFIG_ASPECT		0x00000010
#define CONFIG_OSD_BITRATE	0x00000020
#define CONFIG_OSD_CLOCK	0x00000040
#define CONFIG_OSD_DEMUX_INFO	0x00000080
#define CONFIG_OSD_PROGRESS	0x00000100
#define CONFIG_OSD_PROGRAM	0x00000200
#define CONFIG_OSD_TIMECODE	0x00000400
#define CONFIG_BRIGHTNESS	0x00000800
#define CONFIG_MYTHTV_CONTROL	0x00001000
#define CONFIG_MYTHTV_PROGRAM	0x00002000
#define CONFIG_VOLUME		0x00004000
#define CONFIG_VIEWPORT		0x00008000
#define CONFIG_THEME		0x00010000
#define CONFIG_MYTHTV_IP	0x00020000
#define CONFIG_MCLIENT_IP	0x00040000
#define CONFIG_PLAYBACK_OSD	0x00080000
#define CONFIG_PLAYBACK_PAUSE	0x00100000
#define CONFIG_MYTHTV_SORT	0x00200000
#define CONFIG_MYTHTV_PROGRAMS	0x00400000
#define CONFIG_MYTHTV_RECGROUP	0x00800000

#define MYTHTV_RG_MAX	32

/*
 * The config_t structure will hold all the user settings that can survive
 * a crash or restart.  Eventually, this data will be written to flash or
 * a file so that it can survive a reboot.
 */
typedef struct {
	uint32_t		magic;
	uint32_t		bitmask;
	int			screensaver_timeout;
	av_mode_t		av_mode;
	av_passthru_t		av_audio_output;
	av_video_output_t	av_video_output;
	av_aspect_t		av_aspect;
	int			osd_bitrate;
	int			osd_clock;
	int			osd_demux_info;
	int			osd_progress;
	int			osd_program;
	int			osd_timecode;
	int			brightness;
	int			mythtv_tcp_control;
	int			mythtv_tcp_program;
	int			volume;
	unsigned short		viewport[4];
	char			theme[PATH_MAX];
	char			mythtv_ip[64];
	char			mclient_ip[64];
	int			playback_osd;
	int			playback_pause;
	int			mythtv_sort;
#ifndef MVPMC_HOST
	int			firsttime;/*First run since powerup?*/
#endif
	show_sort_t		mythtv_programs;
	config_mythtv_rg_t	mythtv_recgroup[MYTHTV_RG_MAX];
} config_t;

extern config_t *config;

/*
 * The item definitions for what can be stored in the config file.
 */
#define CONFIG_ITEM_SCREENSAVER		0x0001
#define CONFIG_ITEM_MODE		0x0002
#define CONFIG_ITEM_AUDIO_OUTPUT	0x0003
#define CONFIG_ITEM_VIDEO_OUTPUT	0x0004
#define CONFIG_ITEM_ASPECT		0x0005
#define CONFIG_ITEM_OSD_BITRATE		0x0006
#define CONFIG_ITEM_OSD_CLOCK		0x0007
#define CONFIG_ITEM_OSD_DEMUX_INFO	0x0008
#define CONFIG_ITEM_OSD_PROGRESS	0x0009
#define CONFIG_ITEM_OSD_PROGRAM		0x000a
#define CONFIG_ITEM_OSD_TIMECODE	0x000b
#define CONFIG_ITEM_BRIGHTNESS		0x000c
#define CONFIG_ITEM_MYTHTV_CONTROL	0x000d
#define CONFIG_ITEM_MYTHTV_PROGRAM	0x000e
#define CONFIG_ITEM_VOLUME		0x000f
#define CONFIG_ITEM_VIEWPORT		0x0010
#define CONFIG_ITEM_THEME		0x0011
#define CONFIG_ITEM_MYTHTV_IP		0x0012
#define CONFIG_ITEM_MCLIENT_IP		0x0013
#define CONFIG_ITEM_PLAYBACK_OSD	0x0014
#define CONFIG_ITEM_PLAYBACK_PAUSE	0x0015
#define CONFIG_ITEM_MYTHTV_SORT		0x0016
#define CONFIG_ITEM_MYTHTV_PROGRAMS	0x0017
#define CONFIG_ITEM_MYTHTV_RG_HIDE	0x0018
#define CONFIG_ITEM_MYTHTV_RG_SHOW	0x0019

/*
 * The flags in config_list_t
 */
#ifdef MVPMC_HOST
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define CONFIG_FLAGS_PLATFORM		0x0001
#else
#define CONFIG_FLAGS_PLATFORM		0x0008
#endif
#else
#define CONFIG_FLAGS_PLATFORM		0x0002
#endif
#define CONFIG_FLAGS_COMPRESSED		0x0004

#define CONFIG_MAX_SIZE			(1024*128)
#define CONFIG_MAX_COMPRESSED_SIZE	(1024*32)

#define CONFIG_VERSION			1

typedef struct {
	uint16_t	type;
	uint16_t	buflen;
	uint8_t		buf[0];
} config_item_t;

typedef struct {
	int		magic;
	int		crc;		/* uncompressed data with crc = 0 */
	int		version;
	uint32_t	buflen;
	uint32_t	flags;
	uint32_t	unused[3];
	uint8_t		buf[0];
} config_list_t;

extern int config_compress(config_list_t*, config_list_t*);
extern int config_uncompress(config_list_t*, config_list_t*);
extern int save_config_file(char *file);
extern int load_config_file(char *file, int override);
extern void set_config(void);

extern char *config_file;

#endif /* MVPMC_CONFIG_H */
