#pragma once

namespace CoCo
{
	class CNonCopyable
	{
	public:
		CNonCopyable() = default;
		CNonCopyable(const CNonCopyable&) = delete;
		CNonCopyable& operator = (const CNonCopyable&) = delete;
	};
}