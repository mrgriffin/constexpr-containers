#include "../../cexpr/detail/list.hpp"

using cexpr::detail::basic_list;

constexpr basic_list<int, 4> a = { 1, 2, 3 };
constexpr auto b = a.set(1, 3);

static_assert(b[0] == 1, "!=1");
static_assert(b[1] == 3, "!=3");
static_assert(b[2] == 3, "!=3");
//static_assert(b[1] == 4, "!=4");
//static_assert(b[2] == 2, "!=2");
//static_assert(b[3] == 3, "!=3");

constexpr auto c = b.insert(0, 0);
static_assert(c[0] == 0, "!=0");
static_assert(c[1] == 1, "!=1");
static_assert(c[2] == 3, "!=3");
static_assert(c[3] == 3, "!=3");

constexpr auto d = c.erase(0, 1);
static_assert(d[0] == 1, "!=1");
static_assert(d[1] == 3, "!=3");

constexpr auto it1 = d.cbegin();
constexpr auto end = d.cend();

constexpr auto e = d.insert(1, { 2, 3 });
static_assert(e[0] == 1, "!=1");
static_assert(e[1] == 2, "!=2");
static_assert(e[2] == 3, "!=3");
static_assert(e[3] == 3, "!=3");
