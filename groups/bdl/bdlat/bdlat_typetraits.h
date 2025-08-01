// bdlat_typetraits.h                                                 -*-C++-*-
#ifndef INCLUDED_BDLAT_TYPETRAITS
#define INCLUDED_BDLAT_TYPETRAITS

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide compile-time traits for generated types.
//
//@CLASSES:
//  bdlat_IsBasicChoice<TYPE>:           true for generated "choice" types
//  bdlat_IsBasicSequence<TYPE>:         true for generated "sequence" types
//  bdlat_IsBasicEnumeration<TYPE>:      true for generated "enumeration" types
//  bdlat_HasFallbackEnumerator<TYPE>:   true for generated "enumeration" types
//                                       that have a "fallback" enumerator
//  bdlat_BasicEnumerationWrapper<TYPE>: characteristics of enumeration type
//  bdlat_IsBasicCustomizedType<TYPE>:   true for generated "customized" types
//  bdlat_UsesDefaultValueFlag<TYPE>:    true for generated "sequence" types
//                                       that use `e_DEFAULT_VALUE` fmt flag
//
//@MACROS:
//  BDLAT_DECL_CHOICE_TRAITS(ClassName)
//  BDLAT_DECL_CHOICE_WITH_ALLOCATOR_TRAITS(ClassName)
//  BDLAT_DECL_CHOICE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)
//  BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName)
//  BDLAT_DECL_SEQUENCE_TRAITS(ClassName)
//  BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_TRAITS(ClassName)
//  BDLAT_DECL_SEQUENCE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)
//  BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName)
//  BDLAT_DECL_ENUMERATION_TRAITS(ClassName)
//  BDLAT_DECL_ENUMERATION_WITH_FALLBACK_TRAITS(ClassName)
//  BDLAT_DECL_CUSTOMIZEDTYPE_TRAITS(ClassName)
//  BDLAT_DECL_CUSTOMIZEDTYPE_WITH_ALLOCATOR_TRAITS(ClassName)
//  BDLAT_DECL_CUSTOMIZEDTYPE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)
//  BDLAT_DECL_CUSTOMIZEDTYPE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName)
//
//@SEE_ALSO:
//
//@DESCRIPTION: This component provides compile-time traits that pertain to
// generated types.  The five traits provided, `bdlat_IsBasicChoice`,
// `bdlat_IsBasicSequence`, `bdlat_IsBasicEnumeration`,
// `bdlat_HasFallbackEnumerator`, `bdlat_IsBasicCustomizedType` are used for
// generated "choice" types, "sequence" types, "enumeration", "enumeration"
// with fallback enumerator, and "customized type" types respectively.
//
///Usage
///-----
// This component is intended to be used by components generated using the
// `bas_codegen.pl` tool.

#include <bdlscm_version.h>

#include <bdlat_bdeatoverrides.h>

#include <bdlb_printmethods.h>      // for 'bdlb::TypeTraitHasPrintMethod'

#include <bslma_usesbslmaallocator.h>

#include <bslmf_isbitwisemoveable.h>
#include <bslmf_nestedtraitdeclaration.h>

#ifndef BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#include <bslalg_typetraits.h>
#endif // BDE_DONT_ALLOW_TRANSITIVE_INCLUDES

namespace BloombergLP {

/// This trait may be declared for "choice" types.
template <class TYPE>
struct bdlat_IsBasicChoice :
        bslmf::DetectNestedTrait<TYPE, bdlat_IsBasicChoice>::type {
};

/// This trait may be declared for "sequence" types.
template <class TYPE>
struct bdlat_IsBasicSequence :
        bslmf::DetectNestedTrait<TYPE, bdlat_IsBasicSequence>::type {
};

/// This trait may be declared for "enumeration" types.
template <class TYPE>
struct bdlat_IsBasicEnumeration
: bslmf::DetectNestedTrait<TYPE, bdlat_IsBasicEnumeration>::type {
};

/// This trait may be declared for "enumeration" types that have a fallback
/// enumerator value.  If this trait is declared to be `true` for the
/// specified `TYPE` but the trait `bdlat_IsBasicEnumeration` has not
/// previously been declared to be `true` for `TYPE`, the behavior is
/// undefined.
template <class TYPE>
struct bdlat_HasFallbackEnumerator
: bslmf::DetectNestedTrait<TYPE, bdlat_HasFallbackEnumerator>::type {
};

/// This trait may be declared for "customized type" types.
template <class TYPE>
struct bdlat_IsBasicCustomizedType :
        bslmf::DetectNestedTrait<TYPE, bdlat_IsBasicCustomizedType>::type {
};

/// This trait is declared for "sequence" types that use the `e_DEFAULT_VALUE`
/// formattig mode flag.
template <class TYPE>
struct bdlat_UsesDefaultValueFlag :
             bslmf::DetectNestedTrait<TYPE, bdlat_UsesDefaultValueFlag>::type {
};

/// This template has no definition for most types.  However for types that
/// meet the requirements of `BasicEnumeration`, this template is
/// specialized to inherit from the wrapper class that defines functions
/// like `toString` and `toInt`.  The specialization should also define the
/// type `Wrapper` to be the name of the wrapper class.
template <class TYPE>
struct bdlat_BasicEnumerationWrapper;

/// Legacy adaptor for bdlat_IsBasicChoice.
struct bdlat_TypeTraitBasicChoice {

    /// This class template ties the `bdlat_TypeTraitBasicChoice` trait tag
    /// to the `bdlat_IsBasicChoice` trait metafunction.
    template <class TYPE>
    struct NestedTraitDeclaration :
        bslmf::NestedTraitDeclaration<TYPE, bdlat_IsBasicChoice>
    {
    };

    template <class TYPE>
    struct Metafunction : bdlat_IsBasicChoice<TYPE>::type { };
};

/// Legacy adaptor for bdlat_IsBasicSequence.
struct bdlat_TypeTraitBasicSequence {

    /// This class template ties the `bdlat_TypeTraitBasicSequence` trait
    /// tag to the `bdlat_IsBasicSequence` trait metafunction.
    template <class TYPE>
    struct NestedTraitDeclaration :
        bslmf::NestedTraitDeclaration<TYPE, bdlat_IsBasicSequence>
    {
    };

    template <class TYPE>
    struct Metafunction : bdlat_IsBasicSequence<TYPE>::type { };
};

/// Legacy adaptor for bdlat_IsBasicEnumeration.
struct bdlat_TypeTraitBasicEnumeration {

    /// This class template ties the `bdlat_TypeTraitBasicEnumeration` trait
    /// tag to the `bdlat_IsBasicEnumeration` trait metafunction.
    template <class TYPE>
    struct NestedTraitDeclaration :
        bslmf::NestedTraitDeclaration<TYPE, bdlat_IsBasicEnumeration>
    {
    };

    template <class TYPE>
    struct Metafunction : bdlat_IsBasicEnumeration<TYPE>::type { };
};

/// Legacy adaptor for bdlat_HasFallbackEnumerator.
struct bdlat_TypeTraitHasFallbackEnumerator {

    /// This class template ties the `bdlat_TypeTraitHasFallbackEnumerator`
    /// trait tag to the `bdlat_HasFallbackEnumerator` trait metafunction.
    template <class TYPE>
    struct NestedTraitDeclaration
    : bslmf::NestedTraitDeclaration<TYPE,
                                    bdlat_HasFallbackEnumerator> {
    };

    template <class TYPE>
    struct Metafunction : bdlat_HasFallbackEnumerator<TYPE>::type { };
};

/// Legacy adaptor for bdlat_IsBasicCustomizedType.
struct bdlat_TypeTraitBasicCustomizedType {

    /// This class template ties the `bdlat_TypeTraitBasicCustomizedType`
    /// trait tag to the `bdlat_IsBasicCustomizedType` trait metafunction.
    template <class TYPE>
    struct NestedTraitDeclaration :
        bslmf::NestedTraitDeclaration<TYPE, bdlat_IsBasicCustomizedType>
    {
    };

    template <class TYPE>
    struct Metafunction : bdlat_IsBasicCustomizedType<TYPE>::type { };
};

// Macros defined to facilitate declaring traits for generated code.

// BDE_VERIFY pragma: -SLM01  // leaking macros is the whole point!

#define BDLAT_DECL_CHOICE_TRAITS(ClassName)                                   \
    template <>                                                               \
    struct bdlat_IsBasicChoice<ClassName> : bsl::true_type { };               \
    namespace bdlb {                                                          \
        template <>                                                           \
        struct HasPrintMethod<ClassName> : bsl::true_type { };                \
    }

#define BDLAT_DECL_CHOICE_WITH_ALLOCATOR_TRAITS(ClassName)                    \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    BDLAT_DECL_CHOICE_TRAITS(ClassName)

#define BDLAT_DECL_CHOICE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)              \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_CHOICE_TRAITS(ClassName)

#define BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName)    \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_CHOICE_TRAITS(ClassName)

#define BDLAT_DECL_SEQUENCE_TRAITS(ClassName)                                 \
    template <>                                                               \
    struct bdlat_IsBasicSequence<ClassName> : bsl::true_type { };             \
    namespace bdlb {                                                          \
        template <>                                                           \
        struct HasPrintMethod<ClassName> : bsl::true_type { };                \
    }

#define BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_TRAITS(ClassName)                  \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    BDLAT_DECL_SEQUENCE_TRAITS(ClassName)

#define BDLAT_DECL_SEQUENCE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)            \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_SEQUENCE_TRAITS(ClassName)

#define BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName)  \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_SEQUENCE_TRAITS(ClassName)

#define BDLAT_DECL_ENUMERATION_TRAITS(ClassName)                              \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName::Value> : bsl::true_type { };      \
    }                                                                         \
    template <>                                                               \
    struct bdlat_IsBasicEnumeration<ClassName::Value> : bsl::true_type { };   \
    template <>                                                               \
    struct bdlat_BasicEnumerationWrapper<ClassName::Value> : ClassName {      \
        typedef ClassName Wrapper;                                            \
    };

#define BDLAT_DECL_ENUMERATION_WITH_FALLBACK_TRAITS(ClassName)                \
    BDLAT_DECL_ENUMERATION_TRAITS(ClassName)                                  \
    template <>                                                               \
    struct bdlat_HasFallbackEnumerator<ClassName::Value> : bsl::true_type { };

#define BDLAT_DECL_CUSTOMIZEDTYPE_TRAITS(ClassName)                           \
    template <>                                                               \
    struct bdlat_IsBasicCustomizedType<ClassName> : bsl::true_type { };       \
    namespace bdlb {                                                          \
        template <>                                                           \
        struct HasPrintMethod<ClassName> : bsl::true_type { };                \
    }

#define BDLAT_DECL_CUSTOMIZEDTYPE_WITH_ALLOCATOR_TRAITS(ClassName)            \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    BDLAT_DECL_CUSTOMIZEDTYPE_TRAITS(ClassName)

#define BDLAT_DECL_CUSTOMIZEDTYPE_WITH_BITWISEMOVEABLE_TRAITS(ClassName)      \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_CUSTOMIZEDTYPE_TRAITS(ClassName)

#define                                                                       \
   BDLAT_DECL_CUSTOMIZEDTYPE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(ClassName) \
    namespace bslma {                                                         \
        template <>                                                           \
        struct UsesBslmaAllocator<ClassName> : bsl::true_type { };            \
    }                                                                         \
    namespace bslmf {                                                         \
        template <>                                                           \
        struct IsBitwiseMoveable<ClassName> : bsl::true_type { };             \
    }                                                                         \
    BDLAT_DECL_CUSTOMIZEDTYPE_TRAITS(ClassName)

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
