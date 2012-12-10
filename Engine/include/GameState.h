#ifndef GAMESTATE_H
#define GAMESTATE_H


class GameState
{
    public:
        GameState();
        virtual ~GameState();

        void IssueActivateEvent();
        virtual void Run(double frameTime) = 0;

    protected:
    private:
};

#endif // GAMESTATE_H
