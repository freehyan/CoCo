#pragma once

namespace CoCo
{
	class CNonCopyable
	{
	public:
		CNonCopyable() = delete;
		CNonCopyable(const CNonCopyable&) = delete;
		CNonCopyable& operator = (const CNonCopyable&) = delete;
	};
}