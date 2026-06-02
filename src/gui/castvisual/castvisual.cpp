#include "castvisual.h"
#include "entity/unit.h"
#include <algorithm>

CastVisual::CastVisual(Unit* caster, int castFrames)
    : m_caster(caster)
    , m_totalFrames(castFrames)
{}

void CastVisual::tick()
{
    if (m_elapsed < m_totalFrames)
        m_elapsed++;
}

float CastVisual::progress() const
{
    if (m_totalFrames <= 0) return 1.0f;
    return std::min(1.0f, static_cast<float>(m_elapsed) / m_totalFrames);
}

void CastVisual::reset()
{
    m_elapsed = 0;
}
