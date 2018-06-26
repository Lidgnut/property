#pragma once

#include "Property.h"

#include <array>

namespace mwaack
{
	template <size_t Buffer>
	class PropertyHolder
	{
		std::array<unsigned char, Buffer> m_buffer;
		I_Property* m_fastProperty;
		I_Property* m_slowProperty;

	public:
		template<typename T, typename U,
			typename = std::enable_if_t<(sizeof(T) - sizeof(void*) <= Buffer - sizeof(unsigned long long))>,
			typename = std::enable_if_t<std::is_base_of_v<I_Property, T>>,
			typename = std::enable_if_t<std::is_base_of_v<I_Property, U>>
		>
			PropertyHolder(const T& t, const U& u)
			: m_buffer()
			, m_fastProperty(new((void*)&m_buffer) T(t))
			, m_slowProperty(new U(u))
		{
		}

		template<typename T, typename U,
			typename = std::enable_if_t<(sizeof(T) - sizeof(void*) <= Buffer - sizeof(unsigned long long))>,
			typename = std::enable_if_t<std::is_base_of_v<I_Property, T>>,
			typename = std::enable_if_t<std::is_base_of_v<I_Property, U>>
		>
			PropertyHolder(T&& t, U&& u)
			: m_buffer()
			, m_fastProperty(new((void*)&m_buffer) T(std::forward<T>(t)))
			, m_slowProperty(new U(std::forward<U>(u)))
		{
		}
		
		PropertyHolder& operator=(const PropertyHolder&) = delete;
		PropertyHolder(const PropertyHolder&) = delete;

		PropertyHolder(PropertyHolder&& other)
			: m_buffer(other.m_buffer)
			, m_fastProperty((I_Property*)&m_buffer)
			, m_slowProperty(other.m_slowProperty)
		{
			other.m_slowProperty = nullptr;
		}

		PropertyHolder& operator=(PropertyHolder&& other)
		{
			m_buffer = other.m_buffer;
			m_fastProperty = (I_Property*)&m_buffer;

			m_slowProperty = other.m_slowProperty;

			other.m_slowProperty = nullptr;
			return *this;
		}

		~PropertyHolder()
		{
			if (m_slowProperty != nullptr)
			{
				delete m_slowProperty;
			}
			m_slowProperty = nullptr;
		}


		I_Property& fastProp()
		{
			return *m_fastProperty;
		}

		I_Property& slowProp()
		{
			return *m_slowProperty;
		}

		const I_Property& fastProp() const
		{
			return *m_fastProperty;
		}

		const I_Property& slowProp() const
		{
			return *m_slowProperty;
		}

		const I_Property& cfastProp() const
		{
			return *m_fastProperty;
		}

		const I_Property& cslowProp() const
		{
			return *m_slowProperty;
		}
	};
}