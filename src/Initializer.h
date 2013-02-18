#pragma once

#include <fstream>
#include <ostream>
#include <boost/regex.hpp>
#include <boost/format.hpp>

class Initializer
{
public:

	class Element
	{
	public:
		Element( const std::string& filename, const std::string& init_text, const std::string& check_regex, bool have_to_exist )
			: filename_( filename ), init_text_( init_text ), check_regex_( check_regex ), have_to_exist_( have_to_exist )
		{}

		bool check()
		{
			std::ifstream f( filename_.c_str() );
			if( f.fail() )
			{
				if( have_to_exist_ )
					THROW_RUNTIME( "Unable to open " << filename_ << " for reading" );
				else
					return false;
			}

			std::string line;
			while( getline( f, line ) )
			{
				if( boost::regex_match( line, boost::regex( check_regex_ ) ) )
					return true;
			}

			return false;
		}

		void init()
		{
			std::ofstream f( filename_.c_str(), std::ios::app );
			if( f.fail() )
				THROW_RUNTIME( "Unable to open " << filename_ << " for writing" );

			f << init_text_;

			if( f.fail() )
				THROW_RUNTIME( "Unable to write initialization text to " << filename_ );
		}

		const std::string& getFilename() const
		{
			return filename_;
		}

	private:
		std::string filename_;
		std::string init_text_;
		std::string check_regex_;
		bool have_to_exist_;
	};

	Initializer( const Config& config )
		: config_( config )
	{
		Element config_file( "./.git/config",
			str( boost::format(
			"[filter \"gitcrypt\"]\n"
			"    smudge = gitcrypt decrypt --key %1%\n"
			"    clean = gitcrypt encrypt --key %1%\n"
			"[diff \"gitcrypt\"]\n"
			"    textconv = gitcrypt decrypt --key %1% --out - --in\n"
			) % config.key ),
			".*\\[(filter|diff) \"gitcrypt\"\\].*", true );
		elements_.push_back( config_file );

		Element attr_file( "./.git/info/attributes",
			"* filter=gitcrypt diff=gitcrypt",
			".*(filter|diff).*=.*gitcrypt.*",
			false );
		elements_.push_back( attr_file );
	}
	
	void operator()()
	{
		BOOST_FOREACH( Element e, elements_ )
		{
			if( e.check() )
				THROW_RUNTIME( "Repositotory is already initialized. Please check " << e.getFilename() << " file." );
		}

		BOOST_FOREACH( Element e, elements_ )
		{
			e.init();
			std::cout << e.getFilename() << " has been modified\n";
		}
	}

private:
	const Config& config_;
	std::vector< Element > elements_;
};

