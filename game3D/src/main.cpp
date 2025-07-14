#include <engine/game.hpp>

int main()
{
    Game game("./map/map.txt"); 
    while (game.run())
    {
        game.processInput(); 
        game.update(); 
        game.render();
    }
    return 0;
}
