// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

template< class T >
class EndlineFilter : public ReversibleFilter< T >
{
public:
	T& filter( const T& in )
	{
		assert( &in != &out_ );

		out_ = in;
		out_ += "\n";
		return out_;
	}

	T& reverse( const T& in )
	{
		assert( &in != &out_ );

		if( !in.size() || in[ in.size() - 1 ] != '\n' )
			throw std::runtime_error( "EndlineFilter: no end of line in string");

		size_t newsize = in.size() - 1;
		out_.resize( newsize );
		memcpy( &out_[0], in.data(), newsize );
		return out_;
	}
private:
	T out_;
};
