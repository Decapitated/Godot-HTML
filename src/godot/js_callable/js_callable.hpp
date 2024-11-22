#ifndef JSCALLABLE_H
#define JSCALLABLE_H

#include <godot_cpp/variant/callable_custom.hpp>

#include <AppCore/JSHelpers.h>

using namespace ultralight;

namespace godot {
    class JSCallable : public CallableCustom
    {
    private:
        JSContextRef context;
        JSValue js_value;
    public:
        JSCallable(JSContextRef p_context, JSValue p_js_value)
            : context(p_context), js_value(p_js_value) {}
        
        virtual uint32_t hash() const override;
        virtual String get_as_text() const override;
        virtual CallableCustom::CompareEqualFunc get_compare_equal_func() const override;
        static bool compare_equal_func(const CallableCustom *p_a, const CallableCustom *p_b);
        virtual CallableCustom::CompareLessFunc get_compare_less_func() const override;
        static bool compare_less_func(const CallableCustom *p_a, const CallableCustom *p_b);
        virtual bool is_valid() const override;
        virtual ObjectID get_object() const override;
        virtual int get_argument_count(bool &r_is_valid) const override;
        virtual void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const override;
    };
}

#endif