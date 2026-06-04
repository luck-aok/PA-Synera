#include "skill_silvermane.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"

SkillSilvermane::SkillSilvermane()
    : Skill(QStringLiteral("撕碎剧本"), 60, 50, 300)
{}
//       m_name(name)
//     , m_manaCost(manaCost)
//     , m_castFrames(castFrames)
//     , m_cooldownFrames(cooldownFrames)
// {}
void SkillSilvermane::execute(Unit* caster, Game* /*game*/)
{
    if (!caster || caster->isDead()) return;

    // 自身增益：攻速提升 50%、移速提升 50%，持续 300 帧 (5秒)
    caster->addEffect(SkillEffect{
        EffectType::AttackSpeedUp,
        300,        // 300 帧 = 5秒
        0.5f,       // 50% 增益
        nullptr     // 限时效果，到期自动消失
    });
    caster->addEffect(SkillEffect{
        EffectType::MoveSpeedUp,
        300,
        0.5f,
        nullptr
    });
}
