/*
 * filter_greyscale.c -- greyscale filter
 * Copyright (C) 2003-2004 Ushodaya Enterprises Limited
 * Author: Charles Yates <charles.yates@pandora.be>
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "filter_greyscale.h"

#include <framework/mlt_frame.h>

#include <stdio.h>
#include <stdlib.h>

/** Do it :-).
*/

static int filter_get_image( mlt_frame this, uint8_t **image, mlt_image_format *format, int *width, int *height, int writable )
{
	int error = mlt_frame_get_image( this, image, format, width, height, 1 );
	if ( error == 0 && *format == mlt_image_yuv422 )
	{
		uint8_t *p = *image;
		uint8_t *q = *image + *width * *height * 2;
		while ( p ++ != q )
			*p ++ = 128;
	}
	return error;
}

/** Filter processing.
*/

static mlt_frame filter_process( mlt_filter this, mlt_frame frame )
{
	mlt_frame_push_get_image( frame, filter_get_image );
	return frame;
}

/** Constructor for the filter.
*/

mlt_filter filter_greyscale_init( void *arg )
{
	mlt_filter this = mlt_filter_new( );
	if ( this != NULL )
		this->process = filter_process;
	return this;
}

