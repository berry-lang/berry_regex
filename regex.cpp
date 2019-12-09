#define BERRY_MODULE
#include "berry.h"
#include <regex>

#if defined(_WIN32)
  #define MODULE_API        __declspec(dllexport)
#else
  #define MODULE_API        extern
#endif

class regex_desc {
public:
    regex_desc(const char *text, const char *regex) :
        m_text(text),
        m_regex(regex),
        m_end(std::sregex_iterator()),
        m_it(m_text.begin(), m_text.end(), m_regex)
    {
    }
    bool hasnext() { return m_it != m_end; }
    auto it() { return *m_it; }
    void next() { ++m_it; }

private:
    std::string m_text;
    std::regex m_regex;
    std::sregex_iterator m_end;
    std::sregex_iterator m_it;
};

static int m_match(bvm *vm)
{
    bool res = false;
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        const char *pattern = be_tostring(vm, 1);
        const char *string = be_tostring(vm, 2);
        res = std::regex_match(string, std::regex(pattern));
    }
    be_pushbool(vm, res);
    be_return(vm);
}

static int m_search(bvm *vm)
{
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        const char *pattern = be_tostring(vm, 1);
        const char *string = be_tostring(vm, 2);
        std::cmatch match;
        be_getbuiltin(vm, "list");
        be_newlist(vm);
        if (std::regex_search(string, match, std::regex(pattern))) {
            for (auto i : match) {
                be_pushstring(vm, i.str().c_str());
                be_data_push(vm, -2);
                be_pop(vm, 1);
            }
        }
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_findall(bvm *vm)
{
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        std::regex reg(be_tostring(vm, 1));
        std::string text(be_tostring(vm, 2));
        std::sregex_iterator it(text.begin(), text.end(), reg);
        std::sregex_iterator end;
        be_getbuiltin(vm, "list");
        be_newlist(vm);
        for (; it != end; ++it) {
            be_pushstring(vm, (*it).str(0).c_str());
            be_data_push(vm, -2);
            be_pop(vm, 1);
        }
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int regex_desc_destory(bvm *vm)
{
    delete static_cast<regex_desc*>(be_tocomptr(vm, 1));;
    be_return_nil(vm);
}

static int i_iter(bvm *vm)
{
    be_getupval(vm, 0, 0);
    auto desc = static_cast<regex_desc*>(be_tocomptr(vm, -1));
    if (desc->hasnext()) {
        be_getbuiltin(vm, "list");
        be_newlist(vm);
        for (auto i : desc->it()) {
            be_pushstring(vm, i.str().c_str());
            be_data_push(vm, -2);
            be_pop(vm, 1);
        }
        desc->next();
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_stop_iteration(vm);
    be_return_nil(vm);
}

static int m_finditer(bvm *vm)
{
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        auto desc = new regex_desc(be_tostring(vm, 2), be_tostring(vm, 1));
        be_pushntvclosure(vm, i_iter, 1);
        be_newcomobj(vm, desc, regex_desc_destory);
        be_setupval(vm, -2, 0);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static void bind_member(bvm *vm, const char *attr, bntvfunc f)
{
    be_pushntvfunction(vm, f);
    be_setmember(vm, -2, attr);
    be_pop(vm, 1);
}

extern "C" {
MODULE_API int berry_export(bvm *vm)
{
    be_newmodule(vm);
    bind_member(vm, "match", m_match);
    bind_member(vm, "search", m_search);
    bind_member(vm, "findall", m_findall);
    bind_member(vm, "finditer", m_finditer);
    be_return(vm);
}
}
