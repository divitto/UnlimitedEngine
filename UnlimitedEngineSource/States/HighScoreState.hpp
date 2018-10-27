#ifndef HIGHSCORESTATE_HPP
#define HIGHSCORESTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"
#include "Core/TextNode.hpp"
#include "Core/SpriteNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class HighScoreState : public State
{
public:
    HighScoreState( States::ID id, StateStack& stack, Context context );
    ~HighScoreState( );

    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

    void saveHighScoresToFile( void );

private:
    SceneNode                                        mSceneGraph;
    GUI::Container                                   mGUIContainer;
    std::vector<std::pair<std::string, int>>         mHighScores;

    sf::Text                                         mText;
    sf::Sprite                                       mBgSprite;
    sf::RectangleShape                               mBackgroundRect;
};

#endif // HIGHSCORESTATE_HPP
