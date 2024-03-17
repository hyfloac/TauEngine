#pragma once

#include <utility>
#include <memory>

template<typename _T>
class DependencyInjector final
{
private:
    using T = ::std::remove_pointer_t<::std::remove_reference_t<_T>>;

    using buildScoped_f = ::std::shared_ptr<T>(*)();

    static ::std::shared_ptr<T> staticData;
    static buildScoped_f scopedBuilder;
public:
    template<typename... _Args>
    static void RegisterStatic(_Args&&... args)
    {
        staticData = ::std::make_shared<T>(::std::forward<_Args>(args)...);
        scopedBuilder = nullptr;
    }

    static void RegisterScoped()
    {
        if(staticData)
        {
            staticData = nullptr;
        }

        scopedBuilder = ::std::make_shared<_T>;
    }

    static ::std::shared_ptr<T> Get()
    {
        if(staticData)
        {
            return staticData;
        }

        if(!scopedBuilder)
        {
            return nullptr;
        }

        return scopedBuilder();
    }
};

template<typename _T>
::std::shared_ptr<typename DependencyInjector<_T>::T> DependencyInjector<_T>::staticData = nullptr;

template<typename _T>
typename DependencyInjector<_T>::buildScoped_f DependencyInjector<_T>::scopedBuilder = nullptr;

#define DI_Inject(_Var) _Var(DependencyInjector<decltype(_Var)::element_type>::Get())
