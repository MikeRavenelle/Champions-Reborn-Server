#include <algorithm>
#include <cmath>
#include <string>
#include <cstdint>
#include "Common/Platform.hpp"

#include "Common/Utility.hpp"

int32_t Util::round_up( int32_t numToRound, int32_t multiple )
{
	if( multiple == 0 )
		return numToRound;

	int32_t remainder = abs( numToRound ) % multiple;
	if( remainder == 0 )
		return numToRound;

	if( numToRound < 0 )
		return -( abs( numToRound ) - remainder );
	else
		return numToRound + multiple - remainder;
}

int32_t Util::round_down( int32_t numToRound, int32_t multiple )
{
	if( multiple == 0 )
		return numToRound;

	int32_t remainder = abs( numToRound ) % multiple;
	if( remainder == 0 )
		return numToRound;

	if( numToRound < 0 )
		return -( abs( numToRound ) + remainder );
	else
		return numToRound - remainder;
}

uint16_t Util::ByteSwap( uint16_t val )
{
	return ( val << 8 ) | ( val >> 8 );
}

uint32_t Util::ByteSwap( uint32_t val )
{
	return ( ( val << 24 ) & 0xFF000000 ) |
		( ( val << 8 ) & 0x00FF0000 ) |
		( ( val >> 8 ) & 0x0000FF00 ) |
		( ( val >> 24 ) & 0x000000FF );
}

std::string Util::IPFromAddr( const sockaddr_in &addr )
{
	char buffer[ INET_ADDRSTRLEN ] = {};
	const char *result = inet_ntop( AF_INET, &addr.sin_addr, buffer, sizeof( buffer ) );

	if( result )
		return std::string( buffer );
	else
		return {};
}

namespace
{
	void AppendUTF8( std::string &out, uint32_t cp )
	{
		if( cp < 0x80 )
		{
			out += static_cast< char >( cp );
		}
		else if( cp < 0x800 )
		{
			out += static_cast< char >( 0xC0 | ( cp >> 6 ) );
			out += static_cast< char >( 0x80 | ( cp & 0x3F ) );
		}
		else if( cp < 0x10000 )
		{
			out += static_cast< char >( 0xE0 | ( cp >> 12 ) );
			out += static_cast< char >( 0x80 | ( ( cp >> 6 ) & 0x3F ) );
			out += static_cast< char >( 0x80 | ( cp & 0x3F ) );
		}
		else
		{
			out += static_cast< char >( 0xF0 | ( cp >> 18 ) );
			out += static_cast< char >( 0x80 | ( ( cp >> 12 ) & 0x3F ) );
			out += static_cast< char >( 0x80 | ( ( cp >> 6 ) & 0x3F ) );
			out += static_cast< char >( 0x80 | ( cp & 0x3F ) );
		}
	}
}

std::string Util::WideToUTF8( const std::wstring &wstr )
{
	std::string result;
	result.reserve( wstr.size() );

	for( size_t i = 0; i < wstr.size(); ++i )
	{
		uint32_t cp = static_cast< uint32_t >( wstr[ i ] );
		if( sizeof( wchar_t ) == 2 && cp >= 0xD800 && cp < 0xDC00 && i + 1 < wstr.size() )
		{
			const uint32_t low = static_cast< uint32_t >( wstr[ i + 1 ] );
			if( low >= 0xDC00 && low < 0xE000 )
			{
				cp = 0x10000 + ( ( cp - 0xD800 ) << 10 ) + ( low - 0xDC00 );
				++i;
			}
		}
		AppendUTF8( result, cp );
	}

	return result;
}

std::wstring Util::UTF8ToWide( const std::string &str )
{
	std::wstring result;
	result.reserve( str.size() );

	for( size_t i = 0; i < str.size(); )
	{
		const uint8_t lead = static_cast< uint8_t >( str[ i ] );
		const size_t extra = lead >= 0xF0 ? 3 : lead >= 0xE0 ? 2 : lead >= 0xC0 ? 1 : 0;
		uint32_t cp = extra == 0 ? lead : lead & ( 0x3F >> extra );

		if( i + extra >= str.size() + ( extra == 0 ? 1 : 0 ) && extra != 0 )
			break;

		for( size_t k = 1; k <= extra; ++k )
			cp = ( cp << 6 ) | ( static_cast< uint8_t >( str[ i + k ] ) & 0x3F );
		i += extra + 1;

		if( sizeof( wchar_t ) == 2 && cp >= 0x10000 )
		{
			cp -= 0x10000;
			result += static_cast< wchar_t >( 0xD800 + ( cp >> 10 ) );
			result += static_cast< wchar_t >( 0xDC00 + ( cp & 0x3FF ) );
		}
		else
		{
			result += static_cast< wchar_t >( cp );
		}
	}

	return result;
}
