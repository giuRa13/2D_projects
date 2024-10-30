#pragma once

#include "Box2DWrappers.hpp"
#include "UserData.hpp"


namespace ENGINE_PHYSICS
{

    class ContactListener : public b2ContactListener
    {

    private:
        UserData* m_pUserDataA{nullptr};
        UserData* m_pUserDataB{nullptr};

        void SetUserContacts(UserData* a, UserData* b);


    public:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);

        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

        UserData* GetUserDataA() { return m_pUserDataA; }
        UserData* GetUserDataB() { return m_pUserDataB; }

    };

}