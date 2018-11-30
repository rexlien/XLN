#pragma once

#include <Platform/android/src/main/cpp/JniMethodType.h>

namespace XCr
{
namespace jni
{

    struct ObjectTag { static constexpr auto Name() { return "java/lang/Object"; } };

    template < class TagType >
    struct UntaggedObjectType { using Type = jobject; };

    template < class TheTag >
    class Object;

    template < class TheTag = ObjectTag >
    class Object
    {
        public:
            using TagType = TheTag;


    };

    struct StringTag { static constexpr auto Name() { return "java/lang/String"; } };

    template <>
    struct UntaggedObjectType<StringTag> { using Type = jstring; };


    using String = Object<StringTag>;

}
}