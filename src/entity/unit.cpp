#include "unit.h"
#include "projectile.h"
#include "core/boardgeometry.h"

int Unit::s_nextId = 0;

Unit::Unit(const QString& name)
    : m_id(s_nextId++)
    , m_name(name)
    , m_position(0, 0)
{}
Unit::~Unit()
{
    clearProjectiles();
}

void Unit::updateTarget(const QList<Unit*>& allUnits){
    if(m_state == State::Dead)return;

    // 目标死了 → 清除
    if (m_target && m_target->isDead()) {
        m_target = nullptr;
        m_state = State::Idle;
    }

    //原来目标活着也重新评估，因为在默认寻路逻辑（找设定逻辑下最优）下符合条件的对象可能会发生改变
    Unit* newTarget = findTarget(allUnits);
    if (newTarget && newTarget != m_target) {
        m_target = newTarget;
        //m_state = State::Moving;这里不应该耦合状态切换逻辑，从函数名字看来这就是用来找目标的
    }

}

void Unit::tickCooldowns(Game* /*game*/)
{
    //这种是默认不会放技能的角色，只有四种状态，没有Casting状态
    if (m_state == State::Moving && m_moveCooldown > 0)
        m_moveCooldown--;
    if (m_state == State::Attacking && m_attackCooldown > 0)
        m_attackCooldown--;
}

void Unit::decideState()
{
    if (m_state == State::Dead) return;
    if (!m_target) return;

    bool inRange = isInRangeOf(m_target);

    if (m_state == State::Idle) {
        if (inRange) {
            m_state = State::Attacking;
            m_attackCooldown = m_attackSpeed;   // 攻击前摇
        } else {
            m_state = State::Moving;
        }
    } else if (m_state == State::Moving) {
        if (inRange) {
            m_state = State::Attacking;
            m_attackCooldown = m_attackSpeed;
        }
    } else if (m_state == State::Attacking) {
        if (!inRange) m_state = State::Moving;
    }
}

void Unit::resetToDefault()
{
    m_hp = m_maxHp;
    m_mana = 0;
    m_state = State::Idle;
    m_target = nullptr;
    m_path.clear();
    m_moveCooldown = 0;
    m_attackCooldown = 0;
}

Unit* Unit::findTarget(const QList<Unit*>& allUnits){
    Unit* best = nullptr;
    qreal bestDist = 1e18;
    int bestHp = 999999;

    for(Unit* other : allUnits){
        if(!other || other == this)continue;
        if(other->isDead())continue;
        if(other->owner() == m_owner)continue;
        if (other->position().y() == -1) continue; //Bench上的不参战，不要锁定到bench上的单位

        int dx = m_position.x() - other->position().x();
        int dy = m_position.y() - other->position().y();
        qreal dist = dx * dx + dy * dy;

        bool better = false;
        if (dist < bestDist) {
            better = true;
        } else if (dist == bestDist && best) {
            if (other->hp() < bestHp) {        //血量低优先
                    better = true;
            } else if (other->hp() == bestHp) {
                if (other->position().x() <
                    best->position().x()) {   // 从左到右
                    better = true;
                } else if (other->position().x() ==
                               best->position().x() &&
                           other->position().y() <
                               best->position().y()) {  // 从下到上
                    better = true;
                }
            }
        }

        if (better) {
            best = other;
            bestDist = dist;
            bestHp = other->hp();
        }

    }
    return best;
}

void Unit::updateProjectiles()
{
    for (int i = m_projectiles.size() - 1; i >= 0; --i) {
        Projectile* p = m_projectiles[i];

        if (p->tick()) {
            // 到达：目标还活着就扣血
            if (p->target() && !p->target()->isDead()) {
                p->target()->setHp(p->target()->hp() - p->damage());
                if (p->target()->isDead()) {
                    p->target()->setState(State::Dead);
                }
            }
            delete p;
            m_projectiles.removeAt(i);
        }
    }
}

void Unit::clearProjectiles()
{
    qDeleteAll(m_projectiles);
    m_projectiles.clear();
}

Projectile* Unit::createProjectile(Unit* target, BoardGeometry* geo)
{
    return new SimpleProjectile(target, m_atk, geo);
}