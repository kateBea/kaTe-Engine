#include <memory>
#include <Application.hh>

int main(int, char**) {
    auto app{ std::make_unique<kT::Application>(kT::Application()) };

    app->init();
    app->loop();
    app->shutdown();

    return 0;
}