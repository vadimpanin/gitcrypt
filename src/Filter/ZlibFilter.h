// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ReversibleFilter.h"

#if USE_ZLIB_DIRECTLY

#include "Alg/ZlibStl.h"

template< class T >
class ZlibFilter : public ReversibleFilter< T >
{
public:
	T& filter( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		engine_.compress( in, out_ );
		return out_;
	}

	T& reverse( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		engine_.decompress( in, out_ );
		return out_;
	}
private:
	Zlib engine_;
	T out_;
};

#else

#include <boost/iostreams/copy.hpp> 
#include <boost/iostreams/device/back_inserter.hpp> 
#include <boost/iostreams/filter/zlib.hpp> 
#include <boost/iostreams/filtering_stream.hpp>

template< class T >
class ZlibFilter : public ReversibleFilter< T >
{
public:
	T filter( const T& in )
	{
		return proceed< boost::iostreams::zlib_compressor >( in );
	}

	T reverse( const T& in )
	{
		return proceed< boost::iostreams::zlib_decompressor >( in );
	}

private:
	template< class Action >
	T proceed( const T& in )
	{
		T out;

		boost::iostreams::filtering_streambuf<boost::iostreams::output> fs; 
		fs.push( Action() ); 
		fs.push( boost::iostreams::back_inserter( out ) ); 
		boost::iostreams::copy( boost::make_iterator_range( in ), fs ); 

		return out;
	}
};

#endif
