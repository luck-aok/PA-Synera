#include "magicmirror.h"
#include "skill/skill_magicmirror.h"
#include "gui/castvisual/magicmirror_castvisual.h"

MagicMirror::MagicMirror()
    : Unit(QStringLiteral("魔镜"))
    , m_skill(std::make_unique<SkillMagicMirror>())
    , m_castVisual(std::make_unique<MagicMirrorCastVisual>(this, m_skill->castFrames()))
{
    // 远程辅助：中HP低攻，短蓝快充
    setMaxHp(350);
    setHp(350);
    setMaxMana(40);                  /* 短蓝条，快速放技能 */
    setRange(3);                     /* 远程 */
    setAtk(6);                       /* 低攻 */
    setmanaRecoveryAmount(10);
    setAttackSpeed(25);              /* 慢速普攻 */
    setMoveSpeed(90);
}

void MagicMirror::decideState()
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

void MagicMirror::tickCooldowns(Game* game)
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

void MagicMirror::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
