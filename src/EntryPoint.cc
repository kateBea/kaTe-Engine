/**
 * EntryPoint.cc
 * */

#include <kaTe/Application.hh>

int main(int, char**) {
    auto& app { kT::Application::get() };

    app.init();
    app.loop();
    app.shutDown();

    return 0;
}