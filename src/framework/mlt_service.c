/*
 * mlt_service.c -- interface for all service classes
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

#include "config.h"
#include "mlt_service.h"
#include "mlt_frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** IMPORTANT NOTES

  	The base service implements a null frame producing service - as such,
	it is functional without extension and will produce test cards frames 
	and PAL sized audio frames.

	PLEASE DO NOT CHANGE THIS BEHAVIOUR!!! OVERRIDE THE METHODS THAT 
	CONTROL THIS IN EXTENDING CLASSES.
*/

/** Private service definition.
*/

typedef struct
{
	int size;
	int count;
	mlt_service *in;
	mlt_service out;
}
mlt_service_base;

/** Private methods
*/

static void mlt_service_disconnect( mlt_service this );
static void mlt_service_connect( mlt_service this, mlt_service that );
static int service_get_frame( mlt_service this, mlt_frame_ptr frame, int index );

/** Constructor
*/

int mlt_service_init( mlt_service this, void *child )
{
	// Initialise everything to NULL
	memset( this, 0, sizeof( struct mlt_service_s ) );

	// Assign the child
	this->child = child;

	// Generate private space
	this->private = calloc( sizeof( mlt_service_base ), 1 );

	// Associate the methods
	this->get_frame = service_get_frame;
	
	// Initialise the properties
	return mlt_properties_init( &this->parent, this );
}

/** Connect a producer service.
	Returns: > 0 warning, == 0 success, < 0 serious error
			 1 = this service does not accept input
			 2 = the producer is invalid
			 3 = the producer is already registered with this consumer
*/

int mlt_service_connect_producer( mlt_service this, mlt_service producer, int index )
{
	int i = 0;

	// Get the service base
	mlt_service_base *base = this->private;

	// Check if the producer is already registered with this service
	for ( i = 0; i < base->count; i ++ )
		if ( base->in[ i ] == producer )
			return 3;

	// Allocate space
	if ( index >= base->size )
	{
		int new_size = base->size + index + 10;
		base->in = realloc( base->in, new_size * sizeof( mlt_service ) );
		if ( base->in != NULL )
		{
			for ( i = base->size; i < new_size; i ++ )
				base->in[ i ] = NULL;
			base->size = new_size;
		}
	}

	// If we have space, assign the input
	if ( base->in != NULL && index >= 0 && index < base->size )
	{
		// Now we disconnect the producer service from its consumer
		mlt_service_disconnect( producer );
		
		// Add the service to index specified
		base->in[ index ] = producer;
		
		// Determine the number of active tracks
		if ( index >= base->count )
			base->count = index + 1;

		// Now we connect the producer to its connected consumer
		mlt_service_connect( producer, this );

		// Inform caller that all went well
		return 0;
	}
	else
	{
		return -1;
	}
}

/** Disconnect this service from its consumer.
*/

static void mlt_service_disconnect( mlt_service this )
{
	// Get the service base
	mlt_service_base *base = this->private;

	// There's a bit more required here...
	base->out = NULL;
}

/** Associate this service to the its consumer.
*/

static void mlt_service_connect( mlt_service this, mlt_service that )
{
	// Get the service base
	mlt_service_base *base = this->private;

	// There's a bit more required here...
	base->out = that;
}

/** Get the first connected producer service.
*/

mlt_service mlt_service_get_producer( mlt_service this )
{
	mlt_service producer = NULL;

	// Get the service base
	mlt_service_base *base = this->private;

	if ( base->in != NULL )
		producer = base->in[ 0 ];
	
	return producer;
}

/** Default implementation of get_frame.
*/

static int service_get_frame( mlt_service this, mlt_frame_ptr frame, int index )
{
	mlt_service_base *base = this->private;
	if ( index < base->count )
	{
		mlt_service producer = base->in[ index ];
		if ( producer != NULL )
			return mlt_service_get_frame( producer, frame, index );
	}
	*frame = mlt_frame_init( );
	return 0;
}

/** Obtain a frame.
*/

int mlt_service_get_frame( mlt_service this, mlt_frame_ptr frame, int index )
{
	return this->get_frame( this, frame, index );
}

/** Close the service.
*/

void mlt_service_close( mlt_service this )
{
	mlt_service_base *base = this->private;
	free( base->in );
	free( base );
	mlt_properties_close( &this->parent );
}

