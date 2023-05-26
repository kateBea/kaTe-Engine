//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_APPLICATION_HH
#define KATE_ENGINE_APPLICATION_HH


namespace kT {
    class Application {
    public:
        Application() = default;
        ~Application() = default;

        auto init() -> void;
        auto loop() -> void;
        auto shutdown() -> void;
    private:
        // Helper functions

        // Member variables
    };
}


#endif //KATE_ENGINE_APPLICATION_HH
