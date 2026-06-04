#include "snowwhite.h"
#include "skill/skill_snowwhite.h"
#include "gui/castvisual/snowwhite_castvisual.h"

SnowWhite::SnowWhite()
    : Unit(QStringLiteral("白雪"))
    , m_skill(std::make_unique<SkillSnowWhite>())
    , m_castVisual(std::make_unique<SnowWhiteCastVisual>(this, m_skill->castFrames()))
{
    // 远程控制：肉坦级HP，低攻短蓝
    setMaxHp(600);
    setHp(600);
    setMaxMana(40);               /* 短蓝条，快速控场 */
    setRange(3);                  /* 远程 */
    setAtk(8);                    /* 低攻 */
    setmanaRecoveryAmount(10);
    setAttackSpeed(30);           /* 慢速普攻 */
    setMoveSpeed(95);             /* 走得也慢 */
}

void SnowWhite::decideState()
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

void SnowWhite::tickCooldowns(Game* game)
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

void SnowWhite::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
