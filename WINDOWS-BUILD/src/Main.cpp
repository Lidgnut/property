#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "inckude/Property.h"
#include "inckude/PropertyHolder.h"

namespace mwaack
{
	struct Property : public I_Property
	{
		using I_Property::castTo;
		
		Property() = default;
		Property(const Property&) = default;
		Property& operator=(const Property&) = default;
		Property(Property&&) = default;
		Property& operator=(Property&&) = default;
		virtual ~Property() = default;

		int a = 0;
		std::array<uint8_t, 5> arr;
	};
}

int main()
{

	mwaack::PropertyHolder<20> holder{ mwaack::Property(), mwaack::Property() };
	
	std::cout << holder.cfastProp().castTo<mwaack::Property>().a << '\n';
	std::cout << holder.cslowProp().castTo<mwaack::Property>().a << '\n';

	mwaack::Property property;
	mwaack::I_Property& prop = property;

	std::cout << prop.castTo<mwaack::Property>().a << '\n';

	prop.copyFrom(mwaack::Property());

	std::cout << prop.castTo<mwaack::Property>().a << '\n';


	std::puts("Hello world!\n");
	std::system("pause");
	return 0;
}