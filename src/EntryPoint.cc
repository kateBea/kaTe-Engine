/**
 * EntryPoint.cc
 * */

#include <Engine.hh>

int main(int, char**) {
    auto app { new kaTe::Engine() };
    auto ret{ app->run() };
    delete app;

    return ret;
}