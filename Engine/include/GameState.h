#ifndef GAMESTATE_H
#define GAMESTATE_H


class GameState
{
    public:
        GameState();
        virtual ~GameState();

        void IssueActivateEvent();
        void Run(double frameTime);

    protected:
    private:
};

#endif // GAMESTATE_H
