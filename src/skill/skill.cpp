#include "skill.h"

Skill::Skill(const QString& name, int manaCost, int castFrames, int cooldownFrames)
    : m_name(name)
    , m_manaCost(manaCost)
    , m_castFrames(castFrames)
    , m_cooldownFrames(cooldownFrames)
{}
// QString m_name;
// int m_manaCost;
// int m_castFrames;
// int m_castFramesRemaining = 0;
// int m_cooldownFrames;
// int m_cooldownRemaining = 0;
// };
void Skill::startCast()
{
    m_castFramesRemaining = m_castFrames;
}

void Skill::tickCast()
{
    if (m_castFramesRemaining > 0)
        m_castFramesRemaining--;
}

bool Skill::isCasting() const
{
    return m_castFramesRemaining > 0;
}

bool Skill::isReady() const
{
    return m_cooldownRemaining == 0;
}

void Skill::startCooldown()
{
    m_cooldownRemaining = m_cooldownFrames;
}

void Skill::Cast_tickCooldown()
{
    if (m_cooldownRemaining > 0)
        m_cooldownRemaining--;
}

void Skill::reset()
{
    m_castFramesRemaining = 0;
    m_cooldownRemaining = 0;
}