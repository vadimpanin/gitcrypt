// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <stdexcept>
#include <sstream>

template< class T >
class ExceptionFormatter : public T
{
public:
	ExceptionFormatter( const std::string& str = std::string() )
		: T( "" )
	{
		ss_ << str;
	}

	ExceptionFormatter( const ExceptionFormatter& e )
		: T( "" )
	{
		ss_ << e.ss_.str();
	}

	~ExceptionFormatter() throw()
	{
	}

	template< typename TT >
	ExceptionFormatter& operator << ( const TT& t )
	{
		ss_ << t;
		return *this;
	}

	const char * what() const throw()
	{
		out_ = ss_.str();
		return out_.c_str();
	}

private:
	std::stringstream ss_;
	mutable std::string out_;
};

typedef ExceptionFormatter< std::runtime_error > ExceptionRuntime;
typedef ExceptionFormatter< std::logic_error > ExceptionLogic;

#define THROW_RUNTIME( x ) throw ExceptionRuntime() /*<< __FUNCTION__ << ": "*/ << x
#define THROW_LOGIC( x ) throw ExceptionLogic() << __FUNCTION__ << ": " << x
