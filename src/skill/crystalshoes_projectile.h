#ifndef CRYSTALSHOES_PROJECTILE_H
#define CRYSTALSHOES_PROJECTILE_H

#include "entity/projectile.h"

class CrystalShoesProjectile : public Projectile
{
public:
    CrystalShoesProjectile(Unit* target, int damage, BoardGeometry* geo);

    void setStartPos(const QPointF& pixelPos);
    bool tick() override;
};

#endif // CRYSTALSHOES_PROJECTILE_H
