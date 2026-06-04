#include "silvermane.h"
#include "skill/skill_silvermane.h"
#include "gui/castvisual/silvermane_castvisual.h"

silvermane::silvermane()
    : Unit(QStringLiteral("大灰狼"))
    , m_skill(std::make_unique<SkillSilvermane>())
    , m_castVisual(std::make_unique<SilvermaneCastVisual>(this, m_skill->castFrames()))
{
    // 近战刺客：高攻高速脆皮
    setMaxHp(3000);                    /* 脆皮 */
    setMaxMana(60);
    setRange(1);                      /* 近战 */
    setAtk(15);                       /* 高攻 */
    setmanaRecoveryAmount(10);
    setAttackSpeed(12);               /* 攻速快 */
    setMoveSpeed(70);                 /* 移速快 */
}

void silvermane::decideState()
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

void silvermane::tickCooldowns(Game* game)
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

void silvermane::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
