// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "Exception.h"

#include <zlib.h>

class Zlib
{
public:
	enum { MaxBlock = 16384 };

	struct Deflator
	{
		typedef z_stream Context;

		static int begin( Context& zs, int compression )
		{
			return deflateInit( &zs, compression );
		}

		static int process( Context& zs, int flush )
		{
			return deflate( &zs, flush );
		}

		static int end( Context& zs )
		{
			return deflateEnd( &zs );
		}
	};

	struct Inflator
	{
		typedef z_stream Context;

		static int begin( Context& zs, int param )
		{
			(void)param;
			return inflateInit( &zs );
		}

		static int process( Context& zs, int param )
		{
			return inflate( &zs, 0 );
		}

		static int end( Context& zs )
		{
			return inflateEnd( &zs );
		}
	};

	void compress( const std::string& src, std::string& dst )
	{
		process< Deflator >( src, dst, Z_BEST_SPEED, Z_FINISH );
	}

	void decompress( const std::string& src, std::string& dst )
	{
		process< Inflator >( src, dst, 0, 0 );
	}

private:
	template< class Processor >
	void process( const std::string& src, std::string& dst, int initParam, int processParam )
	{
		dst.clear();
		dst.resize( MaxBlock );

		typename Processor::Context zs = { 0 };

		if( Processor::begin( zs, initParam ) != Z_OK )
			THROW_RUNTIME( "zlib init failed" );

		zs.next_in = (Bytef*)src.data();
		zs.avail_in = src.size();

		int ret;
		size_t processed = 0;

		zs.next_out = (Bytef*)( &dst[ 0 ] );
		zs.avail_out = dst.size();

		do
		{
			ret = Processor::process( zs, processParam );
			assert( ret != Z_STREAM_ERROR );

			processed += zs.total_out - processed;

			if( 0 == zs.avail_out )
			{
				dst.resize( dst.size() + MaxBlock );

				zs.next_out = (Bytef*)( &dst[ processed ] );
				zs.avail_out = dst.size() - processed;
			}
		}
		while( !zs.avail_out );

		assert( 0 == zs.avail_in );

		Processor::end( zs );

		dst.resize( processed );

		if (ret != Z_STREAM_END)
			THROW_RUNTIME( "Exception during zlib processing: (" << ret << ") " << zs.msg );
	}
};
