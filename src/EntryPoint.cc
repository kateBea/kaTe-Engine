/**
 * EntryPoint.cc
 * */

#include <kaTe/Application.hh>

int main(int, char**) {
    auto& app { kaTe::Application::get() };

    app.init();
    app.loop();
    app.shutDown();

    return 0;
}