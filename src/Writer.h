// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class Writer
{
public:
	Writer( Output* output )
		: output_( output )
	{
	}

	template< typename T >
	size_t writeFast( const T& src )
	{
		return = output_->write( &src[0], sizeof( typename T::value_type ), src.size() );
	}

	template< typename T >
	size_t write( const T& src )
	{
		BOOST_FOREACH( const typename T::value_type& i, src )
		{
			if( 1 != output_->write( &i, sizeof( i ), 1 ) )
				THROW_RUNTIME( "Error writing" );
		}

		return src.size();
	}

private:
	Output* output_;
};
