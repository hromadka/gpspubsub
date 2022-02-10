

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from gpspubsub.idl
using RTI Code Generator (rtiddsgen) version 3.1.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef gpspubsub_794216703_hpp
#define gpspubsub_794216703_hpp

#include <iosfwd>

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport __declspec(dllexport)
#endif

#include "dds/domain/DomainParticipant.hpp"
#include "dds/topic/TopicTraits.hpp"
#include "dds/core/SafeEnumeration.hpp"
#include "dds/core/String.hpp"
#include "dds/core/array.hpp"
#include "dds/core/vector.hpp"
#include "dds/core/Optional.hpp"
#include "dds/core/xtypes/DynamicType.hpp"
#include "dds/core/xtypes/StructType.hpp"
#include "dds/core/xtypes/UnionType.hpp"
#include "dds/core/xtypes/EnumType.hpp"
#include "dds/core/xtypes/AliasType.hpp"
#include "rti/core/array.hpp"
#include "rti/core/BoundedSequence.hpp"
#include "rti/util/StreamFlagSaver.hpp"
#include "rti/domain/PluginSupport.hpp"
#include "rti/core/LongDouble.hpp"
#include "dds/core/External.hpp"
#include "rti/core/Pointer.hpp"
#include "rti/topic/TopicTraits.hpp"

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

class NDDSUSERDllExport Position {
  public:
    Position();

    Position(
        int32_t providerID,
        float lat,
        float lon);

    #ifdef RTI_CXX11_RVALUE_REFERENCES
    #ifndef RTI_CXX11_NO_IMPLICIT_MOVE_OPERATIONS
    Position (Position&&) = default;
    Position& operator=(Position&&) = default;
    Position& operator=(const Position&) = default;
    Position(const Position&) = default;
    #else
    Position(Position&& other_) OMG_NOEXCEPT;  
    Position& operator=(Position&&  other_) OMG_NOEXCEPT;
    #endif
    #endif 

    int32_t& providerID() OMG_NOEXCEPT {
        return m_providerID_;
    }

    const int32_t& providerID() const OMG_NOEXCEPT {
        return m_providerID_;
    }

    void providerID(int32_t value) {
        m_providerID_ = value;
    }

    float& lat() OMG_NOEXCEPT {
        return m_lat_;
    }

    const float& lat() const OMG_NOEXCEPT {
        return m_lat_;
    }

    void lat(float value) {
        m_lat_ = value;
    }

    float& lon() OMG_NOEXCEPT {
        return m_lon_;
    }

    const float& lon() const OMG_NOEXCEPT {
        return m_lon_;
    }

    void lon(float value) {
        m_lon_ = value;
    }

    bool operator == (const Position& other_) const;
    bool operator != (const Position& other_) const;

    void swap(Position& other_) OMG_NOEXCEPT ;

  private:

    int32_t m_providerID_;
    float m_lat_;
    float m_lon_;

};

inline void swap(Position& a, Position& b)  OMG_NOEXCEPT 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const Position& sample);

namespace rti {
    namespace flat {
        namespace topic {
        }
    }
}
namespace dds {
    namespace topic {

        template<>
        struct topic_type_name< Position > {
            NDDSUSERDllExport static std::string value() {
                return "Position";
            }
        };

        template<>
        struct is_topic_type< Position > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< Position > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const Position& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(Position& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(Position& sample);

            NDDSUSERDllExport 
            static void allocate_sample(Position& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };

    }
}

namespace rti { 
    namespace topic {
        #ifndef NDDS_STANDALONE_TYPE
        template<>
        struct dynamic_type< Position > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };
        #endif

        template <>
        struct extensibility< Position > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;                
        };

    }
}

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif // gpspubsub_794216703_hpp

