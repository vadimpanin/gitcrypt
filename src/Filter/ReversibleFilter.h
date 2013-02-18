// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

template< class T >
class ReversibleFilter
{
public:
	virtual T& filter( const T& ) = 0;
	virtual T& reverse( const T& ) = 0;

	typedef boost::shared_ptr< ReversibleFilter<T> > Ptr;
};

template< class T >
class ReversibleFiltersChain : public ReversibleFilter< T >
{
public:
	typedef typename ReversibleFilter< T >::Ptr Filter;

	void push_back( Filter p )
	{
		filters_.push_back( p );
	}

	T& filter( const T& in )
	{
		T* result = const_cast< T* >( &in );

		BOOST_FOREACH( Filter f, filters_ )
		{
			result = &(f->filter( *result ) );
		}

		return *result;
	}

	T& reverse( const T& in )
	{
		T* result = const_cast< T* >( &in );

		BOOST_REVERSE_FOREACH( Filter f, filters_ )
		{
			result = &(f->reverse( *result ) );
		}

		return *result;
	}

private:
	typedef typename std::vector< Filter > Filters;
	Filters filters_;
};
