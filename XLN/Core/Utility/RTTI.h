#ifndef RTTI_H
#define RTTI_H

#include <XLN/Core/Config/Config.h>


namespace XCr
{

class XLN_CLASS_DECL RTTI : public XCr::MemObj
{
public:
    RTTI (const char* pcName, const RTTI* pkBaseRTTI);

    inline const char* GetName() const {return m_Name;}
    inline const RTTI* GetBaseRTTI() const {return m_BaseRTTI;}

    bool CopyName(char* acNameBuffer, unsigned int uiMaxSize) const;
    //bool operator <(const RTTI& rhs) const;
    //bool operator ==(const RTTI& rhs) const;
    

protected:
    const char* m_Name;
    const RTTI* m_BaseRTTI;
};
}

// insert in root class declaration
#define XLN_DECLARE_ROOT_RTTI(classname) \
   public: \
        static const XCr::RTTI ms_RTTI; \
        virtual const XCr::RTTI* GetRTTI() const {return &ms_RTTI;} \
        static bool IsExactKindOf(const XCr::RTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsExactKindOf(pkRTTI); \
        } \
        bool IsExactKindOf(const XCr::RTTI* pkRTTI) const \
        { \
            return (GetRTTI() == pkRTTI); \
        } \
        static bool IsKindOf(const XCr::RTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsKindOf(pkRTTI); \
        } \
        bool IsKindOf(const XCr::RTTI* pkRTTI) const \
        { \
            const XCr::RTTI* pkTmp = GetRTTI(); \
            while (pkTmp) \
            { \
                if (pkTmp == pkRTTI) \
                { \
                    return true; \
                } \
                pkTmp = pkTmp->GetBaseRTTI(); \
            } \
            return false; \
        } \
        static classname* VerifyStaticCastDebug(const XCr::RTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return NULL; \
            } \
            classname* pkDynamicCast = DynamicCast(pkRTTI, pkObject); \
            return pkDynamicCast; \
        } \
        static classname* DynamicCast(const XCr::RTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return NULL; \
            } \
            return pkObject->DynamicCast(pkRTTI); \
        } \
        classname* DynamicCast(const XCr::RTTI* pkRTTI) const \
        { \
            return (IsKindOf(pkRTTI) ? (classname*) this : 0 ); \
        }

// insert in class declaration
#define XLN_DECLARE_RTTI \
    public: \
    static const XCr::RTTI ms_RTTI; \
    virtual const XCr::RTTI* GetRTTI() const override {return &ms_RTTI;}

// insert in root class source file
#define XLN_IMPLEMENT_ROOT_RTTI(rootclassname) \
    const XCr::RTTI rootclassname::ms_RTTI(#rootclassname, 0)

// insert in class source file
#define XLN_IMPLEMENT_RTTI(classname, baseclassname) \
    const XCr::RTTI classname::ms_RTTI(#classname, &baseclassname::ms_RTTI)

// macros for run-time type testing
#define XLN_IS_EXACT_KIND_OF(classname, pkObject) \
    classname::IsExactKindOf(&classname::ms_RTTI, pkObject)

#define XLN_ISKIND_OF(classname, pkObject) \
    classname::IsKindOf(&classname::ms_RTTI, pkObject)

// macro for compile time type casting
#define XLN_STATIC_CAST(classname, pkObject) \
    ((classname*) pkObject)

// macro for compile time type casting, with debug run-time assert
#ifdef XLN_DEBUG
#define XLN_VERIFY_STATIC_CAST(classname, pkObject) \
    ((classname*) classname::VerifyStaticCastDebug( \
    &classname::ms_RTTI, pkObject))
#else
#define XLN_VERIFY_STATIC_CAST(classname, pkObject) ((classname*) (pkObject))
#endif

// macro for run-time type casting, returns NULL if invalid cast
#define XLN_DYNAMIC_CAST(classname, pkObject) \
    ((classname*) classname::DynamicCast(&classname::ms_RTTI, pkObject))

#endif

