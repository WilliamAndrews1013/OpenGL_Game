#include "core/Application.hpp"

class GameApp : public Core::Application {
public:
    GameApp() {
        // Game specific initialization can go here
    }
    ~GameApp() {
    }
};

namespace Core {
    Application* CreateApplication() {
        return new GameApp();
    }
}

int main() {
    auto app = Core::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
