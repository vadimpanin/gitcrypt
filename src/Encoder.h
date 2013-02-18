// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma  once

#include "Reader.h"
#include "Writer.h"

template< class T >
class Encoder
{
public:
	typedef T Line;

	void process( Input* in, Output* out, ReversibleFiltersChain<T>& chain )
	{
		T line;
		line.reserve( MaxBlock );

		Reader reader( in );
		Writer writer( out );

		writer.write( MagicBegin );
		writer.write( HeaderText );
		writer.write( MagicEnd );

		T buf;

		size_t read_size;
		while( ( read_size = reader.readFast( buf, MaxBlock ) ) != 0 )
		{
			assert( buf.size() == read_size );

			typename T::value_type* p = &buf[0];
			for( size_t i = 0; i < read_size; i++ )
			{
				typename T::value_type& c = p[i];
				line.push_back( c );

				if( EndLine == c || line.size() >= MaxBlock )
				{
					processLine( line, writer, chain );
					line.clear();
				}
			}
		}

		if( line.size() )
		{
			processLine( line, writer, chain );
			line.clear();
		}
	}

private:
	void processLine( const T& line, Writer& writer, ReversibleFiltersChain<T>& chain )
	{
		_LOG( "\nenc in " << line << "\n" );

		T& filtered = chain.filter( line );

		writer.write( filtered );
		_LOG( "\nenc out " << filtered << "\n" );

		assert( chain.reverse( T( filtered ) ) == line );
	}
};
