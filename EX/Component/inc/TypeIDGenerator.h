#pragma once

class TypeIDGenerator
{
public:
    template <typename T>
    static size_t GetTypeID()
    {
        static const size_t id = typeid(T).hash_code();
        return id;
    }
};