/**
 * MltFilter.h - MLT Wrapper
 * Copyright (C) 2004-2005 Charles Yates
 * Author: Charles Yates <charles.yates@pandora.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
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

#ifndef _MLTPP_FILTER_H_
#define _MLTPP_FILTER_H_

#include <framework/mlt.h>

#include "MltService.h"

namespace Mlt
{
	class Filter : public Service
	{
		public:
			virtual mlt_filter get_filter( ) = 0;
			mlt_service get_service( );
			int connect( Service &service, int index = 0 );
			void set_in_and_out( mlt_position in, mlt_position out );
			mlt_position get_in( );
			mlt_position get_out( );
			int get_track( );
	};

	class FilterInstance : public Filter
	{
		private:
			bool destroy;
			mlt_filter instance;
		public:
			mlt_filter get_filter( );
			FilterInstance( char *id, char *service = NULL );
			FilterInstance( Filter &filter );
			FilterInstance( mlt_filter filter );
			virtual ~FilterInstance( );
	};
}

#endif
