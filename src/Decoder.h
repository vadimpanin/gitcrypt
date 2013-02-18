// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma  once

#include <boost/circular_buffer.hpp>

#include "Reader.h"
#include "Writer.h"

template< class T >
class Decoder
{
public:
	typedef T Line;

	void process( Input* in, Output* out,  ReversibleFiltersChain<T>& chain )
	{
		T line;
		line.reserve( MaxBlock );

		Reader reader( in );
		Writer writer( out );

		T buf;

		reader.read( buf, MagicBegin.size() );
		if( buf != MagicBegin )
		{
			// file is not encoded, streamline it to output
			writer.write( buf );
			while( reader.readFast( buf, MaxBlock ) ) writer.write( buf );
			return;
		}
		else
		{
			// skip header
			boost::circular_buffer< Line::value_type > cb( MagicEnd.size() );

			while( !equalContainers( MagicEnd, cb ) )
			{
				if( 1 != reader.read( cb, 1 ) )
					THROW_RUNTIME( "unable to skip header" );
			}
		}

		size_t read_size;
		while( ( read_size = reader.readFast( buf, MaxBlock ) ) != 0 )
		{
			assert( buf.size() == read_size );

			typename T::value_type* p = &buf[0];
			for( size_t i = 0; i < read_size; i++ )
			{
				typename T::value_type& c = p[i];
				line.push_back( c );

				if( EndLine == c )
				{
					processLine( line, writer, chain );
					line.clear();
				}
			}
		}

		if( line.size() )
			THROW_RUNTIME( "invalid encrypted file end" );
	}

private:

	template< typename A, typename B >
	bool equalContainers( const A& a, const B& b )
	{
		if( a.size() != b.size() ) return false;
		return std::equal( a.begin(), a.end(), b.begin() );
	}

	void processLine( const T& line, Writer& writer, ReversibleFiltersChain<T>& chain )
	{
		_LOG( "\ndec in " << line << "\n" );

		T& reversed = chain.reverse( line );

		writer.write( reversed );
		_LOG( "\ndec out " << reversed << "\n" );

		assert( chain.filter( T( reversed ) ) == line );
	}

};
