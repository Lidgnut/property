#pragma once

#include "Property.h"

#include <array>

namespace mwaack
{
	template<typename PropertyBase = I_Property, typename = std::enable_if_t<std::is_base_of_v<I_Property, PropertyBase>>>
	class I_PropertyHolder
	{
	public:
		virtual ~I_PropertyHolder() = default;


		virtual PropertyBase& fastProp() = 0;

		virtual PropertyBase& slowProp() = 0;

		virtual const PropertyBase& fastProp() const = 0;

		virtual const PropertyBase& slowProp() const = 0;

		virtual const PropertyBase& cfastProp() const = 0;

		virtual const PropertyBase& cslowProp() const = 0;
	};

	template <size_t Buffer, typename PropertyBase = I_Property, typename = std::enable_if_t<std::is_base_of_v<I_Property, PropertyBase>>>
	class PropertyHolder : public I_PropertyHolder<PropertyBase>
	{
		std::array<unsigned char, Buffer> m_buffer;
		PropertyBase* m_fastProperty;
		PropertyBase* m_slowProperty;

	public:
		template<typename T, typename U,
			typename = std::enable_if_t<(sizeof(T) <= Buffer)>,
			typename = std::enable_if_t<std::is_base_of_v<PropertyBase, T>>,
			typename = std::enable_if_t<std::is_base_of_v<PropertyBase, U>>
		>
			PropertyHolder(const T& t, const U& u)
			: m_buffer()
			, m_fastProperty(new((void*)&m_buffer) T(t))
			, m_slowProperty(new U(u))
		{
		}

		template<typename T, typename U,
			typename = std::enable_if_t<(sizeof(T) <= Buffer)>,
			typename = std::enable_if_t<std::is_base_of_v<PropertyBase, T>>,
			typename = std::enable_if_t<std::is_base_of_v<PropertyBase, U>>
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
			, m_fastProperty((PropertyBase*)&m_buffer)
			, m_slowProperty(other.m_slowProperty)
		{
			other.m_slowProperty = nullptr;
		}

		PropertyHolder& operator=(PropertyHolder&& other)
		{
			m_buffer = other.m_buffer;
			m_fastProperty = (PropertyBase*)&m_buffer;

			m_slowProperty = other.m_slowProperty;

			other.m_slowProperty = nullptr;
			return *this;
		}

		virtual ~PropertyHolder()
		{
			if (m_slowProperty != nullptr)
			{
				delete m_slowProperty;
			}
			m_slowProperty = nullptr;
		}


		PropertyBase& fastProp()
		{
			return *m_fastProperty;
		}

		PropertyBase& slowProp()
		{
			return *m_slowProperty;
		}

		const PropertyBase& fastProp() const
		{
			return *m_fastProperty;
		}

		const PropertyBase& slowProp() const
		{
			return *m_slowProperty;
		}

		const PropertyBase& cfastProp() const
		{
			return *m_fastProperty;
		}

		const PropertyBase& cslowProp() const
		{
			return *m_slowProperty;
		}
	};
}