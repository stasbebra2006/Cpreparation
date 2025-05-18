#ifndef __TRAINER__
#include <sstream>
#include <cassert>

#include "InputAnalyzer.hpp"

int main() {
    {
        std::istringstream iss("jsem rad, ze jsem tuto ulohu sepsal");
        InputAnalyzer a;
        assert(a.analyze(iss).count() == 6);
    }
    {
        std::istringstream iss1("Autobus autobus kruzitko"), iss2("Parnik kruzitko");
        InputAnalyzer a, b, c;
        assert(a.analyze(iss1).analyze(iss2).count() == 4);
        std::istringstream iss1b("Autobus autobus kruzitko"), iss2b("Parnik kruzitko");
        assert(b.analyze(iss2b).analyze(iss1b).count() == 4);
        std::istringstream iss1c("Autobus autobus kruzitko"), iss2c("Parnik kruzitko");
        assert(c.analyze(iss1c).analyze(iss1c).count() == 3);
        InputAnalyzer d = a, e, f;
        e = a;
        assert(d.count() == 4);
        assert(e.count() == 4);
        std::istringstream iss1d("Autobus autobus kruzitko"), iss2d("Parnik kruzitko");
        f.analyze(iss2d);
        assert(f.count() == 2);
        f.analyze(iss1d);
        assert(f.count() == 4);
        const InputAnalyzer & g = a;
        assert(g.count() == 4);
        assert(a.count() == 4);
        assert(a.word("autobus") == 1);
        assert(a.word("kruzitko") == 2);
        assert(a.word("plechovka") == 0);
        assert(a.count() == 4);
        assert(g.count() == 4);
        assert(g.word("autobus") == 1);
        assert(g.word("kruzitko") == 2);
        assert(g.word("plechovka") == 0);
        assert(g.count() == 4);
    }
    return 0;
}

#endif