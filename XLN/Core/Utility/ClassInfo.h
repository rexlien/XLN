#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/UniqueID.h>
#include <XLN/Core/Utility/RTTI.h>

namespace XCr
{

class ClassInfo
{
    //public:
        //static bool IsKindof(UniqueID)
    
};


template<class T>
struct TypeToString
{
    static const char* value() {};
};

}


#define DEF_TYPESTRING(x) \
template<> \
struct XCr::TypeToString<x> \
{\
    static const char* value() {return #x; }; \
}; \
template<> \
struct XCr::TypeToString<x *> \
{\
static const char* value() {  static XCr::String ret = XCr::String(#x) + XCr::String("*"); return ret.c_str(); }; \
}; 

#define DEF_TEMPLATE_TYPESTRING(x, param1, param2) \
template<> \
struct XCr::TypeToString<x##<param1##, param2##> >\
{\
    static const char* value() { static XCr::String ret = XCr::String(#x) + XCr::String(#param1) + XCr::String(#param2); return ret.c_str(); }; \
}; 

//XCr::String ret = XCr::String(#x) + XCr::String(#param1); return ret.c_str(); }; \


#define XLN_DECLARE_BASE_STREMABLE_MSG_CLASS(baseClass) \
private: \
    static XCr::String baseClass##Name; \
    static XCr::Util::UniqueID baseClass##UID; \
public: \
       static XCr::String& ClassName(); \
       static XCr::IMessage* Create(); \
       static const XCr::Util::UniqueID& ClassID(); \
       virtual const XCr::Util::UniqueID& ObjClassID() const {return baseClass##UID;};

#define XLN_IMPL_BASE_NAMESPACED_STREMABLE_MSG_CLASS(namespace, baseClass) \
    XCr::String baseClass::baseClass##Name = XCr::String(#namespace) + XCr::String(#baseClass); \
    XCr::Util::UniqueID baseClass::baseClass##UID = XCr::Util::UniqueID::GenStrUID(baseClass::baseClass##Name.c_str()); \
    XCr::String& baseClass::ClassName() {return baseClass##Name;}; \
    XCr::IMessage* baseClass::Create() {return XLN_OBJ_NEW baseClass();}; \
    XCr::MessageCretorRegister< baseClass > baseClass##ClassRegister; \
    const XCr::Util::UniqueID& baseClass::ClassID() {return baseClass##UID;};

#define XLN_DECLARE_BASE_MSG_CLASS(baseClass) \
private: \
    static XCr::String baseClass##Name; \
    static XCr::Util::UniqueID baseClass##UID; \
public: \
    static XCr::String& ClassName(); \
    static const XCr::Util::UniqueID& ClassID(); \
    virtual const XCr::Util::UniqueID& ObjClassID() const {return baseClass##UID;};

#define XLN_IMPL_BASE_NAMESPACED_MSG_CLASS(namespace, baseClass) \
    XCr::String baseClass::baseClass##Name = XCr::String(#namespace) + XCr::String(#baseClass); \
    XCr::Util::UniqueID baseClass::baseClass##UID = XCr::Util::UniqueID::GenStrUID(baseClass::baseClass##Name.c_str()); \
    XCr::String& baseClass::ClassName() {return baseClass##Name;}; \
    const XCr::Util::UniqueID& baseClass::ClassID() {return baseClass##UID;};
   

/*
#define XLN_IMPL_BASE_NAMESPACED_TEMPLATE_CLASS(namespace, classNameTemplate, baseClass) \
    XCr::String classNameTemplate##::baseClass##Name = XCr::String(#namespace) + XCr::String(#classNameTemplate); \
    XCr::Util::UniqueID classNameTemplate##::baseClass##UID = XCr::Util::UniqueID::GenStrUID(classNameTemplate##::baseClass##Name.c_str()); \
    XCr::String& classNameTemplate##::ClassName() {return baseClass##Name;}; \
    const XCr::Util::UniqueID& classNameTemplate##::ClassID() {return baseClass##UID;};
*/
/*
#define XLN_DECLARE_RTTI(cls) XLN_DECLARE_BASE_MSG_CLASS(cls)
#define XLN_IMPL_RTTI(namespace, cls) XLN_IMPL_BASE_NAMESPACED_MSG_CLASS(namespace, cls);
*/
#define XLN_IMPL_BASE_MSG_CLASS(baseClass) XLN_IMPL_BASE_NAMESPACED_CLASS(::, baseClass)
//#define XLN_IS_EXACT_KIND_OF(className, objA) (obj->ObjClassID() == className##::ClassID())
//#define XLN_CLASS_IS_SAME(objA, objB) objA->GetObjClassID() ==  objB->GetObjectClassID()
