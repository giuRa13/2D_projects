#include "Core/ECS/Components/PhysicsComponent.hpp"
#include <Logger/Logger.hpp>


namespace ENGINE_CORE::ECS
{

    PhysicsComponent::PhysicsComponent(ENGINE_PHYSICS::PhysicsWorld pPhysicsWorld, const PhysicsAttributes& physicsAttrib)
        :m_pPhysicsWorld{pPhysicsWorld}, m_pRigidBody{nullptr}, m_InitialAttribs{physicsAttrib}
    {

    }

    void PhysicsComponent::Init(int windowWidth, int windowHeight)
    {
        if(!m_pPhysicsWorld)
        {
            ENGINE_ERROR("Failed to Initialize PhysicsComponent - Physics World idìs nullptr");
            return;
        }

        bool bCircle{m_InitialAttribs.bCircle};

        // create body def
        b2BodyDef bodyDef{};
        bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);

        // set initial position of body
        bodyDef.position.Set(
            (m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f) +
            ((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) * 
                m_InitialAttribs.scale.x ) * 0.5f) * PIXELS_TO_METERS,
            (m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) +
            ((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) * 
                m_InitialAttribs.scale.y ) * 0.5f) * PIXELS_TO_METERS
        );
        bodyDef.gravityScale = m_InitialAttribs.gravityScale;
        bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;

        // Create the Rigid Body
        m_pRigidBody = ENGINE_PHYSICS::MakeSharedBody(m_pPhysicsWorld->CreateBody(&bodyDef));
        if(!m_pRigidBody)
        {
            ENGINE_ERROR("Failed to create the Rigid Body");
            return;
        }

        // Create the Shape
        b2PolygonShape polyShape;
        b2CircleShape circleShape;

        if(bCircle)
        {
            circleShape.m_radius = m_InitialAttribs.radius;
        }
        else if(m_InitialAttribs.bBoxShape)
        {
            polyShape.SetAsBox(
                PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
                PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
            );
        }
        else
        {
            // create polygon shape
        }

        // Create the fixture def
        b2FixtureDef fixtureDef{};
        if(bCircle)
            fixtureDef.shape = &circleShape;
        else
            fixtureDef.shape = &polyShape;

        fixtureDef.density = m_InitialAttribs.density;
        fixtureDef.friction = m_InitialAttribs.friction;
        fixtureDef.restitution = m_InitialAttribs.restituton;
        fixtureDef.restitutionThreshold = m_InitialAttribs.restitutionThreshold;

        auto p_Fixture = m_pRigidBody->CreateFixture(&fixtureDef);
        if(!p_Fixture)
        {
            ENGINE_ERROR("Failed to create the Body Fixture");
        }


    }


    void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
    {

    }

}