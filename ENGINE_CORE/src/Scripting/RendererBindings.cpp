#include "Core/Scripting/RendererBindings.hpp"
#include <Rendering/Essentials/Primitives.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Core/Renderer.hpp>
#include "Core/Resources/AssetManager.hpp"
#include "Core/ECS/Registry.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_RENDERING;
using namespace ENGINE_RESOURCES;


void ENGINE_CORE::Scripting::RendererBinder::CreateRenderingBind(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
{
    auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();

    lua.new_usertype<Line>( 
        "Line",
        sol::call_constructor,
        sol::factories( 
            []( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) 
            {
                return Line{ .p1 = p1, .p2 = p2, .color = color };
            }
        ),
        "p1", &Line::p1,
        "p2", &Line::p2,
        "color", &Line::color 
    );

    lua.new_usertype<Rect>(
        "Rect",
        sol::call_constructor,
        sol::factories( 
            []( const glm::vec2& position, float width, float height, const Color& color ) 
            {
                return Rect{ .position = position, .width = width, .height = height, .color = color };
            }
        ),
        "position", &Rect::position,
        "width", &Rect::width,
        "height", &Rect::height,
        "color", &Rect::color 
    );

	lua.new_usertype<Circle>(
		"Circle",
		sol::call_constructor,
		sol::factories( 
            []( const glm::vec2& position, float lineThickness, float radius, const Color& color ) 
            {
			    return Circle{ .position = position, .lineThickness = lineThickness, .radius = radius, .color = color };
		    } 
        ),
		"position", &Circle::position,
		"lineThickness", &Circle::lineThickness,
		"radius", &Circle::radius,
		"color", &Circle::color );

    lua.new_usertype<Text>(
        "Text",
        sol::call_constructor,
        sol::factories(
            [&]( const glm::vec2& position, const std::string& textStr, const std::string& fontName, float wrap, const Color& color )
            {
                auto pFont = assetManager->GetFont(fontName);
                if(!pFont)
                {
                    ENGINE_ERROR("Failed to get Font [{}] -- does not exsists in Asset Manager", fontName);
                    return Text{};
                }

                return Text{
                    .position = position,
                    .textStr = textStr,
                    .wrap = wrap,
                    .pFont = pFont,
                    .color = color
                };
            }
        ),
        "position", &Text::position,
        "textStr", &Text::textStr,
        "wrap", &Text::wrap,
        "color", &Text::color
    );

    
    // Renderer
    auto& renderer = registry.GetContext<std::shared_ptr<Renderer>>();

    if(!renderer)
    {
        ENGINE_ERROR("Failed to bind the Renderer to Lua, not in the Registry context");
    }

    lua.set_function( 
        "DrawRect",
		sol::overload( 
            [ & ]( const Rect& rect ) { 
                renderer->DrawRect( rect ); 
            },
			[ & ]( const glm::vec2& position, float width, float height, const Color& color ) {
				renderer->DrawRect( position, width, height, color );
			} 
        ) 
    );

	lua.set_function( 
        "DrawLine",
		sol::overload( 
            [ & ]( const Line& line ) { 
                renderer->DrawLine( line );
            },
			[ & ]( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) {
				renderer->DrawLine( p1, p2, color );
			} 
        ) 
    );

	lua.set_function(
		"DrawCircle",
		sol::overload( 
            [ & ]( const Circle& circle ) { 
                renderer->DrawCircle( circle ); 
            },
			[ & ]( const glm::vec2& pos, float lineThickness, float radius, const Color& color ) {
				renderer->DrawCircle( pos, radius, color, lineThickness );
			} 
        ) 
    );

	lua.set_function( "DrawFilledRect", 
        [ & ]( const Rect& rect ) { 
            renderer->DrawFilledRect( rect ); 
        } 
    );

    lua.set_function("DrawText",
        [&](const Text& text) {
            renderer->DrawText2D(text);
        }
    );


    // Camera
    auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();

	if ( !camera )
	{
		ENGINE_ERROR( "Failed to Bind the Camera to Lua - Failed to get the Camera from the Registry Context!" );
		return;
	}

	lua.new_usertype<Camera2D>(
		"Camera",
		sol::no_constructor,
		"get", 
        [ & ] { return *camera; },
		"position", 
        [ & ] { return camera->GetPosition(); },
		"set_position", 
        [ & ]( const glm::vec2 newPosition ) { camera->SetPosition( newPosition ); },
		"scale", 
        [ & ] { return camera->GetScale(); },
		"set_scale", 
        [ & ]( float scale ) { camera->SetScale( scale ); },
        "get_world_coords",
        [&] (const glm::vec2& screenCoords){ return camera->ScreenCoordsToWorld(screenCoords); },
        "get_screen_coords",
        [&] (const glm::vec2& worldCoords){ return camera->WorldCoordsToScreen(worldCoords); },
        "width", 
        [&] { return camera->GetWidth(); },
        "height", 
        [&] { return camera->GetHeight(); }
    );

}