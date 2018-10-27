#ifndef GETNAMESTATE_HPP
#define GETNAMESTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"
#include "Core/TextNode.hpp"
#include "Core/SpriteNode.hpp"

#include <SFML/Graphics/Text.hpp>

class GetNameState : public State
{
    std::string chars = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$[]";

public:
    GetNameState( States::ID id, StateStack& stack, Context context );
    ~GetNameState( );

    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    SceneNode mSceneGraph;
    TextNode* letter1;
    TextNode* letter2;
    TextNode* letter3;
    unsigned int letter1Data = 0;
    unsigned int letter2Data = 0;
    unsigned int letter3Data = 0;
    SpriteNode* mMarker;
    int rankPosition;
    bool nameComplete;
    sf::Text       mTitleText;
};

#endif // GETNAMESTATE_HPP
