#ifndef STATEIDENTIFIERS_HPP
#define STATEIDENTIFIERS_HPP

namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
        Game,
        Pause,
        Settings,
        GameOver,
        Loading,
        HighScoreState,
        GetNameState,
        StartLevelState
	};
}

#endif // STATEIDENTIFIERS_HPP
