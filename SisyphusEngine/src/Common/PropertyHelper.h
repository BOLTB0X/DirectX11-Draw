#pragma once
#include <functional>

namespace PropertyHelper
{
    template <typename T>
    class Property
    {
    public:
        using Getter = std::function<T()>;
        using Setter = std::function<void(const T&)>;

        Property(Getter getter, Setter setter)
            : m_getter(getter), m_setter(setter) {}

        T Get() const { return m_getter(); }
        void Set(const T& value) { m_setter(value); }

    private:
        Getter m_getter;
        Setter m_setter;
    }; // Property

} // PropertyHelper