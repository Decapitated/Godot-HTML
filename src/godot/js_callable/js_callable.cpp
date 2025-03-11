#include "js_callable.hpp"

#include "convert/convert.hpp"

using namespace godot;

// ALERT: Find a way to fix hash & compare to allow disconnecting callables from signals.
uint32_t JSCallable::hash() const
{
    return 420;
}

godot::String JSCallable::get_as_text() const
{
    return "<JSCallable>";
}

bool JSCallable::is_valid() const
{
    return true;
}

CallableCustom::CompareEqualFunc JSCallable::get_compare_equal_func() const
{
    return &JSCallable::compare_equal_func;
}

bool JSCallable::compare_equal_func(const CallableCustom *p_a, const CallableCustom *p_b)
{
    return p_a == p_b;
}

CallableCustom::CompareLessFunc JSCallable::get_compare_less_func() const
{
    return &JSCallable::compare_less_func;
}

bool JSCallable::compare_less_func(const CallableCustom *p_a, const CallableCustom *p_b)
{
    return (void *)p_a < (void *)p_b;
}

ObjectID JSCallable::get_object() const
{
    return ObjectID();
}

int JSCallable::get_argument_count(bool &r_is_valid) const
{
    r_is_valid = true;
    return 0;
}

void JSCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const
{
    JSFunction js_function = js_value.ToFunction();
    JSArgs js_args = JSArgs();

    for (int i = 0; i < p_argcount; i++)
    {
        js_args.push_back(Convert::ToJSValue(context, *p_arguments[i]));
    }

    r_return_value = Convert::ToVariant(context, js_function(js_args));
    r_call_error.error = GDEXTENSION_CALL_OK;
}
