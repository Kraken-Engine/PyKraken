#pragma once

#define ENABLE_ENUM_FLAGS(EnumType)                                              \
    inline EnumType operator|(EnumType a, EnumType b)                            \
    {                                                                            \
        return static_cast<EnumType>(static_cast<int>(a) | static_cast<int>(b)); \
    }                                                                            \
    inline EnumType& operator|=(EnumType& a, EnumType b)                         \
    {                                                                            \
        a = a | b;                                                               \
        return a;                                                                \
    }                                                                            \
    inline EnumType operator&(EnumType a, EnumType b)                            \
    {                                                                            \
        return static_cast<EnumType>(static_cast<int>(a) & static_cast<int>(b)); \
    }                                                                            \
    inline EnumType& operator&=(EnumType& a, EnumType b)                         \
    {                                                                            \
        a = a & b;                                                               \
        return a;                                                                \
    }                                                                            \
    inline EnumType operator~(EnumType a)                                        \
    {                                                                            \
        return static_cast<EnumType>(~static_cast<int>(a));                      \
    }                                                                            \
    inline EnumType operator^(EnumType a, EnumType b)                            \
    {                                                                            \
        return static_cast<EnumType>(static_cast<int>(a) ^ static_cast<int>(b)); \
    }                                                                            \
    inline EnumType& operator^=(EnumType& a, EnumType b)                         \
    {                                                                            \
        a = a ^ b;                                                               \
        return a;                                                                \
    }
