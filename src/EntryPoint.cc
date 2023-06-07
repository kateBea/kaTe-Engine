/**
 * EntryPoint.cc
 * */

#include <Sandbox.hh>

int main(int, char**) {
    auto app { kaTe::Sandbox() };
    auto ret{ app.run() };

    return ret;
}