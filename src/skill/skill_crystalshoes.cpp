#include "skill_crystalshoes.h"
#include "crystalshoes_projectile.h"
#include "entity/unit.h"
#include "core/game.h"
#include <QGraphicsScene>

CrystalShoesSkill::CrystalShoesSkill()
    : Skill(QStringLiteral("水晶鞋"), 50, 100, 300)
{}
//Skill::Skill(const QString& name, int manaCost, int castFrames, int cooldownFrames)
//setAttackSpeed(20);//法师普攻比较慢

void CrystalShoesSkill::execute(Unit* caster, Game* game)
{
    Unit* target = caster->target();
    if (!target || target->isDead()) return;

    CrystalShoesProjectile* proj = new CrystalShoesProjectile(target, 150, game->boardGeometry());
    caster->addProjectile(proj);
    game->scene()->addItem(proj->item());

    QPoint casterPos = caster->position();
    proj->setStartPos(game->boardGeometry()->gridToWorld(casterPos.y(), casterPos.x()));
}
