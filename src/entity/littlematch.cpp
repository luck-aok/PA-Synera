#include "littlematch.h"
#include "skill/skill_littlematch.h"
#include "gui/castvisual/littlematch_castvisual.h"

LittleMatch::LittleMatch()
    : Unit(QStringLiteral("小火苗"))
    , m_skill(std::make_unique<SkillLittleMatch>())
    , m_castVisual(std::make_unique<LittleMatchCastVisual>(this, m_skill->castFrames()))
{
    // 远程爆发法师：脆皮高攻，长读条
    setMaxHp(200);
    setHp(200);
    setMaxMana(80);               /* 长蓝条，大招爆发 */
    setRange(3);                  /* 远程 */
    setAtk(15);
    setmanaRecoveryAmount(10);
    setAttackSpeed(25);
    setMoveSpeed(95);
}

void LittleMatch::decideState()
{
    if (state() == State::Dead) return;

    if (m_skill->isCasting()) return;

    if (mana() >= m_skill->manaCost() && m_skill->isReady()) {
        m_castVisual->reset();
        m_skill->startCast();
        setState(State::Casting);
        return;
    }

    Unit::decideState();
}

void LittleMatch::tickCooldowns(Game* game)
{
    tickEffects();

    if (!m_skill->isCasting())
        m_skill->Cast_tickCooldown();

    if (state() == State::Casting) {
        m_castVisual->tick();
        m_skill->tickCast();
        if (!m_skill->isCasting()) {
            m_skill->execute(this, game);
            m_skill->startCooldown();
            setMana(0);
            setState(State::Idle);
        }
        return;
    }

    Unit::tickCooldowns(game);
}

void LittleMatch::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
