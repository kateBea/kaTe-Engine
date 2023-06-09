/**
 * EntryPoint.cc
 * */

#include <Sandbox.hh>

int main(int, char**) {
    auto app { new kaTe::Sandbox() };
    auto ret{ app->run() };
    delete app;

    return ret;
}