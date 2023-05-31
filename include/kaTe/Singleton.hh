/**
 * Singleton.hh
 * Created by kate on 5/28/23.
 * */

#ifndef KATE_ENGINE_SINGLETON_HH
#define KATE_ENGINE_SINGLETON_HH

#include <Core/Assert.hh>

namespace kT {
    /**
     * Defines a general interface for classes that require global single instance.
     * @tparam Derived The type that requires a single instance
     * */
    template<typename Derived>
    class Singleton {
    public:
        using Value_T = Derived;
        using ValueRef_T = Derived&;
        using SingletonPtr_T = Value_T*;

    public:
        Singleton() {
            KT_ASSERT(!s_Instance, "Singleton instance already exists!");
            s_Instance = static_cast<SingletonPtr_T>(this);
        }

        /**
         * Return a reference to the single instance
         * @returns single instance
         * */
        static auto get() -> ValueRef_T { static Value_T obj{}; return *s_Instance; }

        /**
         * Return a pointer to the single instance
         * @returns pointer single instance
         * */
        static auto getPtr() -> SingletonPtr_T { return s_Instance; }

    private:
        // Forbidden operations for Singleton
        Singleton(const ValueRef_T) = delete;
        auto operator=(const ValueRef_T) -> ValueRef_T = delete;

    protected:
        virtual ~Singleton() = default;

        inline static SingletonPtr_T s_Instance;
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_SINGLETON_HH
