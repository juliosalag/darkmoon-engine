#include "./managers/game.hpp"

int main(){
    try{
        Game game{};
        game.run();
    }
    catch (const std::exception& e){
        std::cerr << "[Exception]: " << e.what() << '\n';
    }
}