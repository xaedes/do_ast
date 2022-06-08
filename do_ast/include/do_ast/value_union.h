#pragma once

#include <array>
#include <cstdint>
#include <type_traits>
#include <string>

#include <do_ast/auto_padding.h>

namespace do_ast {

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    struct ValueUnion8
    {
        static_assert(TMaxDataSize * 8 >= 8, "TMaxDataSize * 8 >= 8");

        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        using Alignment = std::integral_constant<uint32_t, TAlignment>;
        using MaxDataSize = std::integral_constant<uint32_t, TMaxDataSize>;

        template<class T>
        using ArrayOf = std::enable_if_t< (TMaxDataSize >= sizeof(T)), std::array<T, TMaxDataSize / sizeof(T)> >;
        //using ArrayOf = std::array<T, TMaxDataSize / sizeof(T)>;
        
        union
        {
            ArrayOf<bool       > as_bool;
            ArrayOf<int8_t     > as_int8;
            ArrayOf<uint8_t    > as_uint8;
        };

        static_assert(sizeof(ArrayOf<bool       >) <= TMaxDataSize, "sizeof(ArrayOf<bool       >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int8_t     >) <= TMaxDataSize, "sizeof(ArrayOf<int8_t     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize, "sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize");

        Type type = Type::Void;
        
        using SizeOfPayload = std::integral_constant<uint32_t, sizeof(ArrayOf<uint8_t >) + sizeof(Type)>;

        AutoPadding<SizeOfPayload::value, TAlignment> _padding;

        // uint8_t padding[12];

        ValueUnion8()
        {
            // as_uint8.fill(0);
        }
        ValueUnion8(Type type) : type(type) {}
        ValueUnion8(const ValueUnion8& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnion8& operator=(const ValueUnion8& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnion8 Void    () { return ValueUnion8(); }
        static ValueUnion8 Bool    (bool     value)           { static_assert(sizeof(bool       ) <= TMaxDataSize, "Bool not supported with this MaxDataSize.");    ValueUnion8 result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnion8 Int8    (int8_t   value)           { static_assert(sizeof(int8_t     ) <= TMaxDataSize, "Int8 not supported with this MaxDataSize.");    ValueUnion8 result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnion8 Uint8   (uint8_t  value)           { static_assert(sizeof(uint8_t    ) <= TMaxDataSize, "Uint8 not supported with this MaxDataSize.");   ValueUnion8 result(Type::Uint8);   result.as_uint8[0] = value;    return result;}

    };

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    struct ValueUnion16
    {
        static_assert(TMaxDataSize * 8 >= 16, "TMaxDataSize * 8 >= 16");

        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        using Alignment = std::integral_constant<uint32_t, TAlignment>;
        using MaxDataSize = std::integral_constant<uint32_t, TMaxDataSize>;

        template<class T>
        using ArrayOf = std::enable_if_t< (TMaxDataSize >= sizeof(T)), std::array<T, TMaxDataSize / sizeof(T)> >;
        //using ArrayOf = std::array<T, TMaxDataSize / sizeof(T)>;
        
        union
        {
            ArrayOf<bool       > as_bool;
            ArrayOf<int8_t     > as_int8;
            ArrayOf<uint8_t    > as_uint8;
            ArrayOf<int16_t    > as_int16;
            ArrayOf<uint16_t   > as_uint16;
        };

        static_assert(sizeof(ArrayOf<bool       >) <= TMaxDataSize, "sizeof(ArrayOf<bool       >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int8_t     >) <= TMaxDataSize, "sizeof(ArrayOf<int8_t     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize, "sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int16_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int16_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize");

        Type type = Type::Void;
        
        using SizeOfPayload = std::integral_constant<uint32_t, sizeof(ArrayOf<uint8_t >) + sizeof(Type)>;

        AutoPadding<SizeOfPayload::value, TAlignment> _padding;

        // uint8_t padding[12];

        ValueUnion16()
        {
            // as_uint8.fill(0);
        }
        ValueUnion16(Type type) : type(type) {}
        ValueUnion16(const ValueUnion16& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnion16& operator=(const ValueUnion16& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnion16 Void    () { return ValueUnion16(); }
        static ValueUnion16 Bool    (bool     value)           { static_assert(sizeof(bool       ) <= TMaxDataSize, "Bool not supported with this MaxDataSize.");    ValueUnion16 result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnion16 Int8    (int8_t   value)           { static_assert(sizeof(int8_t     ) <= TMaxDataSize, "Int8 not supported with this MaxDataSize.");    ValueUnion16 result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnion16 Uint8   (uint8_t  value)           { static_assert(sizeof(uint8_t    ) <= TMaxDataSize, "Uint8 not supported with this MaxDataSize.");   ValueUnion16 result(Type::Uint8);   result.as_uint8[0] = value;    return result;}
        static ValueUnion16 Int16   (int16_t  value)           { static_assert(sizeof(int16_t    ) <= TMaxDataSize, "Int16 not supported with this MaxDataSize.");   ValueUnion16 result(Type::Int16);   result.as_int16[0] = value;    return result;}
        static ValueUnion16 Uint16  (uint16_t value)           { static_assert(sizeof(uint16_t   ) <= TMaxDataSize, "Uint16 not supported with this MaxDataSize.");  ValueUnion16 result(Type::Uint16);  result.as_uint16[0] = value;   return result;}

    };

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    struct ValueUnion32
    {
        static_assert(TMaxDataSize * 8 >= 32, "TMaxDataSize * 8 >= 32");

        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        using Alignment = std::integral_constant<uint32_t, TAlignment>;
        using MaxDataSize = std::integral_constant<uint32_t, TMaxDataSize>;

        template<class T>
        using ArrayOf = std::enable_if_t< (TMaxDataSize >= sizeof(T)), std::array<T, TMaxDataSize / sizeof(T)> >;
        //using ArrayOf = std::array<T, TMaxDataSize / sizeof(T)>;
        
        union
        {
            ArrayOf<bool       > as_bool;
            ArrayOf<int8_t     > as_int8;
            ArrayOf<uint8_t    > as_uint8;
            ArrayOf<int16_t    > as_int16;
            ArrayOf<uint16_t   > as_uint16;
            ArrayOf<int32_t    > as_int32;
            ArrayOf<uint32_t   > as_uint32;
            ArrayOf<float      > as_float;
        };

        static_assert(sizeof(ArrayOf<bool       >) <= TMaxDataSize, "sizeof(ArrayOf<bool       >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int8_t     >) <= TMaxDataSize, "sizeof(ArrayOf<int8_t     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize, "sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int16_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int16_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int32_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int32_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<float      >) <= TMaxDataSize, "sizeof(ArrayOf<float      >) <= TMaxDataSize");

        Type type = Type::Void;
        
        using SizeOfPayload = std::integral_constant<uint32_t, sizeof(ArrayOf<uint8_t >) + sizeof(Type)>;

        AutoPadding<SizeOfPayload::value, TAlignment> _padding;

        // uint8_t padding[12];

        ValueUnion32()
        {
            // as_uint8.fill(0);
        }
        ValueUnion32(Type type) : type(type) {}
        ValueUnion32(const ValueUnion32& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnion32& operator=(const ValueUnion32& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnion32 Void    () { return ValueUnion32(); }
        static ValueUnion32 Bool    (bool     value)           { static_assert(sizeof(bool       ) <= TMaxDataSize, "Bool not supported with this MaxDataSize.");    ValueUnion32 result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnion32 Int8    (int8_t   value)           { static_assert(sizeof(int8_t     ) <= TMaxDataSize, "Int8 not supported with this MaxDataSize.");    ValueUnion32 result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnion32 Uint8   (uint8_t  value)           { static_assert(sizeof(uint8_t    ) <= TMaxDataSize, "Uint8 not supported with this MaxDataSize.");   ValueUnion32 result(Type::Uint8);   result.as_uint8[0] = value;    return result;}
        static ValueUnion32 Int16   (int16_t  value)           { static_assert(sizeof(int16_t    ) <= TMaxDataSize, "Int16 not supported with this MaxDataSize.");   ValueUnion32 result(Type::Int16);   result.as_int16[0] = value;    return result;}
        static ValueUnion32 Uint16  (uint16_t value)           { static_assert(sizeof(uint16_t   ) <= TMaxDataSize, "Uint16 not supported with this MaxDataSize.");  ValueUnion32 result(Type::Uint16);  result.as_uint16[0] = value;   return result;}
        static ValueUnion32 Int32   (int32_t  value)           { static_assert(sizeof(int32_t    ) <= TMaxDataSize, "Int32 not supported with this MaxDataSize.");   ValueUnion32 result(Type::Int32);   result.as_int32[0] = value;    return result;}
        static ValueUnion32 Uint32  (uint32_t value)           { static_assert(sizeof(uint32_t   ) <= TMaxDataSize, "Uint32 not supported with this MaxDataSize.");  ValueUnion32 result(Type::Uint32);  result.as_uint32[0] = value;   return result;}
        static ValueUnion32 Float   (float    value)           { static_assert(sizeof(float      ) <= TMaxDataSize, "Float not supported with this MaxDataSize.");   ValueUnion32 result(Type::Float);   result.as_float[0] = value;    return result;}

    };

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    struct ValueUnion64
    {
        static_assert(TMaxDataSize * 8 >= 64, "TMaxDataSize * 8 >= 64");

        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        using Alignment = std::integral_constant<uint32_t, TAlignment>;
        using MaxDataSize = std::integral_constant<uint32_t, TMaxDataSize>;

        // template<class SmallType, class BigType = std::string>
        // using CountSmallInBig = std::integral_constant<
        //     uint32_t, 
        //     sizeof(BigType) / sizeof(SmallType)
        // >;

        template<class T>
        using ArrayOf = std::enable_if_t< (TMaxDataSize >= sizeof(T)), std::array<T, TMaxDataSize / sizeof(T)> >;
        //using ArrayOf = std::array<T, TMaxDataSize / sizeof(T)>;
        
        union
        {
            ArrayOf<void*      > as_void_ptr;
            ArrayOf<bool       > as_bool;
            ArrayOf<int8_t     > as_int8;
            ArrayOf<uint8_t    > as_uint8;
            ArrayOf<int16_t    > as_int16;
            ArrayOf<uint16_t   > as_uint16;
            ArrayOf<int32_t    > as_int32;
            ArrayOf<uint32_t   > as_uint32;
            ArrayOf<int64_t    > as_int64;
            ArrayOf<uint64_t   > as_uint64;
            ArrayOf<float      > as_float;
            ArrayOf<double     > as_double;
        };

        static_assert(sizeof(ArrayOf<void*      >) <= TMaxDataSize, "sizeof(ArrayOf<void*      >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<bool       >) <= TMaxDataSize, "sizeof(ArrayOf<bool       >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int8_t     >) <= TMaxDataSize, "sizeof(ArrayOf<int8_t     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize, "sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int16_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int16_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int32_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int32_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int64_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int64_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint64_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint64_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<float      >) <= TMaxDataSize, "sizeof(ArrayOf<float      >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<double     >) <= TMaxDataSize, "sizeof(ArrayOf<double     >) <= TMaxDataSize");

        Type type = Type::Void;
        
        using SizeOfPayload = std::integral_constant<uint32_t, sizeof(ArrayOf<uint8_t >) + sizeof(Type)>;

        AutoPadding<SizeOfPayload::value, TAlignment> _padding;

        // uint8_t padding[12];

        ValueUnion64()
        {
            // as_uint8.fill(0);
        }
        ValueUnion64(Type type) : type(type) {}
        ValueUnion64(const ValueUnion64& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnion64& operator=(const ValueUnion64& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnion64 Void    () { return ValueUnion64(); }
        static ValueUnion64 VoidPtr (void*    value)           { static_assert(sizeof(void*      ) <= TMaxDataSize, "VoidPtr not supported with this MaxDataSize."); ValueUnion64 result(Type::VoidPtr); result.as_void_ptr[0] = value; return result;}
        static ValueUnion64 Bool    (bool     value)           { static_assert(sizeof(bool       ) <= TMaxDataSize, "Bool not supported with this MaxDataSize.");    ValueUnion64 result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnion64 Int8    (int8_t   value)           { static_assert(sizeof(int8_t     ) <= TMaxDataSize, "Int8 not supported with this MaxDataSize.");    ValueUnion64 result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnion64 Uint8   (uint8_t  value)           { static_assert(sizeof(uint8_t    ) <= TMaxDataSize, "Uint8 not supported with this MaxDataSize.");   ValueUnion64 result(Type::Uint8);   result.as_uint8[0] = value;    return result;}
        static ValueUnion64 Int16   (int16_t  value)           { static_assert(sizeof(int16_t    ) <= TMaxDataSize, "Int16 not supported with this MaxDataSize.");   ValueUnion64 result(Type::Int16);   result.as_int16[0] = value;    return result;}
        static ValueUnion64 Uint16  (uint16_t value)           { static_assert(sizeof(uint16_t   ) <= TMaxDataSize, "Uint16 not supported with this MaxDataSize.");  ValueUnion64 result(Type::Uint16);  result.as_uint16[0] = value;   return result;}
        static ValueUnion64 Int32   (int32_t  value)           { static_assert(sizeof(int32_t    ) <= TMaxDataSize, "Int32 not supported with this MaxDataSize.");   ValueUnion64 result(Type::Int32);   result.as_int32[0] = value;    return result;}
        static ValueUnion64 Uint32  (uint32_t value)           { static_assert(sizeof(uint32_t   ) <= TMaxDataSize, "Uint32 not supported with this MaxDataSize.");  ValueUnion64 result(Type::Uint32);  result.as_uint32[0] = value;   return result;}
        static ValueUnion64 Int64   (int64_t  value)           { static_assert(sizeof(int64_t    ) <= TMaxDataSize, "Int64 not supported with this MaxDataSize.");   ValueUnion64 result(Type::Int64);   result.as_int64[0] = value;    return result;}
        static ValueUnion64 Uint64  (uint64_t value)           { static_assert(sizeof(uint64_t   ) <= TMaxDataSize, "Uint64 not supported with this MaxDataSize.");  ValueUnion64 result(Type::Uint64);  result.as_uint64[0] = value;   return result;}
        static ValueUnion64 Float   (float    value)           { static_assert(sizeof(float      ) <= TMaxDataSize, "Float not supported with this MaxDataSize.");   ValueUnion64 result(Type::Float);   result.as_float[0] = value;    return result;}
        static ValueUnion64 Double  (double   value)           { static_assert(sizeof(double     ) <= TMaxDataSize, "Double not supported with this MaxDataSize.");  ValueUnion64 result(Type::Double);  result.as_double[0] = value;   return result;}

    };

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    struct ValueUnionBig
    {
        static_assert(TMaxDataSize >= sizeof(std::string), "TMaxDataSize >= sizeof(std::string)");

        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        using Alignment = std::integral_constant<uint32_t, TAlignment>;
        using MaxDataSize = std::integral_constant<uint32_t, TMaxDataSize>;

        // template<class SmallType, class BigType = std::string>
        // using CountSmallInBig = std::integral_constant<
        //     uint32_t, 
        //     sizeof(BigType) / sizeof(SmallType)
        // >;

        template<class T>
        using ArrayOf = std::enable_if_t< (TMaxDataSize >= sizeof(T)), std::array<T, TMaxDataSize / sizeof(T)> >;
        //using ArrayOf = std::array<T, TMaxDataSize / sizeof(T)>;
        
        union
        {
            ArrayOf<void*      > as_void_ptr;
            ArrayOf<bool       > as_bool;
            ArrayOf<int8_t     > as_int8;
            ArrayOf<uint8_t    > as_uint8;
            ArrayOf<int16_t    > as_int16;
            ArrayOf<uint16_t   > as_uint16;
            ArrayOf<int32_t    > as_int32;
            ArrayOf<uint32_t   > as_uint32;
            ArrayOf<int64_t    > as_int64;
            ArrayOf<uint64_t   > as_uint64;
            ArrayOf<float      > as_float;
            ArrayOf<double     > as_double;
            ArrayOf<std::string> as_string;
        };

        static_assert(sizeof(ArrayOf<void*      >) <= TMaxDataSize, "sizeof(ArrayOf<void*      >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<bool       >) <= TMaxDataSize, "sizeof(ArrayOf<bool       >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int8_t     >) <= TMaxDataSize, "sizeof(ArrayOf<int8_t     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize, "sizeof(ArrayOf<uint8_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int16_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int16_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint16_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int32_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int32_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint32_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<int64_t    >) <= TMaxDataSize, "sizeof(ArrayOf<int64_t    >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<uint64_t   >) <= TMaxDataSize, "sizeof(ArrayOf<uint64_t   >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<float      >) <= TMaxDataSize, "sizeof(ArrayOf<float      >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<double     >) <= TMaxDataSize, "sizeof(ArrayOf<double     >) <= TMaxDataSize");
        static_assert(sizeof(ArrayOf<std::string>) <= TMaxDataSize, "sizeof(ArrayOf<std::string>) <= TMaxDataSize");

        Type type = Type::Void;
        
        using SizeOfPayload = std::integral_constant<uint32_t, sizeof(ArrayOf<uint8_t >) + sizeof(Type)>;

        AutoPadding<SizeOfPayload::value, TAlignment> _padding;

        // uint8_t padding[12];

        ValueUnionBig()
        {
            // as_uint8.fill(0);
        }
        ValueUnionBig(Type type) : type(type) {}
        ValueUnionBig(const ValueUnionBig& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnionBig& operator=(const ValueUnionBig& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnionBig Void    () { return ValueUnionBig(); }
        static ValueUnionBig VoidPtr (void*    value)           { static_assert(sizeof(void*      ) <= TMaxDataSize, "VoidPtr not supported with this MaxDataSize."); ValueUnionBig result(Type::VoidPtr); result.as_void_ptr[0] = value; return result;}
        static ValueUnionBig Bool    (bool     value)           { static_assert(sizeof(bool       ) <= TMaxDataSize, "Bool not supported with this MaxDataSize.");    ValueUnionBig result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnionBig Int8    (int8_t   value)           { static_assert(sizeof(int8_t     ) <= TMaxDataSize, "Int8 not supported with this MaxDataSize.");    ValueUnionBig result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnionBig Uint8   (uint8_t  value)           { static_assert(sizeof(uint8_t    ) <= TMaxDataSize, "Uint8 not supported with this MaxDataSize.");   ValueUnionBig result(Type::Uint8);   result.as_uint8[0] = value;    return result;}
        static ValueUnionBig Int16   (int16_t  value)           { static_assert(sizeof(int16_t    ) <= TMaxDataSize, "Int16 not supported with this MaxDataSize.");   ValueUnionBig result(Type::Int16);   result.as_int16[0] = value;    return result;}
        static ValueUnionBig Uint16  (uint16_t value)           { static_assert(sizeof(uint16_t   ) <= TMaxDataSize, "Uint16 not supported with this MaxDataSize.");  ValueUnionBig result(Type::Uint16);  result.as_uint16[0] = value;   return result;}
        static ValueUnionBig Int32   (int32_t  value)           { static_assert(sizeof(int32_t    ) <= TMaxDataSize, "Int32 not supported with this MaxDataSize.");   ValueUnionBig result(Type::Int32);   result.as_int32[0] = value;    return result;}
        static ValueUnionBig Uint32  (uint32_t value)           { static_assert(sizeof(uint32_t   ) <= TMaxDataSize, "Uint32 not supported with this MaxDataSize.");  ValueUnionBig result(Type::Uint32);  result.as_uint32[0] = value;   return result;}
        static ValueUnionBig Int64   (int64_t  value)           { static_assert(sizeof(int64_t    ) <= TMaxDataSize, "Int64 not supported with this MaxDataSize.");   ValueUnionBig result(Type::Int64);   result.as_int64[0] = value;    return result;}
        static ValueUnionBig Uint64  (uint64_t value)           { static_assert(sizeof(uint64_t   ) <= TMaxDataSize, "Uint64 not supported with this MaxDataSize.");  ValueUnionBig result(Type::Uint64);  result.as_uint64[0] = value;   return result;}
        static ValueUnionBig Float   (float    value)           { static_assert(sizeof(float      ) <= TMaxDataSize, "Float not supported with this MaxDataSize.");   ValueUnionBig result(Type::Float);   result.as_float[0] = value;    return result;}
        static ValueUnionBig Double  (double   value)           { static_assert(sizeof(double     ) <= TMaxDataSize, "Double not supported with this MaxDataSize.");  ValueUnionBig result(Type::Double);  result.as_double[0] = value;   return result;}
        static ValueUnionBig String  (const std::string& value) { static_assert(sizeof(std::string) <= TMaxDataSize, "String not supported with this MaxDataSize.");  ValueUnionBig result(Type::String);  result.as_string = value;      return result;}
        
        ~ValueUnionBig()
        {
            if (type == Type::String)
            {
                for (auto& s : as_string)
                {
                    s.~basic_string();
                }
            }
        }

    };

    template <
        uint32_t TMaxDataSize = sizeof(std::string), 
        uint32_t TAlignment = 1
    >
    using ValueUnion = std::conditional_t<
        TMaxDataSize >= sizeof(std::string),
        ValueUnionBig<TMaxDataSize, TAlignment>,
        std::conditional_t<
            TMaxDataSize*8 >= 64,
            ValueUnion64<TMaxDataSize, TAlignment>,
            std::conditional_t<
                TMaxDataSize*8 >= 32,
                ValueUnion32<TMaxDataSize, TAlignment>,
                std::conditional_t<
                    TMaxDataSize*8 >= 16,
                    ValueUnion16<TMaxDataSize, TAlignment>,
                    ValueUnion8<TMaxDataSize, TAlignment>
                >
            >
        >
    >;

} // namespace do_ast
