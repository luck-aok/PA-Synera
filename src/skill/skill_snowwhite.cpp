#include "skill_snowwhite.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"

SkillSnowWhite::SkillSnowWhite()
    : Skill(QStringLiteral("玻璃棺封印"), 40, 20, 600)
{}

void SkillSnowWhite::execute(Unit* caster, Game* /*game*/)
{
    if (!caster || caster->isDead()) return;

    Unit* target = caster->target();
    if (!target || target->isDead()) return;

    // 封印目标：无法行动、无法被攻击，持续 120 帧 (2秒)
    target->addEffect(SkillEffect{
        EffectType::Sealed,
        120,
        0,
        nullptr
    });
}
