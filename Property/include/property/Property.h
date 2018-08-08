#pragma once

#include <type_traits>
#include <memory>

namespace mwaack
{
	struct __declspec(dllexport) I_Property
	{
		virtual ~I_Property() = default;
		
		virtual I_Property* clone() const = 0;

		// Casts <this> to T and applies the newProp via it's assignmend operators
		template<typename T>
		I_Property& copyFrom(const T& newProp)
		{
			T& myself = castTo<std::decay_t<T>>();
			myself = newProp;
			return *this;
		}

		// Casts <this> to T and applies the newProp via it's move operators
		template<typename T>
		I_Property& moveFrom(T&& newProp)
		{
			T& myself = castTo<std::decay_t<T>>();
			myself = std::forward<T>(newProp);
			return *this;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<I_Property, T>>>
		const T& castTo() const
		{
#if _DEBUG
			return dynamic_cast<const T&>(*this);
#else
			return *(reinterpret_cast<const T*>(this));
#endif
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<I_Property, T>>>
		T& castTo()
		{
#if _DEBUG
			return dynamic_cast<T&>(*this);
#else
			return *(reinterpret_cast<T*>(this));
#endif
		}
	};
}