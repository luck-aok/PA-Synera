#include "uninvited.h"
#include "skill/skill_uninvited.h"
#include "gui/castvisual/uninvited_castvisual.h"

Uninvited::Uninvited()
    : Unit(QStringLiteral("无邀者"))
    , m_skill(std::make_unique<SkillUninvited>())
    , m_castVisual(std::make_unique<UninvitedCastVisual>(this, m_skill->castFrames()))
{
    // 远程诅咒法师：中HP长蓝，大招诅咒
    setMaxHp(350);
    setHp(350);
    setMaxMana(70);               /* 长蓝条，大招型 */
    setRange(3);                  /* 远程 */
    setAtk(10);
    setmanaRecoveryAmount(10);
    setAttackSpeed(25);
    setMoveSpeed(90);
}

void Uninvited::decideState()
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

void Uninvited::tickCooldowns(Game* game)
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

void Uninvited::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
