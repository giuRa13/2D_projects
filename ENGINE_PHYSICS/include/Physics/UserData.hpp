#pragma once 

#include <any>
#include <string>
#include <sstream>

/* std::any
is part of the C++ Standard Library's type-safe container classes 
and provides a way to store and manipulate values of any type in a type-safe manner. 
It can be particularly useful when you need to work with heterogeneous collections of objects 
or when the types of objects are not known at compile-time.
*/

namespace ENGINE_PHYSICS
{

    struct UserData
    {
        std::any userData{};
        std::uint32_t type_id{0}; // for the type_hash from entt::meta
    };

    struct ObjectData
    {
        std::string tag{""}, group{""};
        bool bCollider{false}, bTrigger{false};
        std::uint32_t entityID{};

        [[nodiscard]] std::string to_string() const
        {
            std::stringstream ss;
            ss << 
                "==== Object Data ==== \n" << std::boolalpha <<
                "Tag: " << tag << "\n" <<
                "Group: " << group << "\n" <<
                "bCollider: " << bCollider << "\n" <<
                "bTrigger: " << bTrigger << "\n" <<
                "EntityID: " << entityID << "\n" ;

                return ss.str();
        }
    };

}