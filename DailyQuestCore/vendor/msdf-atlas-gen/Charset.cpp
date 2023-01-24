
#include "Charset.h"

namespace msdf_atlas {

static Charset createAsciiCharset() {
    Charset ascii;
    for (unicode_t cp = 0x20; cp < 0x7f; ++cp)
        ascii.add(cp);
    return ascii;
}

const Charset Charset::ASCII = createAsciiCharset();

Charset Charset::MakeForTarget(unsigned int targetCharacter)
{
    Charset set;
    unsigned int interval = 0x80;
    unsigned int start = targetCharacter / interval * interval;
    for (unicode_t cp = start; cp < start + interval; cp++)
        set.add(cp);
    return set;
}

void Charset::add(unicode_t cp) {
    codepoints.insert(cp);
    if (cp > _last) _last = cp;
    if (cp < _first) _first = cp;
}

void Charset::remove(unicode_t cp) {
    codepoints.erase(cp);
}
bool Charset::has(unicode_t cp) {
    return codepoints.find(cp) != codepoints.end();
}

size_t Charset::size() const {
    return codepoints.size();
}

bool Charset::empty() const {
    return codepoints.empty();
}

std::set<unicode_t>::const_iterator Charset::begin() const {
    return codepoints.begin();
}

std::set<unicode_t>::const_iterator Charset::end() const {
    return codepoints.end();
}

}
