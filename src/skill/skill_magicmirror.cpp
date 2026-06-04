#include "skill_magicmirror.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"

SkillMagicMirror::SkillMagicMirror()
    : Skill(QStringLiteral("照见本质"), 40, 40, 400)
{}

void SkillMagicMirror::execute(Unit* caster, Game* /*game*/)
{
    if (!caster || caster->isDead()) return;

    Unit* target = caster->target();
    if (!target || target->isDead()) return;

    // 目标受到伤害 +25%，持续 180 帧 (3秒)
    target->addEffect(SkillEffect{
        EffectType::DamageTakenUp,
        180,        // 3秒
        0.25f,      // 25% 易伤
        nullptr
    });
}
